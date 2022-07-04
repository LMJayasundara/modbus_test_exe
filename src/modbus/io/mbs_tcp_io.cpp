/* Mbs_Tcp_IO body.
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

#include "modbus/io/mbs_tcp_io.h"
#include "modbus/io/tcp_slave_session.h"
#include "modbus/io/tcp_master_session.h"
#include "modbus/address/addr_space.h"
#include "modbus/station/modbus_station.h"
#include "log/log_sender.h"
#include "cms_config.h"

#include <iostream>
#include <fstream>
#include <exception>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio/ip/tcp.hpp>

#ifdef USE_NS_DEBUG
#include <iostream>
#endif

using namespace std;
using namespace boost;
using boost::asio::ip::tcp;

namespace cms
{
    int Mbs_Tcp_IO::DTOR_CNT = 0;

    Mbs_Tcp_IO::Mbs_Tcp_IO(boost::asio::io_service& io_service, Modbus_Station &mbs_station, const TCP_Port_Info& tcp_port_info,
        Mbs_Tcp_IO::Station_Type stn_type, Mbs_Tcp_IO::Message_Type msg_type)
        : stopped_(false), io_service_(io_service), mbs_station_ref_(mbs_station), tcp_port_info_(tcp_port_info),
        station_type(stn_type), message_type(msg_type),
        connect_deadline_timer_(io_service)
    {
    }

    // start a modbus session
    int Mbs_Tcp_IO::start_session(cms::MBS_STATION_TYPE mbs_type)
    {
        if (mbs_type == MBS_TCP_SLAVE)
        {
            start_accept_master_connect();
            return 0;
        }
        else if (mbs_type == MBS_TCP_MASTER)
        {
            tcp::resolver r(io_service_);
            slave_endpoint_iter_ = r.resolve(tcp::resolver::query(tcp_port_info_.ip_address, tcp_port_info_.port_str));
            connect_slave();
            //start_request_slave();
            return 0;
        }
        else
        {
            std::cout << "Can't start as a slave station, wrong station type!\n";
            return -1;
        }
    }

    // start accept modbus master connect
    void Mbs_Tcp_IO::start_accept_master_connect()
    {
        if (slave_acceptor_.use_count() < 1)
        {
            slave_acceptor_ =  boost::shared_ptr<boost::asio::ip::tcp::acceptor>(new boost::asio::ip::tcp::acceptor(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), tcp_port_info_.port)));
        }
        Tcp_Slave_Session* new_session = new Tcp_Slave_Session(io_service_, *this,
            mbs_station_ref_,mbs_tcp_request_handler_);
        slave_acceptor_->async_accept(new_session->socket(),
            boost::bind(&Mbs_Tcp_IO::handle_accept, this, new_session,boost::asio::placeholders::error));
    }

    // connect to a modbus slave
    void Mbs_Tcp_IO::connect_slave()
    {
        if (slave_endpoint_iter_ != tcp::resolver::iterator())
        {
            std::cout << "Trying " << slave_endpoint_iter_->endpoint() << "...\n";

            // Set a deadline for the connect operation.
            connect_deadline_timer_.expires_from_now(boost::posix_time::seconds(60));

            master_session_ = boost::shared_ptr<Tcp_Master_Session>(new Tcp_Master_Session(io_service_, *this, mbs_station_ref_));
            // Start the asynchronous connect operation.
            master_session_->socket().async_connect(slave_endpoint_iter_->endpoint(), 
                boost::bind(&Mbs_Tcp_IO::handle_slave_connect, this, _1));
        }
        else
        {
            // There are no more endpoints to try. Shut down the client.
            stop();
        }
    }

    void Mbs_Tcp_IO::handle_slave_connect(const boost::system::error_code& ec)
     {
         if (stopped_)
             return;

         // The async_connect() function automatically opens the socket at the start
         // of the asynchronous operation. If the socket is closed at this time then
         // the timeout handler must have run first.
         if (!master_session_->socket().is_open())
         {
             std::cout << "Connect timed out\n";

             // Try the next available endpoint.
             ++slave_endpoint_iter_;
             connect_slave();
         }
         // Check if the connect operation failed before the deadline expired.
         else if (ec)
         {
             std::cout << "Connect error: " << ec.message() << "\n";

             // We need to close the socket used in the previous connection attempt
             // before starting a new one.
             master_session_->socket().close();

             // Try the next available endpoint.
             ++slave_endpoint_iter_;
             connect_slave();
         }
         // Otherwise we have successfully established a connection.
         else
         {
             std::cout << "Connected to " << slave_endpoint_iter_->endpoint() << "\n";
             if (master_session_->socket().is_open())
                 cout << "socket is open\n";
             else
                 cout << "socket is not open\n";
             start_request_slave();
             // Start the input actor.
             //cout << "start_read()";

             // Start the heartbeat actor.
             //start_write();
         }
     }

    // start request modbus slave
    void Mbs_Tcp_IO::start_request_slave()
    {
        try
        {
            master_session_->start();
        }
        catch (const std::exception& exp)
        {
            Log_Sender::send_msg(exp.what());       	
        }       
    }

    boost::asio::io_service& Mbs_Tcp_IO::get_io_service_ref() const
    {
        return this->io_service_;
    }

    // handle modbus master tcp connection
    void Mbs_Tcp_IO::handle_accept(Tcp_Slave_Session* new_session,const boost::system::error_code& error)
    {
        if (!error)
        {
            new_session->start();
        }
        else
        {
            delete new_session;
        }

        // start another io to accept master connect
        start_accept_master_connect();
    }

     void Mbs_Tcp_IO::stop()
     {
         stopped_ = true;
         boost::system::error_code ignored_ec;
         if (master_session_.use_count()>=1)
         {
             master_session_->stop();
         }
         connect_deadline_timer_.cancel();
     }

     Mbs_Tcp_IO::~Mbs_Tcp_IO()
     {
#ifdef USE_NS_DEBUG
         ++DTOR_CNT;
         std::cerr << "Mbs_Tcp_IO DTOR count: " << DTOR_CNT << std::endl;
#endif
     }

} // namespace cms

