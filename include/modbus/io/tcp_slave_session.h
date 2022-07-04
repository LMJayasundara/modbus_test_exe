/* Tcp_Slave_Session header.
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


#ifndef CMS_SLAVE_SESSION_H
#define CMS_SLAVE_SESSION_H

#include <boost/asio.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "modbus/diagram/mbs_tcp_request.h"
#include "modbus/diagram/mbs_tcp_response.h"

namespace cms
{
    void print_data(char *p, unsigned int data_cnt);

    class Mbs_Tcp_Req_Handler;
    class Mbs_Tcp_IO;
    class Modbus_Station;
    class Mbs_Analyzer;

    // Class: Tcp_Slave_Session
    // Function: Process modbus master request and then give it response
    class Tcp_Slave_Session
    {
        static int DTOR_CNT;
    public:
    	Tcp_Slave_Session(boost::asio::io_service& io_service, Mbs_Tcp_IO &mbs_tcp_io, Modbus_Station &mbs_station,
    			Mbs_Tcp_Req_Handler& mbs_tcp_req_handler);

    	boost::asio::ip::tcp::socket& socket();

    	void start();

		~Tcp_Slave_Session();

    private:
    	// handle modbus master request
    	void handle_request(const boost::system::error_code& error, size_t bytes_transferred);

    	// handle the modbus master response
    	void handle_response(const boost::system::error_code& error);

    	boost::asio::ip::tcp::socket socket_;
    
        // buffer to store resquest data and then copy to mbs_serial_request
        std::vector<unsigned char> buffer_;

        Mbs_Tcp_IO &mbs_tcp_io_ref_;
    	Modbus_Station &mbs_station_ref_;

    	Mbs_Tcp_Req_Handler &mbs_tcp_request_handler_;

    	Mbs_Tcp_Request mbs_tcp_request_;
    	Mbs_Tcp_Response mbs_tcp_response_;

        boost::shared_ptr<Mbs_Analyzer> mbs_analyzer_;
    };
} // namespace cms

#endif
