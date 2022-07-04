/* Serial_Slave_Session header.
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


#ifndef CMS_SERIAL_SLAVE_SESSION_H
#define CMS_SERIAL_SLAVE_SESSION_H

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "modbus/diagram/mbs_serial_request.h"
#include "modbus/diagram/mbs_serial_response.h"

namespace cms
{
    void print_buf_data(const std::vector<unsigned char> &buffer, unsigned int data_cnt);

    class Modbus_Station;
    class Mbs_Serial_IO;
    class Mbs_Serial_Req_Handler;
	class Mbs_Analyzer;
    class Diagram_Validator;

    // Class: Serial_Slave_Session
    // Function: Process modbus master request and then give it response
    class Serial_Slave_Session
    {
        static int DTOR_CNT;
		static int CTOR_CNT;
    public:
    	Serial_Slave_Session(Mbs_Serial_IO &mbs_serial_io, Modbus_Station &mbs_station,
    			Mbs_Serial_Req_Handler& mbs_serial_req_handler, boost::asio::serial_port &serial_port);
        ~Serial_Slave_Session();

    	void start();

    private:
    	// handle modbus master request
    	void handle_request(const boost::system::error_code& error, size_t bytes_transferred);

    	// handle the modbus master response
    	void handle_response(const boost::system::error_code& error);

        // buffer to store resquest data and then copy to mbs_serial_request
        std::vector<unsigned char> recv_buffer_;
        std::vector<unsigned char> complete_buffer_;

        Mbs_Serial_IO &mbs_serial_io_ref_;
        Modbus_Station &mbs_station_ref_;

    	Mbs_Serial_Req_Handler &mbs_serial_request_handler_;

    	Mbs_Serial_Request mbs_serial_request_; // request from master 
    	Mbs_Serial_Response mbs_serial_response_; // response to master

    	boost::asio::serial_port &serial_port_ref_;

		boost::shared_ptr<Mbs_Analyzer> mbs_analyzer_;

        boost::shared_ptr<Diagram_Validator> diagram_validator_;
    };
} // namespace cms

#endif
