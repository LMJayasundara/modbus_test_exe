/* Mbs_Widget Body.
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
along with TurbineTools; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include <string>

#include <boost/thread.hpp>

#include <QFile>
#include <QMessageBox>

#include "modbus/gui/mbs_widget.h"
#include "modbus/comm/mbs_comm_model.h"
#include "modbus/comm/mbs_comm_view.h"
#include "modbus/station/modbus_station.h"
#include "modbus/config/serial_port_info.h"
#include "modbus/config/tcp_port_info.h"
#include "log/log_sender.h"
#include "config/cms_config_db.h"
#include "modbus/analyzer/mbs_analyzer.h"
#include "modbus/gui/addr_space_widget.h"
#include "modbus/gui/master_config_widget.h"

using namespace std;
using namespace cms;


namespace cms
{
	Mbs_Widget::Mbs_Widget(boost::shared_ptr<Addr_Space> coil, boost::shared_ptr<Addr_Space> dis_input,
		boost::shared_ptr<Addr_Space> input_reg, boost::shared_ptr<Addr_Space> holding_reg,
		MBS_STATION_TYPE mbs_type, QWidget *parent)
		: coil_addr_space_(coil), dis_input_addr_space_(dis_input),
		input_reg_addr_space_(input_reg), holding_reg_addr_space_(holding_reg),
		mbs_type_(mbs_type), QWidget(parent)
	{
		ui.setupUi(this);
		this->setup_ui();

		connect(ui.start_btn_, SIGNAL(clicked()), this, SLOT(start_modbus_station()));
		connect(ui.start_btn_, SIGNAL(clicked()), this, SLOT(update_comm_config()));
		connect(ui.stop_btn_, SIGNAL(clicked()), this, SLOT(stop_modbus_station()));
		//connect(ui.load_btn_, SIGNAL(clicked()), this, SLOT(load_master_cfg_file()));

		config_db_ = boost::shared_ptr<CMS_Config_DB>(new CMS_Config_DB());
	}

	void Mbs_Widget::set_mbs_type(MBS_STATION_TYPE mbs_type)
	{
		this->mbs_type_ = mbs_type;
		string mbs_analyzer_name;
		if (mbs_type_ == MBS_SERIAL_MASTER)
		{
			mbs_analyzer_name = "MBS-SM";
			mbs_analyzer_ = boost::shared_ptr<Mbs_Analyzer>(new Mbs_Analyzer(mbs_analyzer_name));
			mbs_analyzer_->init();
			mbs_analyzer_->init_data();
		}
		else if (mbs_type_ == MBS_SERIAL_SLAVE)
		{
			mbs_analyzer_name = "MBS-SS";
			mbs_analyzer_ = boost::shared_ptr<Mbs_Analyzer>(new Mbs_Analyzer(mbs_analyzer_name));
			mbs_analyzer_->init();
			mbs_analyzer_->init_data();
		}
		else if (mbs_type_ == MBS_TCP_MASTER)
		{
			mbs_analyzer_name = "MBS-TM";
			mbs_analyzer_ = boost::shared_ptr<Mbs_Analyzer>(new Mbs_Analyzer(mbs_analyzer_name));
			mbs_analyzer_->init();
			mbs_analyzer_->init_data();
		}
		else if (mbs_type_ == MBS_TCP_SLAVE)
		{
			mbs_analyzer_name = "MBS-TS";
			mbs_analyzer_ = boost::shared_ptr<Mbs_Analyzer>(new Mbs_Analyzer(mbs_analyzer_name));
			mbs_analyzer_->init();
			mbs_analyzer_->init_data();
		}

		mbs_comm_model_ = boost::shared_ptr<Mbs_Comm_Model>(new Mbs_Comm_Model(mbs_analyzer_name));
		mbs_comm_view_ = new Mbs_Comm_View(ui.mbs_tab_widget_);


		ui.mbs_tab_widget_->insertTab(0, mbs_comm_view_, tr("Communication"));
		this->mbs_comm_view_->setModel(mbs_comm_model_.get());

		this->set_gui_type(mbs_type);
		this->load_recent_comm_config();

		if (mbs_type_ == MBS_SERIAL_MASTER || mbs_type_ == MBS_SERIAL_SLAVE)
		{
			ui.tcp_info_gb_->hide();
		}

		if (mbs_type_ == MBS_TCP_MASTER || mbs_type_ == MBS_TCP_SLAVE)
		{
			ui.serial_info_gb_->hide();ui.tcp_info_gb_->move(ui.serial_info_gb_->pos());
		}

		if (this->master_config_widget_)
			this->master_config_widget_->set_mbs_type(mbs_type);
	}

	void Mbs_Widget::load_recent_comm_config()
	{
		if (mbs_type_ == MBS_SERIAL_MASTER)
		{
			ui.serial_port_cb_->setEditText(config_db_->get_app_setting("serial_master_last_used_serial_port").c_str());
			ui.baud_cb_->setEditText(config_db_->get_app_setting("serial_master_last_used_baud").c_str());
			ui.data_bits_cb_->setEditText(config_db_->get_app_setting("serial_master_last_data_bits").c_str());
			ui.parity_cb_->setEditText(config_db_->get_app_setting("serial_master_last_parity").c_str());
			ui.stop_bits_cb_->setEditText(config_db_->get_app_setting("serial_master_last_stop_bits").c_str());
			ui.flow_ctrl_cb_->setEditText(config_db_->get_app_setting("serial_master_last_flow_ctrl").c_str());
		}
		else if (mbs_type_ == MBS_SERIAL_SLAVE)
		{
			ui.serial_port_cb_->setEditText(config_db_->get_app_setting("serial_slave_last_used_serial_port").c_str());
			ui.baud_cb_->setEditText(config_db_->get_app_setting("serial_slave_last_used_baud").c_str());
			ui.data_bits_cb_->setEditText(config_db_->get_app_setting("serial_slave_last_data_bits").c_str());
			ui.parity_cb_->setEditText(config_db_->get_app_setting("serial_slave_last_parity").c_str());
			ui.stop_bits_cb_->setEditText(config_db_->get_app_setting("serial_slave_last_stop_bits").c_str());
			ui.flow_ctrl_cb_->setEditText(config_db_->get_app_setting("serial_slave_last_flow_ctrl").c_str());
		}
		else if (mbs_type_ == MBS_TCP_MASTER)
		{
		}
		else if (mbs_type_ == MBS_TCP_SLAVE)
		{
		}
	}

	void Mbs_Widget::update_comm_config()
	{
		if (mbs_type_ == MBS_SERIAL_MASTER)
		{
			config_db_->set_app_setting("serial_master_last_used_serial_port", ui.serial_port_cb_->currentText().toLocal8Bit().data());
			config_db_->set_app_setting("serial_master_last_used_baud", ui.baud_cb_->currentText().toLocal8Bit().data());
			config_db_->set_app_setting("serial_master_last_data_bits", ui.data_bits_cb_->currentText().toLocal8Bit().data());
			config_db_->set_app_setting("serial_master_last_parity", ui.parity_cb_->currentText().toLocal8Bit().data());
			config_db_->set_app_setting("serial_master_last_stop_bits", ui.stop_bits_cb_->currentText().toLocal8Bit().data());
			config_db_->set_app_setting("serial_master_last_flow_ctrl", ui.flow_ctrl_cb_->currentText().toLocal8Bit().data());
		}
		else if (mbs_type_ == MBS_SERIAL_SLAVE)
		{
			config_db_->set_app_setting("serial_slave_last_used_serial_port", ui.serial_port_cb_->currentText().toLocal8Bit().data());
			config_db_->set_app_setting("serial_slave_last_used_baud", ui.baud_cb_->currentText().toLocal8Bit().data());
			config_db_->set_app_setting("serial_slave_last_data_bits", ui.data_bits_cb_->currentText().toLocal8Bit().data());
			config_db_->set_app_setting("serial_slave_last_parity", ui.parity_cb_->currentText().toLocal8Bit().data());
			config_db_->set_app_setting("serial_slave_last_stop_bits", ui.stop_bits_cb_->currentText().toLocal8Bit().data());
			config_db_->set_app_setting("serial_slave_last_flow_ctrl", ui.flow_ctrl_cb_->currentText().toLocal8Bit().data());
		}
		else if (mbs_type_ == MBS_TCP_MASTER)
		{
		}
		else if (mbs_type_ == MBS_TCP_SLAVE)
		{
		}
	}

	void Mbs_Widget::load_master_cfg_file()
	{
		;
	}

	void Mbs_Widget::set_gui_type(cms::MBS_STATION_TYPE mbs_type)
	{
		if (mbs_type == MBS_SERIAL_MASTER)
		{
			this->setWindowTitle(tr("Modbus Serial Master"));
			ui.conn_cb_->setCurrentIndex(0);
			ui.type_cb_->setCurrentIndex(0);
			ui.conn_cb_->setEnabled(false);
			ui.type_cb_->setEnabled(false);
			ui.mode_cb_->setEnabled(false);
			ui.serial_port_cb_->setEnabled(true);
			ui.baud_cb_->setEnabled(true);
			ui.data_bits_cb_->setEnabled(true);
			ui.parity_cb_->setEnabled(true);
			ui.stop_bits_cb_->setEnabled(true);
			ui.flow_ctrl_cb_->setEnabled(true);
			ui.slave_ip_cb_->setEnabled(false);
			ui.ip_port_cb_->setEnabled(false);
			ui.timeout_cb_->setEnabled(false);
		}
		else if (mbs_type == MBS_SERIAL_SLAVE)
		{
			this->setWindowTitle(tr("Modbus Serial Slave"));
			ui.conn_cb_->setCurrentIndex(0);
			ui.type_cb_->setCurrentIndex(1);
			ui.conn_cb_->setEnabled(false);
			ui.type_cb_->setEnabled(false);
			ui.mode_cb_->setEnabled(false);
			ui.serial_port_cb_->setEnabled(true);
			ui.baud_cb_->setEnabled(true);
			ui.data_bits_cb_->setEnabled(true);
			ui.parity_cb_->setEnabled(true);
			ui.stop_bits_cb_->setEnabled(true);
			ui.flow_ctrl_cb_->setEnabled(true);
			ui.slave_ip_cb_->setEnabled(false);
			ui.ip_port_cb_->setEnabled(false);
			ui.timeout_cb_->setEnabled(false);
		}
		else if (mbs_type == MBS_TCP_MASTER)
		{
			this->setWindowTitle(tr("Modbus TCP Master"));
			ui.conn_cb_->setCurrentIndex(1);
			ui.type_cb_->setCurrentIndex(0);
			ui.conn_cb_->setEnabled(false);
			ui.type_cb_->setEnabled(false);
			ui.mode_cb_->setEnabled(false);
			ui.serial_port_cb_->setEnabled(false);
			ui.baud_cb_->setEnabled(false);
			ui.data_bits_cb_->setEnabled(false);
			ui.parity_cb_->setEnabled(false);
			ui.stop_bits_cb_->setEnabled(false);
			ui.flow_ctrl_cb_->setEnabled(false);
			ui.slave_ip_cb_->setEnabled(true);
			ui.ip_port_cb_->setEnabled(true);
			ui.timeout_cb_->setEnabled(true);
		}
		else if (mbs_type == MBS_TCP_SLAVE)
		{
			this->setWindowTitle(tr("Modbus TCP Slave"));
			ui.conn_cb_->setCurrentIndex(1);
			ui.type_cb_->setCurrentIndex(1);
			ui.conn_cb_->setEnabled(false);
			ui.type_cb_->setEnabled(false);
			ui.baud_cb_->setEnabled(true);
			ui.mode_cb_->setEnabled(false);
			ui.serial_port_cb_->setEnabled(false);
			ui.baud_cb_->setEnabled(false);
			ui.data_bits_cb_->setEnabled(false);
			ui.parity_cb_->setEnabled(false);
			ui.stop_bits_cb_->setEnabled(false);
			ui.flow_ctrl_cb_->setEnabled(false);
			ui.slave_ip_cb_->setEnabled(false);
			ui.ip_port_cb_->setEnabled(true);
			ui.timeout_cb_->setEnabled(true);
		}
	}

	void Mbs_Widget::start_modbus_station()
	{
		mbs_station_ = boost::shared_ptr<cms::Modbus_Station>(new cms::Modbus_Station);
		mbs_station_->set_addr_space_ref(addr_space_widget_->get_coil_addr_space(), addr_space_widget_->get_dis_input_addr_space(),
			addr_space_widget_->get_input_reg_addr_space(), addr_space_widget_->get_holding_reg_addr_space());
		mbs_station_->init();

		bool io_created = false;
		if ((this->mbs_type_ == MBS_SERIAL_SLAVE) || (this->mbs_type_ == MBS_SERIAL_MASTER))
		{
			Serial_Port_Info serial_port_info;
			serial_port_info.port_name = ui.serial_port_cb_->currentText().toLocal8Bit().data();
			serial_port_info.baud_rate = ui.baud_cb_->currentText().toLocal8Bit().toUInt();
			if (ui.data_bits_cb_->currentText() == "8 bits")
			{
				serial_port_info.data_bits = 8;
			}
			else if (ui.data_bits_cb_->currentText() == "7 bits")
			{
				serial_port_info.data_bits = 7;
			}
			else
			{
				serial_port_info.data_bits = 8;
			}

			if (ui.parity_cb_->currentText() == "Odd")
			{
				serial_port_info.parity = "odd";
			}
			else if (ui.parity_cb_->currentText() == "Even")
			{
				serial_port_info.parity = "even";
			}
			else
			{
				serial_port_info.parity = "none";
			}

			if (ui.stop_bits_cb_->currentText() == "1.5 bits")
			{
				serial_port_info.stop_bits = "1.5 bits";
			}
			else if (ui.stop_bits_cb_->currentText() == "2 bits")
			{
				serial_port_info.stop_bits = "2 bits";
			}
			else
			{
				serial_port_info.stop_bits = "1 bits";
			}

			if (ui.flow_ctrl_cb_->currentText() == "hardware")
			{
                serial_port_info.flow_ctrl = "hardware";
			}
			else if (ui.flow_ctrl_cb_->currentText() == "software")
			{
				serial_port_info.flow_ctrl = "software";
			}
			else
			{
				serial_port_info.flow_ctrl = "none";
			}

			io_created = mbs_station_->create_serial_io(serial_port_info);
			if (!io_created)
			{
				QMessageBox::about(this, tr("Error!"), tr("Failed to create serial io"));
			}

		}
		else if ((this->mbs_type_ == MBS_TCP_SLAVE) || (this->mbs_type_ == MBS_TCP_MASTER))
		{
			TCP_Port_Info tcp_port_info;
			tcp_port_info.ip_address = ui.slave_ip_cb_->currentText().toLocal8Bit().data();
			tcp_port_info.port_str = ui.ip_port_cb_->currentText().toLocal8Bit().data();
			tcp_port_info.port = ui.ip_port_cb_->currentText().toInt();
			io_created = mbs_station_->create_tcp_io(tcp_port_info);
			if (!io_created)
			{
				QMessageBox::about(this, tr("Error!"), tr("Failed to create TCP io"));
			}
		}

		if (mbs_type_ == MBS_SERIAL_MASTER)
		{
			serial_master_cfg_file_name_ = config_db_->get_app_setting("recent_serial_master_config_file");
			if (QFile::exists(serial_master_cfg_file_name_.c_str()))
			{
				if (mbs_station_->load_master_diagram_group_config(serial_master_cfg_file_name_, MBS_SERIAL_MASTER) == false)
					return;
				else
				{
#ifdef USE_NS_DEBUG
					std::cerr << "Start Modbus Station, Load serial master group config file: " << serial_master_cfg_file_name_ << std::endl;
#endif
				}
			}
		}
		else if (mbs_type_ == MBS_TCP_MASTER)
		{
			tcp_master_cft_file_name_ = config_db_->get_app_setting("recent_tcp_master_config_file");
			if (mbs_station_->load_master_diagram_group_config(tcp_master_cft_file_name_, MBS_TCP_MASTER) == false)
				return;
			else
			{
#ifdef USE_NS_DEBUG
				std::cerr << "Start Modbus Station, Load tcp master group config file: " << tcp_master_cft_file_name_ << std::endl;
#endif
			}
		}
		if (io_created)
		{

			mbs_station_->set_station_type(mbs_type_);
			mbs_station_->run();
			//station_thread_ = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Modbus_Station::run, mbs_station_)));
			ui.start_btn_->setEnabled(false);
			ui.stop_btn_->setEnabled(true);
			//station_thread_->join();
		}
		//update_interval_ = 5000;
		//update_addr_space_timer_ = boost::shared_ptr<boost::asio::deadline_timer>(new boost::asio::deadline_timer
		//(io_service_, boost::posix_time::milliseconds(update_interval_)));

		//update_addr_space_timer_->async_wait(boost::bind(&Mbs_Widget::update_addr_space,
		//this, boost::asio::placeholders::error));

		//boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service_));
	}

	void Mbs_Widget::stop_modbus_station()
	{
		mbs_station_->stop();
		ui.start_btn_->setEnabled(true);
		ui.stop_btn_->setEnabled(false);
	}

	void Mbs_Widget::setup_ui()
	{
		addr_space_widget_ = new Addr_Space_Widget(coil_addr_space_, dis_input_addr_space_,
			input_reg_addr_space_, holding_reg_addr_space_, this);
		ui.mbs_tab_widget_->insertTab(0, addr_space_widget_, tr("Address Space"));
		if (this->mbs_type_ == MBS_SERIAL_MASTER || this->mbs_type_ == MBS_TCP_MASTER)
		{
			master_config_widget_ = new Master_Config_Widget(mbs_type_, this);
			ui.mbs_tab_widget_->insertTab(1, master_config_widget_, tr("Configuration"));
		}
		ui.mbs_tab_widget_->setCurrentWidget(addr_space_widget_);
	}

	void Mbs_Widget::closeEvent(QCloseEvent* /*event*/)
	{
		if (mbs_station_.use_count() >= 1)
			mbs_station_->stop();
	}

	Mbs_Widget::~Mbs_Widget()
	{
	}
}
