/* Slave_Session body.
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

#include "modbus/io/tcp_slave_session.h"
#include "modbus/station/modbus_station.h"
#include "modbus/diagram/mbs_tcp_req_handler.h"
#include "modbus/analyzer/mbs_analyzer.h"
#include "cms_config.h"

#ifdef USE_NS_DEBUG
#include <iostream>
#endif

using namespace std;

namespace cms
{
    void print_data2(unsigned char *p, unsigned int diagram_cnt)
    {
    	for (unsigned int i=0; i<diagram_cnt; ++i)
    	{
    		char a = p[i];
    		cout << std::hex << setw(3) << (int)a;
    	}
    	cout << "\n";
    }

    // Class: Slave_Session
    // Function: Process modbus master request and then give it response
    int Tcp_Slave_Session::DTOR_CNT = 0;

    Tcp_Slave_Session::Tcp_Slave_Session(boost::asio::io_service& io_service, Mbs_Tcp_IO &mbs_tcp_io,
    		Modbus_Station &mbs_station,
    		Mbs_Tcp_Req_Handler& mbs_tcp_req_handler)
        : socket_(io_service), buffer_(DIAGRAM_MAX_LEN),
          mbs_tcp_io_ref_(mbs_tcp_io), mbs_station_ref_(mbs_station),
          mbs_tcp_request_handler_(mbs_tcp_req_handler)
    {
        mbs_analyzer_ = boost::shared_ptr<Mbs_Analyzer>(new Mbs_Analyzer("MBS-TS"));
        mbs_analyzer_->init();
    }

    boost::asio::ip::tcp::socket& Tcp_Slave_Session::socket()
    {
    	return socket_;
    }

    void Tcp_Slave_Session::start()
    {
    	socket_.async_read_some(boost::asio::buffer(buffer_, DIAGRAM_MAX_LEN),
    			boost::bind(&Tcp_Slave_Session::handle_request, this,boost::asio::placeholders::error,
    					boost::asio::placeholders::bytes_transferred));
    }

    // handle modbus master request
    void Tcp_Slave_Session::handle_request(const boost::system::error_code& error, size_t bytes_transferred)
    {
    	if (!error)
    	{
			//cout << "requset: ";
			//print_data2(&buffer_[0], bytes_transferred);
            mbs_analyzer_->append_record(&(buffer_[0]), bytes_transferred, RX);
            mbs_tcp_request_.transaction_identifier = (unsigned int)(buffer_[0]*0x100) + buffer_[1];
            mbs_tcp_request_.protocol_identifier = (unsigned int)(buffer_[2]*0x100) + buffer_[3];
            mbs_tcp_request_.length = (unsigned int)(buffer_[4]*0x100)  + buffer_[5];
            mbs_tcp_request_.unit_identifier = buffer_[6];
            mbs_tcp_request_.function_code = buffer_[7];

            // copy buffer data to request.diagram
            mbs_tcp_request_.diagram.resize(bytes_transferred);
            std::copy(buffer_.begin(), buffer_.begin() + bytes_transferred, mbs_tcp_request_.diagram.begin());

    		mbs_tcp_request_handler_.handle_request(mbs_tcp_request_, mbs_tcp_response_, mbs_station_ref_);
    	    boost::asio::async_write(socket_, boost::asio::buffer(mbs_tcp_response_.diagram),
    	    		  boost::bind(&Tcp_Slave_Session::handle_response, this, boost::asio::placeholders::error));
			
			//cout << "local endpoint: " << socket_.local_endpoint().address().to_string() << ":" << socket_.local_endpoint().port() <<"\n";
			//cout << "remote endpoint: " << socket_.remote_endpoint().address().to_string() << ":" << socket_.local_endpoint().port() <<"\n";
			//cout << "response: ";
			//print_data2(&mbs_tcp_response_.diagram[0], mbs_tcp_response_.diagram.size());
    	}
    	else
    	{
    		delete this;
    	}
    }

    // handle modbus master response
    void Tcp_Slave_Session::handle_response(const boost::system::error_code& error)
    {
    	if (!error)
    	{
            mbs_analyzer_->append_record(&(mbs_tcp_response_.diagram[0]), mbs_tcp_response_.diagram.size(), TX);
    		// get another modbus master request
    		socket_.async_read_some(boost::asio::buffer(buffer_, DIAGRAM_MAX_LEN),
    				boost::bind(&Tcp_Slave_Session::handle_request, this,boost::asio::placeholders::error,
    						boost::asio::placeholders::bytes_transferred));
    	}
    	else
    	{
    		delete this;
    	}
    }

    Tcp_Slave_Session::~Tcp_Slave_Session()
    {
        socket_.close();

#ifdef USE_NS_DEBUG
        ++DTOR_CNT;
        std::cerr << "Tcp_Slave_Session DTOR count: " << DTOR_CNT << std::endl;
#endif
    }
} // namespace cms

