/* Serial_Slave_Session body.
Copyright (C) 2011
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

#include "modbus/io/serial_slave_session.h"
#include "modbus/io/mbs_serial_io.h"
#include "modbus/station/modbus_station.h"
#include "modbus/analyzer/mbs_analyzer.h"
#include "modbus/diagram/diagram_validator.h"
#include "cms_config.h"

#ifdef USE_NS_DEBUG
#include <iostream>
#endif

using namespace std;

namespace cms
{
    void print_buf_data(const std::vector<unsigned char> &buffer, unsigned int data_cnt)
    {
        for (unsigned int i=0; i<data_cnt; ++i)
        {
            unsigned char a = buffer[i];
            cout << std::hex << setw(2) << (int)a;
        }
        cout << "\n";
    }

    // Class: Serial_Slave_Session
    // Function: Process modbus master request and then give it response
    int Serial_Slave_Session::DTOR_CNT = 0;
	int Serial_Slave_Session::CTOR_CNT = 0;

    Serial_Slave_Session::Serial_Slave_Session(Mbs_Serial_IO &mbs_serial_io,
        Modbus_Station &mbs_station,
        Mbs_Serial_Req_Handler& mbs_serial_req_handler, boost::asio::serial_port &serial_port)
        : recv_buffer_(DIAGRAM_MAX_LEN), complete_buffer_(DIAGRAM_MAX_LEN),
        mbs_serial_io_ref_(mbs_serial_io), mbs_station_ref_(mbs_station),
        mbs_serial_request_handler_(mbs_serial_req_handler),
        serial_port_ref_(serial_port)
    {
        mbs_analyzer_ = boost::shared_ptr<Mbs_Analyzer>(new Mbs_Analyzer("MBS-SS"));
        mbs_analyzer_->init();

        diagram_validator_ = boost::shared_ptr<Diagram_Validator>(new Diagram_Validator());

#ifdef USE_NS_DEBUG
        ++CTOR_CNT;
        std::cerr << "Serial_Slave_Session CTOR count: " << CTOR_CNT << std::endl;
#endif
    }

    void Serial_Slave_Session::start()
    {
        /*async_read(serial_port_ref_, boost::asio::buffer(recv_buffer_),
        boost::bind(&Serial_Slave_Session::handle_request, this,boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));*/
        serial_port_ref_.async_read_some(boost::asio::buffer(recv_buffer_, DIAGRAM_MAX_LEN),
            boost::bind(&Serial_Slave_Session::handle_request, this,boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }

    // handle modbus master request
    void Serial_Slave_Session::handle_request(const boost::system::error_code& error, size_t bytes_transferred)
    {     
        if (!error)
        {            
            cout << "handle request, data lenght: " << bytes_transferred << "\n";
            //print_buf_data(recv_buffer_, bytes_transferred);            
            MBS_VALIDATE_RC rc = diagram_validator_->validate_serial_master_requst(recv_buffer_, complete_buffer_, bytes_transferred);
            if (rc==V_OK)
            {
				cout << "validate ok\n";
                mbs_analyzer_->append_record(&(complete_buffer_[0]), diagram_validator_->get_recv_bytes_count(), RX);
                mbs_serial_request_.slave_address = complete_buffer_[0];
                mbs_serial_request_.function_code = complete_buffer_[1];
                
                mbs_serial_request_.diagram.resize(diagram_validator_->get_recv_bytes_count());         
                std::copy(complete_buffer_.begin(), complete_buffer_.begin() + diagram_validator_->get_recv_bytes_count(), mbs_serial_request_.diagram.begin());
                mbs_serial_request_handler_.handle_request(mbs_serial_request_, mbs_serial_response_, mbs_station_ref_);
                diagram_validator_->clear_complete_buffer();
                boost::asio::async_write(serial_port_ref_, boost::asio::buffer(mbs_serial_response_.diagram),
                    boost::bind(&Serial_Slave_Session::handle_response, this, boost::asio::placeholders::error));	
            }
            else if (rc ==DISCARD_UNTIL_NOT_FULL)
            {
				cout << "display discard bytes\n";
                mbs_analyzer_->append_record(&(diagram_validator_->get_discard_buffer()[0]), diagram_validator_->get_discard_bytes_count(), DISCARD);
                handle_request(error, 0);
            }
            else if (rc==RECV_AGAIN)
            {
				cout << "recv again\n";
                serial_port_ref_.async_read_some(boost::asio::buffer(recv_buffer_, DIAGRAM_MAX_LEN),
                    boost::bind(&Serial_Slave_Session::handle_request, this,boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
            }
			else if (rc==DISCARD_ALL)
			{
				cout << "discard all\n";
				mbs_analyzer_->append_record(&(diagram_validator_->get_discard_buffer()[0]), diagram_validator_->get_discard_bytes_count(), DISCARD);
                serial_port_ref_.async_read_some(boost::asio::buffer(recv_buffer_, DIAGRAM_MAX_LEN),
                    boost::bind(&Serial_Slave_Session::handle_request, this,boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
			}
        }
        else
        {
            //delete this;
            cerr << "serial slave session handle request error!: " << error.message() << "\n";
        }
    }

    // handle modbus master response
    void Serial_Slave_Session::handle_response(const boost::system::error_code& error)
    {
        if (!error)
        {
            //cout << "handle response\n";
            //print_buf_data(buffer_, buffer_.size());
            // get another modbus master request
            if (mbs_serial_response_.diagram.size() > 0)
            {
                mbs_analyzer_->append_record(&(mbs_serial_response_.diagram[0]), mbs_serial_response_.diagram.size(), TX);
            }

            serial_port_ref_.async_read_some(boost::asio::buffer(recv_buffer_, DIAGRAM_MAX_LEN),
                boost::bind(&Serial_Slave_Session::handle_request, this,boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));

            /*async_read(serial_port_ref_, boost::asio::buffer(recv_buffer_),
            boost::bind(&Serial_Slave_Session::handle_request, this,boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
            cout << "handle response\n";
            print_buf_data(recv_buffer_, recv_buffer_.size());*/
        }
        else
        {
			cerr << "serial slave session handle response error: " << error.message() << "\n";
            //delete this;
        }
    }

    Serial_Slave_Session::~Serial_Slave_Session()
    {
#ifdef USE_NS_DEBUG
        ++DTOR_CNT;
        std::cerr << "Serial_Slave_Session DTOR count: " << DTOR_CNT << std::endl;
#endif
    }

} // namespace cms

