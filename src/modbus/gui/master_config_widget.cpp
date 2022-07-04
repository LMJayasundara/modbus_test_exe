/* Master_Config_Widget Body.
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
#include <fstream>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>

#include "modbus/gui/master_config_widget.h"
#include "modbus/config/master_config_model.h"
#include "modbus/config/master_config_view.h"
#include "modbus/config/mbs_def.h"
#include "config/cms_config_db.h"
#include "log/log_sender.h"
#include "modbus/config/config_item_delegate.h"

using namespace std;
using namespace cms;


namespace cms
{
	Master_Config_Widget::Master_Config_Widget(cms::MBS_STATION_TYPE mbs_type, QWidget *parent)
		: mbs_type_(mbs_type), QWidget(parent)
	{
		config_db_ = boost::shared_ptr<CMS_Config_DB>(new CMS_Config_DB());
		master_configuration_.reserve(MAX_MASTER_CONFIG_RECORD);

		this->setup_ui();

		this->load_recent_master_config();

		this->init();
	}

	void Master_Config_Widget::init()
	{
		master_config_model_ = boost::shared_ptr<Master_Config_Model>(new Master_Config_Model(this->master_configuration_, this));
		master_config_view_ = new Master_Config_View(this);
		connect(master_config_model_.get(), SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), master_config_view_, SLOT(dataChanged(const QModelIndex &, const QModelIndex &)));

		master_config_view_->setModel(master_config_model_.get());
		master_config_view_->update_actions();
		main_layout_->addWidget(master_config_view_);

		connect(master_config_load_btn_, SIGNAL(clicked()), this, SLOT(load_master_config_file()));
		connect(master_config_save_btn_, SIGNAL(clicked()), this, SLOT(save_master_config_file()));

		slave_addr_delegate_ = new Spin_Box_Delegate(0, 255, 1, master_config_view_);
		combo_box_delegate_ = new Combo_Box_Delegate(master_config_view_);
		start_addr_delegate_ = new Spin_Box_Delegate(0, 65535, 1, master_config_view_);
		pt_count_delegate_ = new Spin_Box_Delegate(0, 65535, 1, master_config_view_);
		retries_delegate_ = new Spin_Box_Delegate(0, 255, 1, master_config_view_);
		timeout_delegate_ = new Spin_Box_Delegate(0, 3600000, 1000, master_config_view_);
		master_config_view_->setItemDelegateForColumn(Master_Config_Model::SLAVE_ADDR_INDEX, slave_addr_delegate_);
		master_config_view_->setItemDelegateForColumn(Master_Config_Model::FC_INDEX, combo_box_delegate_);
		master_config_view_->setItemDelegateForColumn(Master_Config_Model::START_ADDR_INDEX, start_addr_delegate_);
		master_config_view_->setItemDelegateForColumn(Master_Config_Model::PT_CNT_INDEX, pt_count_delegate_);
		master_config_view_->setItemDelegateForColumn(Master_Config_Model::RETRIES_INDEX, retries_delegate_);
		master_config_view_->setItemDelegateForColumn(Master_Config_Model::TIMEOUT_INDEX, timeout_delegate_);

		connect(goto_row_sb_, SIGNAL(valueChanged(int)), this, SLOT(goto_row(int)));
	}

	void Master_Config_Widget::set_mbs_type(cms::MBS_STATION_TYPE mbs_type)
	{
		this->mbs_type_ = mbs_type;
	}

	void Master_Config_Widget::setup_ui()
	{
		main_layout_ = new QVBoxLayout(this);
		this->setLayout(main_layout_);
		master_config_load_layout_ = new QHBoxLayout();
		main_layout_->addLayout(master_config_load_layout_);
		goto_row_layout_ = new QHBoxLayout();
		main_layout_->addLayout(goto_row_layout_);

		master_config_file_lbl_ = new QLabel(tr("Master Configuration File:"));
		master_config_file_txt_ = new QLineEdit();
		master_config_load_btn_ = new QPushButton(tr("Load"));
		master_config_save_btn_ = new QPushButton(tr("Save"));
		master_config_load_layout_->addWidget(master_config_file_lbl_);
		master_config_load_layout_->addWidget(master_config_file_txt_);
		master_config_load_layout_->addWidget(master_config_load_btn_);
		master_config_load_layout_->addWidget(master_config_save_btn_);

		goto_row_lbl_ = new QLabel(tr("Goto Row: "));
		goto_row_sb_ = new QSpinBox();
		goto_row_sb_->setMinimum(0);
		goto_row_sb_->setMaximum(MBS_LARGEST_MASTER_GROUP);
		connect(goto_row_sb_, SIGNAL(valueChanged(int)), this, SLOT(goto_row(int)));
		goto_row_layout_->addWidget(goto_row_lbl_);
		goto_row_layout_->addWidget(goto_row_sb_);

		QString recent_config_file;
		if (this->mbs_type_ == MBS_SERIAL_MASTER)
		{
			recent_config_file = QString("%1").arg(config_db_->get_app_setting("recent_serial_master_config_file").c_str());
		}
		else if (this->mbs_type_ == MBS_TCP_MASTER)
		{
			recent_config_file = QString("%1").arg(config_db_->get_app_setting("recent_tcp_master_config_file").c_str());
		}
		this->master_config_file_txt_->setText(recent_config_file);
	}

	void Master_Config_Widget::goto_row(int row_number)
	{
		this->master_config_view_->goto_row(row_number);
	}

	void Master_Config_Widget::load_master_config_file()
	{
		QFileDialog file_dialog;

		QString recent_config_file;
		if (this->mbs_type_ == MBS_SERIAL_MASTER)
		{
			recent_config_file = QString("%1").arg(config_db_->get_app_setting("recent_serial_master_config_file").c_str());
		}
		else if (this->mbs_type_ == MBS_TCP_MASTER)
		{
			recent_config_file = QString("%1").arg(config_db_->get_app_setting("recent_tcp_master_config_file").c_str());
		}

		QFileInfo file_info(recent_config_file);
		file_dialog.setDirectory(file_info.dir().absolutePath());

		QString full_master_config_file_name=file_dialog.getOpenFileName(this);

		if (!full_master_config_file_name.isEmpty())
		{
			this->master_config_file_txt_->setText(full_master_config_file_name);

			if (this->mbs_type_ == MBS_SERIAL_MASTER)
			{
				config_db_->set_app_setting("recent_serial_master_config_file", full_master_config_file_name.toLocal8Bit().data());
			}
			else if (this->mbs_type_ == MBS_TCP_MASTER)
			{
				config_db_->set_app_setting("recent_tcp_master_config_file", full_master_config_file_name.toLocal8Bit().data());
			}
			this->load_master_config_file(full_master_config_file_name.toLocal8Bit().data());
		}
	}

	void Master_Config_Widget::load_master_config_file(const std::string& config_file_name)
	{
		this->master_configuration_.clear();
		ifstream config_file;
		config_file.open(config_file_name.c_str());
		unsigned int record_id = 0;
		if (config_file.is_open())
		{
			string data;
			typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
			boost::char_separator<char> sep(",");
			const unsigned int data_colum_cnt = 6;
			unsigned int data_line[data_colum_cnt];
			unsigned int index = 0;
			while (config_file >> data)
			{
				++index;
				if (index == 1)
					continue; // skip first line

				tokenizer tokens(data, sep);
				unsigned int i=0;
				for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
				{
					if (i < data_colum_cnt)
					{
						try
						{
							data_line[i] = boost::lexical_cast<unsigned int>(tok_iter.current_token());
							++i;
						}
						catch(boost::bad_lexical_cast &)
						{
							Log_Sender::send_msg(QString("bad lexical cast"));
						}
					}
				}
				Master_Config_Record record;
				record.slave_addr = data_line[0];
				record.function_code = data_line[1];
				record.start_addr = data_line[2];
				record.point_count = data_line[3];
				record.retries = data_line[4];
				record.timeout = data_line[5];
				this->master_configuration_.push_back(record);
				++record_id;
			} 
		}
		if (this->master_config_model_.use_count() > 0)
			this->master_config_model_->update_row_count();
		config_file.close();
	}

	void Master_Config_Widget::save_master_config_file()
	{
		QFileDialog file_dialog;

		QString recent_config_file;
		if (this->mbs_type_ == MBS_SERIAL_MASTER)
		{
			recent_config_file = QString("%1").arg(config_db_->get_app_setting("recent_serial_master_config_file").c_str());
		}
		else if (this->mbs_type_ == MBS_TCP_MASTER)
		{
			recent_config_file = QString("%1").arg(config_db_->get_app_setting("recent_tcp_master_config_file").c_str());
		}

		QFileInfo file_info(recent_config_file);
		file_dialog.setDirectory(file_info.dir().absolutePath());

		QString full_master_config_file_name=file_dialog.getSaveFileName(this, tr("Save Master Configuration File"), recent_config_file);
		
		if (!full_master_config_file_name.isEmpty())
		{
			this->master_config_file_txt_->setText(full_master_config_file_name);

			if (this->mbs_type_ == MBS_SERIAL_MASTER)
			{
				config_db_->set_app_setting("recent_serial_master_config_file", full_master_config_file_name.toLocal8Bit().data());
			}
			else if (this->mbs_type_ == MBS_TCP_MASTER)
			{
				config_db_->set_app_setting("recent_tcp_master_config_file", full_master_config_file_name.toLocal8Bit().data());
			}

			this->save_master_config_file(full_master_config_file_name.toLocal8Bit().data());
			Log_Sender::send_msg(QString("Save master config file: %1").arg(full_master_config_file_name));
		}
	}

	void Master_Config_Widget::save_master_config_file(const std::string& config_file_name)
	{
		ofstream config_file;
        config_file.open(config_file_name.c_str());
		config_file << "slave_addr,function_code,pt_start_addr,pt_count,retries,timeout\n";
		for (std::vector<Master_Config_Record>::const_iterator record_idx=this->master_configuration_.begin();
			record_idx!=this->master_configuration_.end();++record_idx)
		{
			config_file << (unsigned int)record_idx->slave_addr << "," <<(unsigned int) record_idx->function_code << ","
				<< (unsigned int)record_idx->start_addr << "," << (unsigned int)record_idx->point_count << ","
				<< (unsigned int)record_idx->retries << "," << (unsigned int)record_idx->timeout << "\n";
		}
		config_file.close();
	}

	void Master_Config_Widget::load_recent_master_config()
	{
		if (this->mbs_type_ == MBS_SERIAL_MASTER)
			this->load_master_config_file(config_db_->get_app_setting("recent_serial_master_config_file"));
		else if (this->mbs_type_ == MBS_TCP_MASTER)
			this->load_master_config_file(config_db_->get_app_setting("recent_tcp_master_config_file"));
	}

	void Master_Config_Widget::closeEvent(QCloseEvent* /*event*/)
	{
	}

	Master_Config_Widget::~Master_Config_Widget()
	{
	}
}
