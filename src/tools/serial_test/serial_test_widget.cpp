/* Serial_Test_Widget Body.
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

#include <string>
#include <exception>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <QMessageBox>
#include <QTimer>
#include <QtTest/qtest.h>

#include "tools/serial_test/serial_test_widget.h"
#include "modbus/config/serial_port_info.h"
#include "modbus/config/mbs_def.h"

using namespace std;
using namespace boost;
using namespace boost::asio;

namespace cms
{
	//const int Serial_Test_Widget::MAX_SEND_BUFFER_SIZE;
	std::string BYTE_TO_STR_TABLE[256] = {"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F",
		"10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F",
		"20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F",
		"30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F",
		"40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F",
		"50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F",
		"60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F",
		"70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F",
		"80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
		"90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F",
		"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF",
		"B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF",
		"C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF",
		"D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF",
		"E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF",
		"F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF"};

	void convert_str_to_hex(const std::string &str, char *hex_buffer, unsigned int max_send_buffer_size)
	{
		unsigned int high_char, low_char;
		unsigned int end_count = str.size()-1;
		if (end_count >= max_send_buffer_size)
		{
			end_count = max_send_buffer_size - 1;
		}
		for (unsigned int i=0; i<end_count; i += 2)
		{
			if (str[i] >= '0' && str[i] <= '9')
			{
				high_char = (str[i] - '0') * 16;
			}
			else if (str[i] >= 'a' && str[i] <= 'f')
			{
				high_char = (str[i] - 'a' + 10) * 16;
			}
			else if (str[i] >= 'A' && str[i] <= 'F')
			{
				high_char = (str[i] - 'A' + 10) * 16;
			}
			else
				return;

			if (str[i+1] >= '0' && str[i+1] <= '9')
			{
				low_char = (str[i+1] - '0');
			}
			else if (str[i+1] >= 'a' && str[i+1] <= 'f')
			{
				low_char = (str[i+1] - 'a' + 10);
			}
			else if (str[i+1] >= 'A' && str[i+1] <= 'F')
			{
				low_char = (str[i+1] - 'A' + 10);
			}
			else
				return;
			unsigned int val = high_char + low_char;

			hex_buffer[i/2] = val;
		}
	}

	Serial_Test_Widget::Serial_Test_Widget(QWidget *parent)
		: QWidget(parent)
	{
		ui.setupUi(this);
		this->setup_ui();
		connect(ui.send_btn_, SIGNAL(clicked()), this, SLOT(process_send_request()));
		connect(ui.send_repeatedly_cb_, SIGNAL(stateChanged(int)), this, SLOT(change_send_repeatedly()));
        connect(ui.recv_btn_, SIGNAL(clicked()), this, SLOT(process_recv_request()));
		connect(ui.stop_recv_btn_, SIGNAL(clicked()), this, SLOT(stop_recv_data()));
		send_repeatedly_ = false;
		send_interval_ = 100;
		send_data_count_ = 0;
		is_send_port_working_ = false;
		is_recv_port_working_ = false;

		stop_recv_ = true;

		update_ui_timer_ = boost::shared_ptr<QTimer>(new QTimer(this));    
		update_ui_timer_->start(1000);
		connect(update_ui_timer_.get(), SIGNAL(timeout()), this, SLOT(update_ui()));
	}

	void Serial_Test_Widget::setup_ui()
	{
	}

	void Serial_Test_Widget::update_ui()
	{
		if (is_send_port_working_ == true)
		{
			ui.send_serial_port_cb_->setEnabled(false);
		}
		else
		{
			ui.send_serial_port_cb_->setEnabled(true);
		}

		if (is_recv_port_working_ == true)
		{
			ui.recv_serial_port_cb_->setEnabled(false);
		}
		else
		{
			ui.recv_serial_port_cb_->setEnabled(true);
		}
	}

	void Serial_Test_Widget::change_send_repeatedly()
	{
		if (ui.send_repeatedly_cb_->isChecked())
		{
			send_repeatedly_ = true;
		}
		else
		{
			send_repeatedly_ = false;
		}
	}

	void Serial_Test_Widget::process_send_request()
	{
		if (config_send_port())
		{
			send_data();
		}
	}

	bool Serial_Test_Widget::config_send_port()
	{
		Serial_Port_Info serial_port_info;
		serial_port_info.port_name = ui.send_serial_port_cb_->currentText().toLocal8Bit().data();
		serial_port_info.baud_rate = ui.send_baud_cb_->currentText().toLocal8Bit().toUInt();
		if (ui.send_data_bits_cb_->currentText() == "8 bits")
		{
			serial_port_info.data_bits = 8;
		}
		else if (ui.send_data_bits_cb_->currentText() == "7 bits")
		{
			serial_port_info.data_bits = 7;
		}
		else
		{
			serial_port_info.data_bits = 8;
		}

		if (ui.send_parity_cb_->currentText() == "Odd")
		{
			serial_port_info.parity = "odd";
		}
		else if (ui.send_parity_cb_->currentText() == "Even")
		{
			serial_port_info.parity = "even";
		}
		else
		{
			serial_port_info.parity = "none";
		}

		if (ui.send_stop_bits_cb_->currentText() == "1.5 bits")
		{
			serial_port_info.stop_bits = "1.5 bits";
		}
		else if (ui.send_stop_bits_cb_->currentText() == "2 bits")
		{
			serial_port_info.stop_bits = "2 bits";
		}
		else
		{
			serial_port_info.stop_bits = "1 bits";
		}

		if (ui.send_flow_ctrl_cb_->currentText() == "hardware")
		{
			serial_port_info.flow_ctrl = "hardware";
		}
		else if (ui.send_flow_ctrl_cb_->currentText() == "software")
		{
			serial_port_info.flow_ctrl = "software";
		}
		else
		{
			serial_port_info.flow_ctrl = "none";
		}

		try  
		{  

			send_serial_port_ = boost::shared_ptr<boost::asio::serial_port>(new boost::asio::serial_port(send_io_, serial_port_info.port_name));

			send_serial_port_->set_option(serial_port::baud_rate(serial_port_info.baud_rate));
			if (serial_port_info.flow_ctrl == "hardware")
			{
				send_serial_port_->set_option(serial_port::flow_control(serial_port::flow_control::hardware));
			}
			else if (serial_port_info.flow_ctrl == "software")
			{
				send_serial_port_->set_option(serial_port::flow_control(serial_port::flow_control::software));
			}
			else
			{
				send_serial_port_->set_option(serial_port::flow_control(serial_port::flow_control::none));
			}

			if (serial_port_info.parity == "even")
			{
				send_serial_port_->set_option(serial_port::parity(serial_port::parity::even));
			}
			else if (serial_port_info.parity == "odd")
			{
				send_serial_port_->set_option(serial_port::parity(serial_port::parity::odd));
			}
			else
			{
				send_serial_port_->set_option(serial_port::parity(serial_port::parity::none));
			}

			if (serial_port_info.stop_bits == "2 bits")
			{
				send_serial_port_->set_option(serial_port::stop_bits(serial_port::stop_bits::two));
			}
			else if (serial_port_info.stop_bits == "1.5 bits")
			{
				send_serial_port_->set_option(serial_port::stop_bits(serial_port::stop_bits::onepointfive));
			}
			else
			{
				send_serial_port_->set_option(serial_port::stop_bits(serial_port::stop_bits::one));
			}

			if (serial_port_info.stop_bits == "7 bits")
			{
				send_serial_port_->set_option(serial_port::character_size(7));
			}
			else
			{
				send_serial_port_->set_option(serial_port::character_size(8));
			}

			send_interval_ = ui.send_interval_cb_->currentText().toUInt();
			string send_data_str = ui.send_data_txt_->toPlainText().toLocal8Bit().data();
			const char *send_data_ptr = send_data_str.c_str();
			send_data_count_ = send_data_str.size();
			if (send_data_count_ >= MAX_SEND_BUFFER_SIZE)
			{
				send_data_count_ = MAX_SEND_BUFFER_SIZE;
				QMessageBox::about(this, "Warning", "Only 255 bytes can be sent!");
			}
			memcpy(send_buffer_, send_data_ptr, send_data_count_);
			if (ui.send_data_type_cb_->currentText() == "HEX")
			{
				convert_str_to_hex(send_data_str, send_buffer_, MAX_SEND_BUFFER_SIZE);
				send_data_count_ = send_data_count_/2;;
			}
			
			return true;	
		}  
		catch (const std::exception& err)  
		{  
			QMessageBox::about(this, "Send Data Error", err.what());
			is_send_port_working_ = false;
			return false;
		}  
	}

	void Serial_Test_Widget::send_data()
	{
		
		send_io_thread_ = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&boost::asio::io_service::run, &send_io_)));
		
		boost::asio::async_write(*(send_serial_port_.get()), boost::asio::buffer(send_buffer_, send_data_count_), boost::bind(&Serial_Test_Widget::handle_send_data, this, boost::asio::placeholders::error));
		
		//send_io_thread_->join();
		is_send_port_working_ = true;
		ui.send_serial_port_cb_->setEnabled(false);
		cout << "send action\n";
		cout << "use count: " << send_serial_port_.use_count() << "\n";
		
	}

	void Serial_Test_Widget::handle_send_data(const boost::system::error_code& /*error*/)
	{
		cout << "handle send\n";
		if (send_repeatedly_ == false)
		{
			is_send_port_working_ = false;
			this->send_serial_port_->cancel();
			this->send_serial_port_->close();
			
			return;
		}
		else
		{
			QTest::qSleep(send_interval_);
			boost::asio::async_write(*(send_serial_port_.get()), boost::asio::buffer(send_buffer_, send_data_count_), boost::bind(&Serial_Test_Widget::handle_send_data, this, boost::asio::placeholders::error));
		}
	}

	void Serial_Test_Widget::process_recv_request()
	{
		if (config_recv_port())
		{
			recv_data();
		}
	}

    bool Serial_Test_Widget::config_recv_port()
    {
        Serial_Port_Info serial_port_info;
		serial_port_info.port_name = ui.recv_serial_port_cb_->currentText().toLocal8Bit().data();
		serial_port_info.baud_rate = ui.recv_baud_cb_->currentText().toLocal8Bit().toUInt();
		if (ui.recv_data_bits_cb_->currentText() == "8 bits")
		{
			serial_port_info.data_bits = 8;
		}
		else if (ui.recv_data_bits_cb_->currentText() == "7 bits")
		{
			serial_port_info.data_bits = 7;
		}
		else
		{
			serial_port_info.data_bits = 8;
		}

		if (ui.recv_parity_cb_->currentText() == "Odd")
		{
			serial_port_info.parity = "odd";
		}
		else if (ui.recv_parity_cb_->currentText() == "Even")
		{
			serial_port_info.parity = "even";
		}
		else
		{
			serial_port_info.parity = "none";
		}

		if (ui.recv_stop_bits_cb_->currentText() == "1.5 bits")
		{
			serial_port_info.stop_bits = "1.5 bits";
		}
		else if (ui.recv_stop_bits_cb_->currentText() == "2 bits")
		{
			serial_port_info.stop_bits = "2 bits";
		}
		else
		{
			serial_port_info.stop_bits = "1 bits";
		}

		if (ui.recv_flow_ctrl_cb_->currentText() == "hardware")
		{
			serial_port_info.flow_ctrl = "hardware";
		}
		else if (ui.recv_flow_ctrl_cb_->currentText() == "software")
		{
			serial_port_info.flow_ctrl = "software";
		}
		else
		{
			serial_port_info.flow_ctrl = "none";
		}

		try  
		{  

			recv_serial_port_ = boost::shared_ptr<boost::asio::serial_port>(new boost::asio::serial_port(recv_io_, serial_port_info.port_name));

			recv_serial_port_->set_option(serial_port::baud_rate(serial_port_info.baud_rate));
			if (serial_port_info.flow_ctrl == "hardware")
			{
				recv_serial_port_->set_option(serial_port::flow_control(serial_port::flow_control::hardware));
			}
			else if (serial_port_info.flow_ctrl == "software")
			{
				recv_serial_port_->set_option(serial_port::flow_control(serial_port::flow_control::software));
			}
			else
			{
				recv_serial_port_->set_option(serial_port::flow_control(serial_port::flow_control::none));
			}

			if (serial_port_info.parity == "even")
			{
				recv_serial_port_->set_option(serial_port::parity(serial_port::parity::even));
			}
			else if (serial_port_info.parity == "odd")
			{
				recv_serial_port_->set_option(serial_port::parity(serial_port::parity::odd));
			}
			else
			{
				recv_serial_port_->set_option(serial_port::parity(serial_port::parity::none));
			}

			if (serial_port_info.stop_bits == "2 bits")
			{
				recv_serial_port_->set_option(serial_port::stop_bits(serial_port::stop_bits::two));
			}
			else if (serial_port_info.stop_bits == "1.5 bits")
			{
				recv_serial_port_->set_option(serial_port::stop_bits(serial_port::stop_bits::onepointfive));
			}
			else
			{
				recv_serial_port_->set_option(serial_port::stop_bits(serial_port::stop_bits::one));
			}

			if (serial_port_info.stop_bits == "7 bits")
			{
				recv_serial_port_->set_option(serial_port::character_size(7));
			}
			else
			{
				recv_serial_port_->set_option(serial_port::character_size(8));
			}
			
            return true;
		}  
		catch (const std::exception& err)  
		{  
			QMessageBox::about(this, "Receive Data Error", err.what());
			is_recv_port_working_ = false;
			return false;
		}  
        
    }

	void Serial_Test_Widget::recv_data()
	{
		recv_io_thread_ = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&boost::asio::io_service::run, &recv_io_)));
			/*async_read(*(recv_serial_port_.get()), boost::asio::buffer(recv_buffer_), 
                boost::bind(&Serial_Test_Widget::handle_recv_data, this,boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));*/
			stop_recv_ = false;
            recv_serial_port_->async_read_some(boost::asio::buffer(recv_buffer_, MAX_RECV_BUFFER_SIZE), 
                boost::bind(&Serial_Test_Widget::handle_recv_data, this,boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
			
			is_recv_port_working_ = true;
			ui.recv_serial_port_cb_->setEnabled(false);

			ui.stop_recv_btn_->setEnabled(true);
			ui.recv_btn_->setEnabled(false);
	}

	void Serial_Test_Widget::stop_recv_data()
	{
		stop_recv_ = true;
		if (recv_serial_port_.use_count() >=1)
		{
			recv_serial_port_->cancel();
			recv_serial_port_->close();
			is_recv_port_working_ = false;
		}
		ui.stop_recv_btn_->setEnabled(false);
		ui.recv_btn_->setEnabled(true);
	}

    void Serial_Test_Widget::handle_recv_data(const boost::system::error_code& /*error*/, size_t bytes_transferred)
	{
		if (stop_recv_)
			return;
		string str;
		for (unsigned int i=0; i < bytes_transferred; ++i)
		{
			str.append(BYTE_TO_STR_TABLE[(unsigned char)(*(recv_buffer_ + i))]);
			str.append(" ");	
		}
        ui.recv_data_txt_->insertPlainText(QString("%1").arg(str.c_str()));
		recv_serial_port_->async_read_some(boost::asio::buffer(recv_buffer_, MAX_RECV_BUFFER_SIZE), 
			boost::bind(&Serial_Test_Widget::handle_recv_data, this,boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

	void Serial_Test_Widget::closeEvent(QCloseEvent* /*event*/)
	{
		send_repeatedly_ = false;
		stop_recv_data();
	}

	Serial_Test_Widget::~Serial_Test_Widget()
	{
		
	}
}
