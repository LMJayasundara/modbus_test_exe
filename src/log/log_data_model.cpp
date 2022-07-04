/* Log_Data_Model Body.
Copyright (C) 2012
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


#include "log/log_data_model.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDateTime>

#include <iostream>
#include <boost/thread/thread.hpp>

using namespace std;

namespace cms
{
	Log_Data_Model::Log_Data_Model(QObject *parent)
		: QStandardItemModel(parent)
	{
        current_log_entry_ = 0;
        max_log_entries_ = 1000;

		// append 1st message, otherwise the column will be double
		Log_Data log_data;
		log_data.log_sender = "any";
		log_data.time_stamp = QDateTime::currentDateTime().toString(Qt::ISODate);
		log_data.msg = "log server started!";
		log_data_list_.push_back(log_data);
        for (unsigned int col_idx=0; col_idx<COLUMN_CNT; ++col_idx)
        {
            QStandardItem *item = new QStandardItem();
            this->setItem(current_log_entry_,col_idx,item);
        }
        ++current_log_entry_;

		this->start_log_server();
	}

	int Log_Data_Model::rowCount(const QModelIndex &/*parent*/) const
	{
		return log_data_list_.size();
	}

	int Log_Data_Model::columnCount(const QModelIndex &/*parent*/) const
	{
		return COLUMN_CNT;
	}

	QVariant Log_Data_Model::data(const QModelIndex &index, int role) const
	{
		if (!index.isValid())
			return QVariant();
       
		if (role == Qt::DisplayRole && ((unsigned int)(index.row()) < log_data_list_.size()))
		{
			if (index.column() == LOG_SENDER_INDEX)
            {
                return (log_data_list_[index.row()].log_sender);
            }
			else if (index.column() == TIME_STAMP_INDEX)
            {
                return log_data_list_[index.row()].time_stamp;
            }
			else if (index.column() == MSG_INDEX)
            {
                return log_data_list_[index.row()].msg;
            }
			else
				return QVariant();
		}
		else
			return QVariant();
	}

	QVariant Log_Data_Model::headerData(int section, Qt::Orientation orientation, int role) const
	{
		if (role != Qt::DisplayRole || log_data_list_.size()==0)
			return QVariant();
		if (orientation == Qt::Horizontal && log_data_list_.size() > 0)
		{
			switch (section)
			{
			case LOG_SENDER_INDEX:
				return tr("Sender");
			case TIME_STAMP_INDEX:
				return tr("Timestamp");
			case MSG_INDEX:
				return tr("Message");
			default:
				return QVariant();
			}
			return QString("1");
		}
		else
		{
			return QString("%1").arg(section + 1);
		}
	}
	
	void Log_Data_Model::start_log_server()
	{
		try
		{
			unsigned int log_svr_listen_port = 61234;
			using namespace std; // For atoi.
			log_server_ = boost::shared_ptr<Log_Server>(new Log_Server(io_service_, log_svr_listen_port, *this));

			boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service_));
		}
		catch (std::exception& e)
		{
			std::cerr << "Start Log Server Exception: " << e.what() << "\n";
		}
	}

	void Log_Data_Model::append_msg(const QString& msg)
	{
		QStringList msg_list = msg.split("|");

		Log_Data log_data;
		if (msg_list.count() == 3)
		{			
			log_data.log_sender = msg_list[0];
			log_data.time_stamp = msg_list[1];
			log_data.msg = msg_list[2];	
		}
		else
		{
			log_data.log_sender = "Unkown";
			log_data.time_stamp = QDateTime::currentDateTime().toString();
			log_data.msg = msg;
		}
		this->append_msg(log_data);
	}

	void Log_Data_Model::append_msg(const Log_Data& log_data)
	{    
        log_data_list_.push_back(log_data);
		
		for (unsigned int col_idx=0; col_idx<COLUMN_CNT; ++col_idx)
		{
			QStandardItem *item = this->item(current_log_entry_, col_idx);
            if (item)
            {
                this->setItem(current_log_entry_,col_idx,item);                
            }
            else
            {
                item = new QStandardItem();
                this->setItem(current_log_entry_,col_idx,item);
            }
		}

        ++current_log_entry_;
        if (current_log_entry_ >= max_log_entries_)
        {
            current_log_entry_ = 0;
            
            log_data_list_.clear();
            this->removeRows(0, max_log_entries_);
        }
	}

	Log_Data_Model::~Log_Data_Model()
	{
		io_service_.stop(); // the stop should be done, otherwise the application can't exit normally
	}
}