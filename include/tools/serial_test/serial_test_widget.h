/* Serial_Test_Widget Header.
Copyright (C) 2014
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
along with TurbineTools; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#ifndef CMS_SERIAL_TEST_WIDGET_H
#define CMS_SERIAL_TEST_WIDGET_H

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <QWidget>
#include <QPointer>

#include "ui/ui_serial_test_widget.h"

class QTimer;

namespace cms
{
    class Serial_Test_Widget : public QWidget
    {
    Q_OBJECT
    public:
        Serial_Test_Widget(QWidget *parent=0);
        ~Serial_Test_Widget();

    public slots:
		void process_send_request();

		bool config_send_port();

        void send_data();

        void change_send_repeatedly();

		void process_recv_request();

		bool config_recv_port();

        void recv_data();

		void stop_recv_data();

		void update_ui();

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        void setup_ui();

		void handle_send_data(const boost::system::error_code& error);

        void handle_recv_data(const boost::system::error_code& error, size_t bytes_transferred);

        Ui::Serial_Test_Widget ui;

		boost::asio::io_service send_io_;
		boost::shared_ptr<boost::asio::serial_port> send_serial_port_;
		bool is_send_port_working_;
		boost::shared_ptr<boost::thread> send_io_thread_;

		boost::asio::io_service recv_io_;
        boost::shared_ptr<boost::asio::serial_port> recv_serial_port_;
		bool is_recv_port_working_;
		boost::shared_ptr<boost::thread> recv_io_thread_;

		boost::shared_ptr<QTimer> update_ui_timer_;

		static const int MAX_SEND_BUFFER_SIZE = 255;
		char send_buffer_[MAX_SEND_BUFFER_SIZE]; 
		unsigned int send_data_count_;

        static const int MAX_RECV_BUFFER_SIZE = 2048;
		char recv_buffer_[MAX_RECV_BUFFER_SIZE];

        bool send_repeatedly_;
		bool stop_recv_;
		unsigned int send_interval_;
    };
}

#endif 
