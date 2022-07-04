/* Log_Sender Body.
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

#include <QLocalSocket>
#include <QByteArray>

#include "log/log_sender.h"

#include <string>
#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::udp;

enum { max_length = 1024 };

namespace cms
{
	void Log_Sender::send_msg(const QString& msg)
	{
		Log_Sender::send_msg_udp(msg);
	}

	void Log_Sender::send_msg_udp(const QString& msg)
	{
		try
		{
			boost::asio::io_service io_service;
			udp::socket msg_sender_socket(io_service);
			udp::resolver resolver(io_service);
			string address = "127.0.0.1";
			string log_svr_listen_port = "61234";
			udp::resolver::query query(udp::v4(), address, "61234");
			udp::resolver::iterator iterator = resolver.resolve(query);
			msg_sender_socket.open(udp::v4());
			msg_sender_socket.send_to(boost::asio::buffer(msg.toLocal8Bit().data(), msg.size()), *iterator);
			msg_sender_socket.close();
		}
		catch (std::exception& e)
		{
			std::cerr << "UDP Log Sender Exception: " << e.what() << "\n";
		}
	}
}