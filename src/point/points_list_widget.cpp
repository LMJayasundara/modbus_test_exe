/* Points_List_Widget Body.
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
#include <boost/algorithm/string.hpp>
#include <boost/thread/thread.hpp>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>

#include "point/points_list_widget.h"
#include "point/points_list_model.h"
#include "point/points_list_view.h"
#include "point/point_item_delegate.h"
#include "config/cms_config_db.h"
#include "log/log_sender.h"

using namespace std;
using namespace cms;


namespace cms
{
	Points_List_Widget::Points_List_Widget(boost::shared_ptr<Addr_Space> coil, boost::shared_ptr<Addr_Space> dis_input,
		boost::shared_ptr<Addr_Space> input_reg, boost::shared_ptr<Addr_Space> holding_reg,
		QWidget *parent)
		: coil_addr_space_(coil), dis_input_addr_space_(dis_input),
		input_reg_addr_space_(input_reg), holding_reg_addr_space_(holding_reg),
		QWidget(parent)
	{
		config_db_ = boost::shared_ptr<CMS_Config_DB>(new CMS_Config_DB());

		this->setup_ui();

		this->load_recent_points_list();

		this->init();

		this->setWindowTitle("Point List Widget");
	}

	void Points_List_Widget::init()
	{
		points_list_model_ = boost::shared_ptr<Points_List_Model>(
			new Points_List_Model(coil_addr_space_, dis_input_addr_space_, input_reg_addr_space_, holding_reg_addr_space_, this->points_list_, this));
		points_list_view_ = new Points_List_View(this);
		connect(points_list_model_.get(), SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), points_list_view_, SLOT(dataChanged(const QModelIndex &, const QModelIndex &)));

		points_list_view_->setModel(points_list_model_.get());
		points_list_view_->update_actions();
		main_layout_->addWidget(points_list_view_);

		connect(points_list_load_btn_, SIGNAL(clicked()), this, SLOT(load_points_list_file()));
		connect(points_save_btn_, SIGNAL(clicked()), this, SLOT(save_points_list_file()));

		pt_value_delegate_ = new Pt_Value_Delegate(points_list_view_);
		points_list_view_->setItemDelegateForColumn(Points_List_Model::VALUE_INDEX, pt_value_delegate_);
		value_type_delegate_ = new Value_Type_Delegate(points_list_view_);
		points_list_view_->setItemDelegateForColumn(Points_List_Model::VALUE_TYPE_INDEX, value_type_delegate_);
		io_type_delegate_ = new IO_Type_Delegate(points_list_view_);
		points_list_view_->setItemDelegateForColumn(Points_List_Model::IO_TYPE_INDEX, io_type_delegate_);
		mbs_addr_type_delegate_ = new Mbs_Addr_Type_Delegate(points_list_view_);
		points_list_view_->setItemDelegateForColumn(Points_List_Model::MBS_ADDR_REF_INDEX, mbs_addr_type_delegate_);
		mbs_addr_delegate_ = new Pt_Spin_Box_Delegate(0, 65535, 1, points_list_view_);
		points_list_view_->setItemDelegateForColumn(Points_List_Model::MBS_ADDR_INDEX, mbs_addr_delegate_);
		
		connect(goto_row_sb_, SIGNAL(valueChanged(int)), this, SLOT(goto_row(int)));
	}

	void Points_List_Widget::setup_ui()
	{
		main_layout_ = new QVBoxLayout(this);
		this->setLayout(main_layout_);
		master_config_load_layout_ = new QHBoxLayout();
		main_layout_->addLayout(master_config_load_layout_);
		goto_row_layout_ = new QHBoxLayout();
		main_layout_->addLayout(goto_row_layout_);

		points_list_file_lbl_ = new QLabel(tr("Points List File:"));
		points_list_file_txt_ = new QLineEdit();
		points_list_load_btn_ = new QPushButton(tr("Load"));
		points_save_btn_ = new QPushButton(tr("Save"));
		master_config_load_layout_->addWidget(points_list_file_lbl_);
		master_config_load_layout_->addWidget(points_list_file_txt_);
		master_config_load_layout_->addWidget(points_list_load_btn_);
		master_config_load_layout_->addWidget(points_save_btn_);

		goto_row_lbl_ = new QLabel(tr("Goto Row: "));
		goto_row_sb_ = new QSpinBox();
		goto_row_sb_->setMinimum(0);
		goto_row_sb_->setMaximum(MAX_POINTS_RECORD);
		connect(goto_row_sb_, SIGNAL(valueChanged(int)), this, SLOT(goto_row(int)));
		goto_row_layout_->addWidget(goto_row_lbl_);
		goto_row_layout_->addWidget(goto_row_sb_);
	}

	void Points_List_Widget::goto_row(int row_number)
	{
		this->points_list_view_->goto_row(row_number);
	}

	void Points_List_Widget::load_points_list_file()
	{
		QFileDialog file_dialog;

		QString recent_config_file;
		recent_config_file = QString("%1").arg(config_db_->get_app_setting("recent_points_list_file").c_str());
	
		QFileInfo file_info(recent_config_file);
		file_dialog.setDirectory(file_info.dir().absolutePath());

		QString full_master_config_file_name=file_dialog.getOpenFileName(this);

		if (!full_master_config_file_name.isEmpty())
		{
			this->points_list_file_txt_->setText(full_master_config_file_name);

			config_db_->set_app_setting("recent_points_list_file", full_master_config_file_name.toLocal8Bit().data());

			this->load_points_list_file(full_master_config_file_name.toLocal8Bit().data());
		}
	}

	void Points_List_Widget::load_points_list_file(const std::string& pts_list_file_name)
	{
		this->points_list_.clear();
		ifstream config_file;
        config_file.open(pts_list_file_name.c_str());
		unsigned int record_id = 0;
		if (config_file.is_open())
		{
			string data;
			typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
			boost::char_separator<char> sep(",");
			const unsigned int data_colum_cnt = 6;
		
			unsigned int index = 0;
			while (config_file >> data)
			{
				++index;
				if (index == 1)
					continue; // skip first line

				tokenizer tokens(data, sep);
				unsigned int i=0;
				Point_Record record;
				string value_str;
				for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
				{
					if (i < data_colum_cnt)
					{
						string trimmed_token_str = tok_iter.current_token();
						boost::trim(trimmed_token_str);
						if (trimmed_token_str.size() < 1)
							continue;
						try
						{
							if (i == 0)
								record.point_name = tok_iter.current_token();
							else if (i == 1)
							{
								value_str = tok_iter.current_token();
							}
							else if (i == 2)
							{
								record.value_type = boost::lexical_cast<unsigned int>(tok_iter.current_token());

								if ((record.value_type == INT16) || (record.value_type == INT32)
									|| (record.value_type == INT32REV) || (record.value_type == INT32_BS)
									|| (record.value_type == INT32_WS_BS))
								{
									record.union_value.i_value = boost::lexical_cast<int>(value_str);
								}
								else if ((record.value_type == UINT16) || (record.value_type == UINT32)
									|| (record.value_type == UINT32REV) || (record.value_type == UINT32_BS)
									|| (record.value_type == UINT32_WS_BS))
								{
									record.union_value.ui_value = boost::lexical_cast<unsigned int>(value_str);
								}
								else if ((record.value_type == FLOAT) || (record.value_type == FLOATREV)
									|| (record.value_type == FLOAT_BS) || (record.value_type == FLOAT_WS_BS))
								{
									record.union_value.f_value = boost::lexical_cast<float>(value_str);
								}
								else if ((record.value_type == BOOLEAN))
								{
									record.union_value.b_value = boost::lexical_cast<bool>(value_str);
								}
							}
							else if (i == 3)
								record.io_type = boost::lexical_cast<unsigned int>(tok_iter.current_token());
							else if (i == 4)
								record.mbs_addr_ref = boost::lexical_cast<unsigned int>(tok_iter.current_token());
							else if (i == 5)
								record.mbs_addr = boost::lexical_cast<unsigned int>(tok_iter.current_token());
							++i;
						}
						catch(boost::bad_lexical_cast &)
						{
							Log_Sender::send_msg(QString("bad lexical cast"));
						}
					}
				}
						
				//record.union_value = data_line[1];
					
				this->points_list_.push_back(record);
				++record_id;
			} 
		}
		if (this->points_list_model_.use_count() > 0)
			this->points_list_model_->update_row_count();
		config_file.close();
		this->points_list_file_txt_->setText(pts_list_file_name.c_str());
	}

	void Points_List_Widget::save_points_list_file()
	{
		QFileDialog file_dialog;

		QString recent_config_file;
		recent_config_file = QString("%1").arg(config_db_->get_app_setting("recent_points_list_file").c_str());
		
		QFileInfo file_info(recent_config_file);
		file_dialog.setDirectory(file_info.dir().absolutePath());

		QString full_master_config_file_name=file_dialog.getSaveFileName(this, tr("Save Points List File"), recent_config_file);

		if (!full_master_config_file_name.isEmpty())
		{
			this->points_list_file_txt_->setText(full_master_config_file_name);

			config_db_->set_app_setting("recent_points_list_file", full_master_config_file_name.toLocal8Bit().data());

			this->save_points_list_file(full_master_config_file_name.toLocal8Bit().data());
			Log_Sender::send_msg(QString("Save master config file: %1").arg(full_master_config_file_name));
		}
	}

	void Points_List_Widget::save_points_list_file(const std::string& pts_list_file_name)
	{
		ofstream config_file;
        config_file.open(pts_list_file_name.c_str());
		config_file << "point_name,init_value,value_type,io_type,mbs_addr_ref,mbs_addr\n";
		string value_str;
		for (std::vector<Point_Record>::const_iterator record_idx=this->points_list_.begin();
			record_idx!=this->points_list_.end();++record_idx)
		{
			if ((record_idx->value_type == INT16) || (record_idx->value_type == INT32)
				|| (record_idx->value_type == INT32REV) || (record_idx->value_type == INT32_BS)
				|| (record_idx->value_type == INT32_WS_BS))
			{
				value_str = boost::lexical_cast<string>(record_idx->union_value.i_value);
			}
			else if ((record_idx->value_type == UINT16) || (record_idx->value_type == UINT32)
				|| (record_idx->value_type == UINT32REV) || (record_idx->value_type == UINT32_BS)
				|| (record_idx->value_type == UINT32_WS_BS))
			{
				value_str = boost::lexical_cast<string>(record_idx->union_value.ui_value);
			}
			else if ((record_idx->value_type == FLOAT) || (record_idx->value_type == FLOATREV)
				|| (record_idx->value_type == FLOAT_BS) || (record_idx->value_type == FLOAT_WS_BS))
			{
				value_str = boost::lexical_cast<string>(record_idx->union_value.f_value);
			}
			else if ((record_idx->value_type == BOOLEAN))
			{
				if (record_idx->union_value.b_value != 0)
					value_str = "1";
				else
					value_str = "0";
			}

			config_file << record_idx->point_name << "," <<  value_str << ","
				<< (unsigned int)record_idx->value_type << "," << (unsigned int)record_idx->io_type << ","
				<< (unsigned int)record_idx->mbs_addr_ref << "," << (unsigned int)record_idx->mbs_addr << "\n";
		}
		config_file.close();
	}

	void Points_List_Widget::load_recent_points_list()
	{
		this->load_points_list_file(config_db_->get_app_setting("recent_points_list_file"));
	}

	void Points_List_Widget::update_points_value()
	{
		;
	}

	void Points_List_Widget::closeEvent(QCloseEvent* event)
	{
		this->hide();
	}

	Points_List_Widget::~Points_List_Widget()
	{
	}
}
