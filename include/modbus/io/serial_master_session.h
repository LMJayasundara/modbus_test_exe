/* Serial_Master_Session header.
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


#ifndef CMS_SERIAL_MASTER_SESSION_H
#define CMS_SERIAL_MASTER_SESSION_H

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <ctime>

#include "modbus/diagram/mbs_serial_request.h"
#include "modbus/diagram/mbs_serial_response.h"

namespace cms
{
	class Modbus_Station;
    class Mbs_Serial_IO;
	class Mbs_Analyzer;
    class Diagram_Validator;

    // Class: Serial_Master_Session
    // Function: Send modbus master request and process response 
    class Serial_Master_Session
    {
        static int DTOR_CNT;
    public:
    	Serial_Master_Session(Mbs_Serial_IO &mbs_serial_io, Modbus_Station &mbs_station,
    			boost::asio::serial_port &serial_port);
        ~Serial_Master_Session();

    	void start();
        void stop();

    private:
    	// send modbus master request
    	void send_request();

    	// handle the modbus master request
    	void handle_request(const boost::system::error_code& error);

		// handle the modbus master request wait, it run when the timer expired or canceled
		void handle_request_wait();

		// handle the modbus slave response
		void handle_response(const boost::system::error_code& error, size_t bytes_transferred);

        std::vector<unsigned char> recv_buffer_;

        bool stopped_;

        Mbs_Serial_IO &mbs_serial_io_ref_;
        Modbus_Station &mbs_station_ref_;

    	boost::asio::serial_port &serial_port_ref_;

		boost::asio::deadline_timer retry_timer_;
		boost::asio::deadline_timer interval_timer_;

		unsigned int current_master_group_id_;
		unsigned int max_master_group_id_;
        bool is_last_request_success_;

		boost::shared_ptr<Mbs_Analyzer> mbs_analyzer_;

        boost::shared_ptr<Diagram_Validator> diagram_validator_;
    };
} // namespace cms

#endif
