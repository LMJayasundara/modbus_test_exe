/* Master_Config_Model Header.
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

#ifndef CMS_MASTER_CONFIG_MODEL_H
#define CMS_MASTER_CONFIG_MODEL_H

#include <QStandardItemModel>
#include <QString>
#include <QTimer>

#include <vector>
#include <boost/shared_ptr.hpp>

#include "point/point_record.h"

namespace cms
{
	class Addr_Space;

	class Points_List_Model : public QStandardItemModel
	{
		Q_OBJECT
	public:
		static const int PT_NAME_INDEX = 0;
		static const int VALUE_INDEX = 1;
		static const int VALUE_TYPE_INDEX = 2;
		static const int IO_TYPE_INDEX = 3;
		static const int MBS_ADDR_REF_INDEX = 4;
		static const int MBS_ADDR_INDEX = 5;
		static const int COLUMN_CNT = 6;

		Points_List_Model(boost::shared_ptr<Addr_Space> coil, boost::shared_ptr<Addr_Space> dis_input,
			boost::shared_ptr<Addr_Space> input_reg, boost::shared_ptr<Addr_Space> holding_reg,
			std::vector<Point_Record> &points_list, QObject *parent = 0);
		~Points_List_Model();

		void init();

		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

		virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
		virtual Qt::ItemFlags flags(const QModelIndex & index) const;

	public:
		void edit_data(unsigned int row, unsigned int col, const QVariant &data);
		void insert_row(unsigned int row);
		void remove_row(unsigned int row);
		void update_row_count();

	public slots:
		void update_points_value();

	private:
		void update_pt_value(Point_Record& pt);

		std::vector<Point_Record> &points_list_ref_;
		unsigned int last_row_count_;

		// only holding reference
		boost::shared_ptr<cms::Addr_Space> coil_addr_space_;
		boost::shared_ptr<cms::Addr_Space> dis_input_addr_space_;
		boost::shared_ptr<cms::Addr_Space> input_reg_addr_space_;
		boost::shared_ptr<cms::Addr_Space> holding_reg_addr_space_;

		boost::shared_ptr<QTimer> update_pts_timer_;
	};
}

#endif