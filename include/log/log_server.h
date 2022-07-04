/* Log_Server Header.
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

#ifndef CMS_LOG_SERVER_H
#define CMS_LOG_SERVER_H

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

namespace cms
{
	class Log_Data_Model;

	class Log_Server
	{
	public:
		Log_Server(boost::asio::io_service& io_service, short port, Log_Data_Model& log_data_model);

		void handle_receive_from(const boost::system::error_code& error,size_t bytes_recvd);

	private:
		boost::asio::io_service& io_service_;
		udp::socket socket_;
		udp::endpoint sender_endpoint_;
		enum { max_length = 1024 };
		char data_[max_length];

		Log_Data_Model &log_data_model_ref_;
	};
}

#endif