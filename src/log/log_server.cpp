/* Log_Server Body.
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


#include "log/log_server.h"
#include "log/log_data_model.h"

#include <QDateTime>
#include <QStringList>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

namespace cms
{
	Log_Server::Log_Server(boost::asio::io_service& io_service, short port, Log_Data_Model& log_data_model)
		: io_service_(io_service), socket_(io_service, udp::endpoint(udp::v4(), port)),
		  log_data_model_ref_(log_data_model)
	{
		socket_.async_receive_from(boost::asio::buffer(data_, max_length), sender_endpoint_,
			boost::bind(&Log_Server::handle_receive_from, this, boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

	void Log_Server::handle_receive_from(const boost::system::error_code& error,
		size_t bytes_recvd)
	{
		if (!error && bytes_recvd > 0)
		{
			QString str;
			str = QString::fromLocal8Bit(data_, bytes_recvd);
		
			log_data_model_ref_.append_msg(str);
		}

		socket_.async_receive_from(boost::asio::buffer(data_, max_length), sender_endpoint_,
			boost::bind(&Log_Server::handle_receive_from, this, boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
}