/* Addr_Space_Model Header.
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

#ifndef CMS_ADDR_SPACE_MODEL_H
#define CMS_ADDR_SPACE_MODEL_H

#include <QStandardItemModel>
#include <QString>

#include <vector>
#include <boost/shared_ptr.hpp>

namespace cms
{
	class Addr_Space;

	class Addr_Space_Model : public QStandardItemModel
	{
		Q_OBJECT
	public:
		static const int REF_0X_INDEX = 0;
		static const int REF_1X_INDEX = 1;
		static const int REF_3X_INDEX = 2;
		static const int REF_4X_INDEX = 3;
		static const int MAX_ROW_CNT = 65536;
		static const int COLUMN_CNT = 4;

		Addr_Space_Model(boost::shared_ptr<Addr_Space> coil_addr_space, boost::shared_ptr<Addr_Space> dis_input_addr_space,
			boost::shared_ptr<Addr_Space> input_reg_addr_space, boost::shared_ptr<Addr_Space> holding_reg_addr_space, QObject *parent = 0);
		~Addr_Space_Model();

		void init();

		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

		virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
		virtual Qt::ItemFlags flags(const QModelIndex & index) const;

	public:
		void edit_data(unsigned int row, unsigned int col, int data);

	private:
		boost::shared_ptr<Addr_Space> coil_addr_space_;
		boost::shared_ptr<Addr_Space> dis_input_addr_space_;
		boost::shared_ptr<Addr_Space> input_reg_addr_space_;
		boost::shared_ptr<Addr_Space> holding_reg_addr_space_;
	};
}

#endif