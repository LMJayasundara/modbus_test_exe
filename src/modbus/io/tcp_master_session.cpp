/* Tcp_Master_Session body.
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

#include "modbus/io/tcp_master_session.h"
#include "modbus/io/mbs_tcp_io.h"
#include "modbus/station/modbus_station.h"
#include "modbus/diagram/mbs_tcp_master.h"
#include "modbus/config/mbs_def.h"
#include "modbus/analyzer/mbs_analyzer.h"
#include "log/log_sender.h"
#include "cms_config.h"

#ifdef USE_NS_DEBUG
#include <iostream>
#endif

using namespace std;

namespace cms
{
    void print_buf_data3(const std::vector<unsigned char> &buffer, unsigned int data_cnt)
    {
    	for (unsigned int i=0; i<data_cnt; ++i)
    	{
    		unsigned char a = buffer[i];
    		cout << std::hex << setw(2) << (int)a;
    	}
    	cout << "\n";
    }

    // Class: Tcp_Master_Session
    // Function: Process modbus master request and then give it response
    int Tcp_Master_Session::DTOR_CNT = 0;

    Tcp_Master_Session::Tcp_Master_Session(boost::asio::io_service& io_service, Mbs_Tcp_IO &mbs_tcp_io,
    		Modbus_Station &mbs_station)
        : stopped_(false), socket_(io_service), mbs_tcp_io_ref_(mbs_tcp_io), mbs_station_ref_(mbs_station),
		  retry_timer_(mbs_tcp_io.get_io_service_ref()), interval_timer_(mbs_tcp_io.get_io_service_ref())
    {
        current_master_group_id_ = 0;
		max_master_group_id_ = mbs_station.get_tcp_master_group_count();
        is_last_request_success_ = false;

		mbs_analyzer_ = boost::shared_ptr<Mbs_Analyzer>(new Mbs_Analyzer("MBS-TM"));
        mbs_analyzer_->init();
    }

    void Tcp_Master_Session::start()
    {
		this->send_request();
    }

    void Tcp_Master_Session::stop()
    {
        stopped_ = true;
        retry_timer_.cancel();
        interval_timer_.cancel();
        this->socket_.close();
    }

    // send modbus master request
    void Tcp_Master_Session::send_request()
    {
        if (stopped_)
            return;

		is_last_request_success_ = false;
		boost::shared_ptr<Mbs_Tcp_Master> mbs_tcp_master = mbs_station_ref_.get_tcp_master_group(current_master_group_id_);
        mbs_tcp_master->make_request();
		retry_timer_.expires_from_now(boost::posix_time::seconds(3));
		retry_timer_.async_wait(boost::bind(&Tcp_Master_Session::handle_request_wait, this));
		boost::asio::async_write(socket_, boost::asio::buffer(mbs_tcp_master->get_request_buffer()),
			boost::bind(&Tcp_Master_Session::handle_request, this, boost::asio::placeholders::error));	
		mbs_analyzer_->append_record(&(mbs_tcp_master->get_request_buffer())[0], mbs_tcp_master->get_request_length(), TX);
    }

    void Tcp_Master_Session::handle_request(const boost::system::error_code& error)
    {
        if (stopped_)
            return;

    	if (!error)
    	{		
            cout << "handle tcp request\n";
            boost::shared_ptr<Mbs_Tcp_Master> mbs_tcp_master = mbs_station_ref_.get_tcp_master_group(current_master_group_id_);
			socket_.async_read_some(boost::asio::buffer(mbs_tcp_master->get_response_buffer(), mbs_tcp_master->get_response_length()),
				boost::bind(&Tcp_Master_Session::handle_response, this,boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));             
    	}
    	else
    	{
            Log_Sender::send_msg(QString("Modbus TCP Master handle request error: %1").arg(error.message().c_str()));
    		delete this;
    	}
    }

	void Tcp_Master_Session::handle_request_wait()
	{
        if (!is_last_request_success_)
        {
            send_request();
        }
	}

	void Tcp_Master_Session::handle_response(const boost::system::error_code& error, size_t bytes_transferred)
	{
        if (stopped_)
            return;

		if (!error)
		{
            boost::shared_ptr<Mbs_Tcp_Master> mbs_tcp_master = mbs_station_ref_.get_tcp_master_group(current_master_group_id_);
            mbs_tcp_master->handle_response();
			mbs_analyzer_->append_record(&(mbs_tcp_master->get_response_buffer())[0], mbs_tcp_master->get_response_buffer().size(), RX);
			print_buf_data3(mbs_tcp_master->get_response_buffer(), bytes_transferred);
            is_last_request_success_ = true;
           
			retry_timer_.cancel();

			++current_master_group_id_;
			if (current_master_group_id_ >=  max_master_group_id_)
			{
				current_master_group_id_ = 0;
			}

			interval_timer_.expires_from_now(boost::posix_time::seconds(1));
			interval_timer_.async_wait(boost::bind(&Tcp_Master_Session::send_request, this));
		}
        else
        {
            Log_Sender::send_msg(QString("Modbus TCP Master handle response error: %1").arg(error.message().c_str()));
        }
	}

    boost::asio::ip::tcp::socket& Tcp_Master_Session::socket()
    {
        return this->socket_;
    }

    Tcp_Master_Session::~Tcp_Master_Session()
    {
#ifdef USE_NS_DEBUG
        ++DTOR_CNT;
        std::cerr << "Tcp_Master_Session DTOR count: " << DTOR_CNT << std::endl;
#endif
    }
} // namespace cms

