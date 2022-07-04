/* Addr_Space_Model Body.
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

#include "modbus/address/addr_space.h"
#include "modbus/address/addr_space_model.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDateTime>

#include <iostream>
#include <boost/thread/thread.hpp>

using namespace std;

namespace cms
{
	Addr_Space_Model::Addr_Space_Model(boost::shared_ptr<Addr_Space> coil_addr_space, boost::shared_ptr<Addr_Space> dis_input_addr_space,
		boost::shared_ptr<Addr_Space> input_reg_addr_space, boost::shared_ptr<Addr_Space> holding_reg_addr_space, QObject *parent)
		: coil_addr_space_(coil_addr_space), dis_input_addr_space_(dis_input_addr_space),
		input_reg_addr_space_(input_reg_addr_space), holding_reg_addr_space_(holding_reg_addr_space), 
		QStandardItemModel(parent)
	{
		this->init();
	}

	int Addr_Space_Model::rowCount(const QModelIndex &/*parent*/) const
	{
		return MAX_ROW_CNT;
	}

	int Addr_Space_Model::columnCount(const QModelIndex &/*parent*/) const
	{
		return COLUMN_CNT;
	}

	QVariant Addr_Space_Model::data(const QModelIndex &index, int role) const
	{
		if (!index.isValid())
			return QVariant();

		if (role == Qt::DisplayRole || role == Qt::EditRole)
		{
			if (index.column() == REF_0X_INDEX)
			{
				return coil_addr_space_->get_bit_data(index.row());
			}
			else if (index.column() == REF_1X_INDEX)
			{
				return dis_input_addr_space_->get_bit_data(index.row());
			}
			else if (index.column() == REF_3X_INDEX)
			{
				return input_reg_addr_space_->get_point_data(index.row());
			}
			else if (index.column() == REF_4X_INDEX)
			{
				return holding_reg_addr_space_->get_point_data(index.row());
			}
			else
				return QVariant();
		}
		else
			return QVariant();
	}

	QVariant Addr_Space_Model::headerData(int section, Qt::Orientation orientation, int role) const
	{
		if (role != Qt::DisplayRole)
			return QVariant();
		if (orientation == Qt::Horizontal)
		{
			switch (section)
			{
			case REF_0X_INDEX:
				return tr("Coil Status(0X)");
			case REF_1X_INDEX:
				return tr("Input Status(1X)");
			case REF_3X_INDEX:
				return tr("Input Reg(3X)");
			case REF_4X_INDEX:
				return tr("Holding Reg(4X)");
			default:
				return QVariant();
			}
			return QString("1");
		}
		else
		{
			return QString("%1").arg(section);
		}
	}

	bool Addr_Space_Model::setData(const QModelIndex &index, const QVariant &value, int role)
	{
		if (index.isValid() && role == Qt::EditRole)
		{
			this->edit_data(index.row(), index.column(), value.toInt());
			emit dataChanged(index, index);
			return true;
		}
		return false;
	}

	Qt::ItemFlags Addr_Space_Model::flags(const QModelIndex &index) const
	{
		Qt::ItemFlags flags = QStandardItemModel::flags(index);
		if (index.row() != index.column())
			flags |= Qt::ItemIsEditable;
		return flags;
	}

	void Addr_Space_Model::init()
	{
		for (unsigned int row_idx=0; row_idx<MAX_ROW_CNT; ++row_idx)
		{
			for (unsigned int col_idx=0; col_idx<COLUMN_CNT; ++col_idx)
			{
				QStandardItem *item = this->item(row_idx, col_idx);
				this->setItem(row_idx,col_idx,item);                
			}
		}
	}

	void Addr_Space_Model::edit_data(unsigned int row, unsigned int col, int data)
	{
		if (col == REF_0X_INDEX)
		{
			coil_addr_space_->set_bit_data(row, data);
		}
		else if (col == REF_1X_INDEX)
		{
			dis_input_addr_space_->set_bit_data(row, data);;
		}
		else if (col == REF_3X_INDEX)
		{
			input_reg_addr_space_->set_point_data(row, data);
		}
		else if (col == REF_4X_INDEX)
		{
			holding_reg_addr_space_->set_point_data(row, data);
		}

		QStandardItem *item = this->item(row, col);
		QModelIndex index = this->indexFromItem(item);
		emit dataChanged(index, index);
	}

	Addr_Space_Model::~Addr_Space_Model()
	{
		//cout << "Addr_Space_Model deleted!" << endl;
	}
}