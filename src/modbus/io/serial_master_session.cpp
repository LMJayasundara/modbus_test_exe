/* Serial_Master_Session body.
Copyright (C) 2013
newsuppy

This file is part of SmartDCS.comm_svr.

SmartDCS.comm_svr is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

SmartDCS.comm_svr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SmartDCS.comm_svr; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include <iostream>
#include <iomanip>

#include <boost/bind.hpp>

#include "modbus/io/serial_master_session.h"
#include "modbus/io/mbs_serial_io.h"
#include "modbus/station/modbus_station.h"
#include "modbus/diagram/mbs_serial_master.h"
#include "modbus/config/mbs_def.h"
#include "modbus/analyzer/mbs_analyzer.h"
#include "modbus/diagram/diagram_validator.h"
#include "cms_config.h"

#ifdef USE_NS_DEBUG
#include <iostream>
#endif

using namespace std;

namespace cms
{
    void print_buf_data2(const std::vector<unsigned char> &buffer, unsigned int data_cnt)
    {
        for (unsigned int i=0; i<data_cnt; ++i)
        {
            unsigned char a = buffer[i];
            cout << std::hex << setw(2) << (int)a;
        }
        cout << "\n";
    }

    // Class: Serial_Master_Session
    // Function: Process modbus master request and then give it response

    int Serial_Master_Session::DTOR_CNT = 0;

    Serial_Master_Session::Serial_Master_Session(Mbs_Serial_IO &mbs_serial_io, Modbus_Station &mbs_station, boost::asio::serial_port &serial_port)
        : stopped_(false), recv_buffer_(DIAGRAM_MAX_LEN), mbs_serial_io_ref_(mbs_serial_io), mbs_station_ref_(mbs_station),
        serial_port_ref_(serial_port), 
        retry_timer_(mbs_serial_io.get_io_service_ref()), interval_timer_(mbs_serial_io.get_io_service_ref())
    {
        current_master_group_id_ = 0;
        max_master_group_id_ = mbs_station.get_serial_master_group_count();
        is_last_request_success_ = false;

        mbs_analyzer_ = boost::shared_ptr<Mbs_Analyzer>(new Mbs_Analyzer("MBS-SM"));
        mbs_analyzer_->init();

        diagram_validator_ = boost::shared_ptr<Diagram_Validator>(new Diagram_Validator());
    }

    void Serial_Master_Session::start()
    {
        this->send_request();
    }

    void Serial_Master_Session::stop()
    {
        stopped_ = true;
        this->serial_port_ref_.close();
    }

    // send modbus master request
    void Serial_Master_Session::send_request()
    {
        if (stopped_)
            return;

        is_last_request_success_ = false;

        //request_parser_.parse(mbs_serial_request_, buffer_);
        //mbs_serial_request_handler_.handle_request(mbs_serial_request_, mbs_serial_response_, mbs_station_ref_);
        boost::shared_ptr<Mbs_Serial_Master> mbs_serial_master = mbs_station_ref_.get_serial_master_group(current_master_group_id_);
        mbs_serial_master->make_request();
        retry_timer_.expires_from_now(boost::posix_time::seconds(3));
        retry_timer_.async_wait(boost::bind(&Serial_Master_Session::handle_request_wait, this));
        boost::asio::async_write(serial_port_ref_, boost::asio::buffer(mbs_serial_master->get_request_buffer()),
            boost::bind(&Serial_Master_Session::handle_request, this, boost::asio::placeholders::error));	
        mbs_analyzer_->append_record(&(mbs_serial_master->get_request_buffer())[0], mbs_serial_master->get_request_length(), TX);
        //print_buf_data2(mbs_serial_master->get_request_buffer(), 8);

    }

    void Serial_Master_Session::handle_request(const boost::system::error_code& error)
    {
        if (stopped_)
            return;

        if (!error)
        {		
            boost::shared_ptr<Mbs_Serial_Master> mbs_serial_master = mbs_station_ref_.get_serial_master_group(current_master_group_id_);
            /*serial_port_ref_.async_read_some(boost::asio::buffer(mbs_serial_master->get_response_buffer(), mbs_serial_master->get_response_length()),
            boost::bind(&Serial_Master_Session::handle_response, this,boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));*/

            serial_port_ref_.async_read_some(boost::asio::buffer(recv_buffer_, DIAGRAM_MAX_LEN), //mbs_serial_master->get_response_length()), // the max read bytes has been calculated by master
                boost::bind(&Serial_Master_Session::handle_response, this,boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        }
        else
        {
            delete this;
            cerr << "serial master session handle request error!\n";
        }
    }

    void Serial_Master_Session::handle_request_wait()
    {
        if (stopped_)
            return;

        if (!is_last_request_success_)
        {
            send_request();
        }
    }

    void Serial_Master_Session::handle_response(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (stopped_)
            return;

        if (!error)
        {
            //cout << "response bytes: " << bytes_transferred << "\n";
            boost::shared_ptr<Mbs_Serial_Master> mbs_serial_master = mbs_station_ref_.get_serial_master_group(current_master_group_id_);
            MBS_VALIDATE_RC rc = diagram_validator_->validate_serial_slave_response(recv_buffer_, 
                mbs_serial_master->get_response_buffer(), bytes_transferred, 
                (Function_Code)(mbs_serial_master->get_function_code()), mbs_serial_master->get_response_length());
            if (rc==V_OK)
            {
                mbs_serial_master->handle_response();
                mbs_analyzer_->append_record(&(mbs_serial_master->get_response_buffer())[0], diagram_validator_->get_recv_bytes_count(), RX);
                //print_buf_data2(mbs_serial_master->get_response_buffer(), bytes_transferred);
                is_last_request_success_ = true;
                diagram_validator_->clear_complete_buffer();

                retry_timer_.cancel();

                diagram_validator_->clear_complete_buffer();
                ++current_master_group_id_;
                if (current_master_group_id_ >=  max_master_group_id_)
                {
                    current_master_group_id_ = 0;
                }

                interval_timer_.expires_from_now(boost::posix_time::seconds(1));
                interval_timer_.async_wait(boost::bind(&Serial_Master_Session::send_request, this));
            }
            else if (rc==DISCARD_FIRST)
            {
                mbs_analyzer_->append_record(&(diagram_validator_->get_discard_buffer()[0]), diagram_validator_->get_discard_bytes_count(), DISCARD);
                handle_response(error, 0);
            }
            else if (rc==RECV_AGAIN)
            {
                serial_port_ref_.async_read_some(boost::asio::buffer(recv_buffer_, DIAGRAM_MAX_LEN), // the max read bytes has been calculated by master
                    boost::bind(&Serial_Master_Session::handle_response, this,boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
            }
        }
        else
        {
            delete this;
            cerr << "serial master session handle response error!\n";
        }
    }

    Serial_Master_Session::~Serial_Master_Session()
    {
#ifdef USE_NS_DEBUG
        ++DTOR_CNT;
        std::cerr << "Serial_Master_Session DTOR count: " << DTOR_CNT << std::endl;
#endif
    }
} // namespace cms

