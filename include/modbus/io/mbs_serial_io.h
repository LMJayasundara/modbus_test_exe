/* Mbs_Serial_IO header.
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


#ifndef CMS_MBS_SERIAL_IO_H
#define CMS_MBS_SERIAL_IO_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/serial_port.hpp>

#include <string>

#include "modbus/address/addr_space.h"
#include "modbus/diagram/mbs_serial_req_handler.h"
#include "modbus/config/serial_port_info.h"

namespace cms
{
    class Serial_Slave_Session;
	class Serial_Master_Session;
    class Modbus_Station;

    // Class: Mbs_Serial_IO
    // Function: It can work as a modbus master/slave serial station
    class Mbs_Serial_IO
    {
        static int DTOR_CNT;
    public:
	    enum Station_Type {MASTER, SLAVE, HYBIRD, UNKNOWN};
	    Mbs_Serial_IO::Station_Type station_type;

	    enum Message_Type {RTU, ASCII};
	    Mbs_Serial_IO::Message_Type message_type;

	    Mbs_Serial_IO(boost::asio::io_service& io_service, Modbus_Station &mbs_station, const Serial_Port_Info& serial_port_info,
	    		Mbs_Serial_IO::Station_Type stn_type = HYBIRD, Mbs_Serial_IO::Message_Type msg_type = RTU);

    	// start a modbus session
        int start_session(cms::MBS_STATION_TYPE mbs_type);

        // start accept modbus master request
        void start_accept_master_request();

		// start request modbus slave
		void start_request_slave();

        void stop();

		boost::asio::io_service& get_io_service_ref() const;

        ~Mbs_Serial_IO();

    private:
        bool stopped_;

        // handle modbus master request when it recieved the request
    	void handle_master_request(Serial_Slave_Session* new_session,const boost::system::error_code& error);

    	boost::asio::io_service& io_service_;

    	boost::asio::serial_port serial_port_;

    	Mbs_Serial_Req_Handler mbs_serial_request_handler_;

    	boost::shared_ptr<Serial_Slave_Session> slave_session_;

		boost::shared_ptr<Serial_Master_Session> master_session_;

    	Modbus_Station &mbs_station_ref_;
    };
} // namespace cms

#endif
