/* Log_Data_Model Header.
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

#ifndef CMS_LOG_DATA_MODEL_H
#define CMS_LOG_DATA_MODEL_H

#include <QStandardItemModel>
#include <QString>

#include <vector>
#include <boost/shared_ptr.hpp>

#include "log/log_server.h"

namespace cms
{
	class Log_Data
	{
	public:
		QString log_sender;
		QString time_stamp;
		QString msg;
	};

	class Log_Data_Model : public QStandardItemModel
	{
		Q_OBJECT
	public:
		static const int LOG_SENDER_INDEX = 0;
		static const int TIME_STAMP_INDEX = 1;
		static const int MSG_INDEX = 2;
		
		static const int COLUMN_CNT = 3;

		Log_Data_Model(QObject *parent = 0);
		~Log_Data_Model();

		void start_log_server();

		void append_msg(const QString& msg);
		void append_msg(const Log_Data& log_data);

		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	public:

	private:
		std::vector<Log_Data> log_data_list_;
        unsigned int max_log_entries_;
        unsigned int current_log_entry_;

		boost::asio::io_service io_service_;
		boost::shared_ptr<Log_Server> log_server_;
	};
}

#endif