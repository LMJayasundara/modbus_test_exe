/* Mbs_Tcp_IO header.
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


#ifndef CMS_MBS_TCP_IO_H
#define CMS_MBS_TCP_IO_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include <string>

#include "modbus/address/addr_space.h"
#include "modbus/diagram/mbs_tcp_req_handler.h"
#include "modbus/config/tcp_port_info.h"

namespace cms
{
    class Tcp_Slave_Session;
	class Tcp_Master_Session;
    class Modbus_Station;

    // Class: Mbs_Tcp_IO
    // Function: It can work as a modbus master/slave TCP station
    class Mbs_Tcp_IO
    {
        static int DTOR_CNT;
    public:
	    enum Station_Type {MASTER, SLAVE, HYBIRD, UNKNOWN};
	    Mbs_Tcp_IO::Station_Type station_type;

	    enum Message_Type {RTU, ASCII};
	    Mbs_Tcp_IO::Message_Type message_type;

    	Mbs_Tcp_IO(boost::asio::io_service& io_service, Modbus_Station &mbs_station, const TCP_Port_Info& tcp_port_info,
    			Mbs_Tcp_IO::Station_Type stn_type= HYBIRD, Mbs_Tcp_IO::Message_Type msg_type = RTU);
        ~Mbs_Tcp_IO();

    	// initialize modbus station
    	void init();

    	// start a modbus session
        int start_session(cms::MBS_STATION_TYPE mbs_type);

        // start accept modbus master connect
        void start_accept_master_connect();

        // connect to a modbus slave
        void connect_slave();
        // handle slave connection
        void handle_slave_connect(const boost::system::error_code& ec);
        // start request modbus slave
        void start_request_slave();        

        void stop();

		boost::asio::io_service& get_io_service_ref() const;

    private:
        bool stopped_;

        // handle modbus master tcp connection
    	void handle_accept(Tcp_Slave_Session* new_session,const boost::system::error_code& error);

    	boost::asio::io_service& io_service_;

        TCP_Port_Info tcp_port_info_;

    	boost::shared_ptr<boost::asio::ip::tcp::acceptor> slave_acceptor_;

    	Mbs_Tcp_Req_Handler mbs_tcp_request_handler_;

        boost::asio::deadline_timer connect_deadline_timer_;
        boost::asio::ip::tcp::resolver::iterator slave_endpoint_iter_;
        boost::shared_ptr<Tcp_Master_Session> master_session_;

    	Modbus_Station &mbs_station_ref_;
    };
} // namespace cms

#endif
