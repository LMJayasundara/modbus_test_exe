/* Mbs_Serial_IO body.
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

#include "modbus/io/mbs_serial_io.h"
#include "modbus/io/serial_slave_session.h"
#include "modbus/io/serial_master_session.h"
#include "modbus/address/addr_space.h"
#include "modbus/config/mbs_def.h"
#include "cms_config.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#ifdef USE_NS_DEBUG
#include <iostream>
#endif

using namespace std;
using namespace boost;
using namespace boost::asio;

namespace cms
{
    int Mbs_Serial_IO::DTOR_CNT = 0;

    Mbs_Serial_IO::Mbs_Serial_IO(boost::asio::io_service& io_service, Modbus_Station &mbs_station, const Serial_Port_Info& serial_port_info,
    		Mbs_Serial_IO::Station_Type stn_type, Mbs_Serial_IO::Message_Type msg_type)
        : stopped_(false), io_service_(io_service), mbs_station_ref_(mbs_station), serial_port_(io_service_, serial_port_info.port_name),
          station_type(stn_type), message_type(msg_type)
    {
		serial_port_.set_option(serial_port::baud_rate(serial_port_info.baud_rate));
		if (serial_port_info.flow_ctrl == "hardware")
		{
			serial_port_.set_option(serial_port::flow_control(serial_port::flow_control::hardware));
		}
		else if (serial_port_info.flow_ctrl == "software")
		{
			serial_port_.set_option(serial_port::flow_control(serial_port::flow_control::software));
		}
		else
		{
			serial_port_.set_option(serial_port::flow_control(serial_port::flow_control::none));
		}
		
		if (serial_port_info.parity == "even")
		{
			serial_port_.set_option(serial_port::parity(serial_port::parity::even));
		}
		else if (serial_port_info.parity == "odd")
		{
			serial_port_.set_option(serial_port::parity(serial_port::parity::odd));
		}
		else
		{
			serial_port_.set_option(serial_port::parity(serial_port::parity::none));
		}

		if (serial_port_info.stop_bits == "2 bits")
		{
			serial_port_.set_option(serial_port::stop_bits(serial_port::stop_bits::two));
		}
		else if (serial_port_info.stop_bits == "1.5 bits")
		{
			serial_port_.set_option(serial_port::stop_bits(serial_port::stop_bits::onepointfive));
		}
		else
		{
			serial_port_.set_option(serial_port::stop_bits(serial_port::stop_bits::one));
		}
		
		if (serial_port_info.stop_bits == "7 bits")
		{
			serial_port_.set_option(serial_port::character_size(7));
		}
		else
		{
			serial_port_.set_option(serial_port::character_size(8));
		}
    }

    // start a modbus session
    int Mbs_Serial_IO::start_session(cms::MBS_STATION_TYPE mbs_type)
    {
    	if (mbs_type == MBS_SERIAL_SLAVE)
    	{
    		start_accept_master_request();
    		return 0;
    	}
		else if (mbs_type == MBS_SERIAL_MASTER)
		{
			start_request_slave();
			return 0;
		}
    	else
    	{
    		std::cout << "Can't start as a slave station, wrong station type!\n";
    		return -1;
    	}
    }

    // start accept modbus master request
    void Mbs_Serial_IO::start_accept_master_request()
    {
    	slave_session_ = boost::shared_ptr<Serial_Slave_Session>(new Serial_Slave_Session(*this, mbs_station_ref_, mbs_serial_request_handler_, serial_port_));
    	slave_session_->start();
    }

	// start request modbus slave
	void Mbs_Serial_IO::start_request_slave()
	{
		master_session_ = boost::shared_ptr<Serial_Master_Session>(new Serial_Master_Session(*this, mbs_station_ref_, serial_port_));
    	master_session_->start();
	}

    void Mbs_Serial_IO::stop()
    {
        stopped_ = true;
        boost::system::error_code ignored_ec;
        if (master_session_.use_count()>=1)
        {
            master_session_->stop();
        }
    }

	boost::asio::io_service& Mbs_Serial_IO::get_io_service_ref() const
	{
		return this->io_service_;
	}

    Mbs_Serial_IO::~Mbs_Serial_IO()
    {
    	serial_port_.close();
  
#ifdef USE_NS_DEBUG
        ++DTOR_CNT;
       std::cerr << "Mbs_Serial_IO DTOR count: " << DTOR_CNT << std::endl;
#endif   
    }

} // namespace cms

/*
 * Mbs_Serial_IO.cpp
 *
 *  Created on: 2011-10-18
 *      Author: Administrator
 */

