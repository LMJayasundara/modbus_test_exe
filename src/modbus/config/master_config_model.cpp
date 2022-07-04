/* Master_Config_Model Body.
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
along with SmartDCS.comm_svr; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "modbus/config/master_config_model.h"
#include "modbus/config/mbs_def.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDateTime>

#include <iostream>
#include <boost/thread/thread.hpp>

using namespace std;

namespace cms
{
	Master_Config_Model::Master_Config_Model(std::vector<Master_Config_Record> &master_cfg, QObject *parent)
		: master_configuarion_ref_(master_cfg), last_row_count_(master_configuarion_ref_.size()), QStandardItemModel(parent)
	{
		this->init();
	}

	int Master_Config_Model::rowCount(const QModelIndex &/*parent*/) const
	{
		return this->master_configuarion_ref_.size();
		//return MAX_MASTER_CONFIG_RECORD;
	}

	int Master_Config_Model::columnCount(const QModelIndex &/*parent*/) const
	{
		return COLUMN_CNT;
	}

	QVariant Master_Config_Model::data(const QModelIndex &index, int role) const
	{
		if (!index.isValid())
			return QVariant();

		if (role == Qt::DisplayRole || role ==Qt::EditRole)
		{
			if (index.column() == SLAVE_ADDR_INDEX)
			{
				return master_configuarion_ref_[index.row()].slave_addr;
			}
			else if (index.column() == FC_INDEX)
			{
				return convert_function_code_to_str(master_configuarion_ref_[index.row()].function_code).c_str();
			}
			else if (index.column() == START_ADDR_INDEX)
			{
				return master_configuarion_ref_[index.row()].start_addr;
			}
			else if (index.column() == PT_CNT_INDEX)
			{
				return master_configuarion_ref_[index.row()].point_count;
			}
			else if (index.column() == RETRIES_INDEX)
			{
				return master_configuarion_ref_[index.row()].retries;
			}
			else if (index.column() == TIMEOUT_INDEX)
			{
				return master_configuarion_ref_[index.row()].timeout;
			}
			else
				return QVariant();
		}
		else
			return QVariant();
	}

	QVariant Master_Config_Model::headerData(int section, Qt::Orientation orientation, int role) const
	{
		if (role != Qt::DisplayRole)
			return QVariant();
		if (orientation == Qt::Horizontal)
		{
			switch (section)
			{
			case SLAVE_ADDR_INDEX:
				return tr("Slave Address");
			case FC_INDEX:
				return tr("Function Code");
			case START_ADDR_INDEX:
				return tr("Start Address");
			case PT_CNT_INDEX:
				return tr("Points Count");
			case RETRIES_INDEX:
				return tr("Retries");
			case TIMEOUT_INDEX:
				return tr("Timeout");
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

	bool Master_Config_Model::setData(const QModelIndex &index, const QVariant &value, int role)
	{
		if (index.isValid() && role == Qt::EditRole)
		{
			this->edit_data(index.row(), index.column(), value.toUInt());
			emit dataChanged(index, index);
			return true;
		}
		return false;
	}

	Qt::ItemFlags Master_Config_Model::flags(const QModelIndex &index) const
	{
		Qt::ItemFlags flags = QStandardItemModel::flags(index);
		if (index.row() != index.column())
			flags |= Qt::ItemIsEditable;
		return flags;
	}

	void Master_Config_Model::init()
	{
		// add first row, otherwise the new line can't be inserted
		unsigned int row_cnt = master_configuarion_ref_.size();
		if (row_cnt == 0)
			row_cnt = 1;
		for (unsigned int row_idx=0; row_idx<row_cnt; ++row_idx)
		{
			for (unsigned int col_idx=0; col_idx<COLUMN_CNT; ++col_idx)
			{
				QStandardItem *item = this->item(row_idx, col_idx);
				this->setItem(row_idx,col_idx,item);                
			}
		}
	}

	void Master_Config_Model::edit_data(unsigned int row, unsigned int col, int data)
	{
		if (col == SLAVE_ADDR_INDEX)
		{
			if (row < this->master_configuarion_ref_.size())
				this->master_configuarion_ref_[row].slave_addr = data;
		}
		else if (col == FC_INDEX)
		{
			this->master_configuarion_ref_[row].function_code = data;
		}
		else if (col == START_ADDR_INDEX)
		{
			this->master_configuarion_ref_[row].start_addr = data;
		}
		else if (col == PT_CNT_INDEX)
		{
			this->master_configuarion_ref_[row].point_count = data;
		}
		else if (col == RETRIES_INDEX)
		{
			this->master_configuarion_ref_[row].retries = data;
		}
		else if (col == TIMEOUT_INDEX)
		{
			this->master_configuarion_ref_[row].timeout = data;
		}

		QStandardItem *item = this->item(row, col);
		QModelIndex index = this->indexFromItem(item);
		emit dataChanged(index, index);
	}

	void Master_Config_Model::insert_row(unsigned int row)
	{
		Master_Config_Record record;
		record.slave_addr = 1;
		record.function_code = 3;
		record.start_addr = 0;
		record.point_count = 1;
		record.retries = 3;
		record.timeout = 1000;
		std::vector<Master_Config_Record>::iterator idx = this->master_configuarion_ref_.begin();
		this->master_configuarion_ref_.insert(idx+row, record);
		this->insertRow(row); // insert actual item row
	}

	void Master_Config_Model::remove_row(unsigned int row)
	{
		if (row < this->master_configuarion_ref_.size())
		{
			std::vector<Master_Config_Record>::iterator idx = this->master_configuarion_ref_.begin();
			this->master_configuarion_ref_.erase(idx + row);
			//this->master_configuarion_ref_.shrink_to_fit();
			this->removeRow(row);
		}
	}

	void Master_Config_Model::update_row_count()
	{
		this->removeRows(0, this->rowCount(QModelIndex()), QModelIndex());
		for (unsigned int row_idx=0; row_idx<this->master_configuarion_ref_.size(); ++row_idx)
		{
			for (unsigned int col_idx=0; col_idx<COLUMN_CNT; ++col_idx)
			{
				QStandardItem *item = this->item(row_idx, col_idx);
				this->setItem(row_idx,col_idx,item);                
			}
		}
		last_row_count_ = master_configuarion_ref_.size();
	}

	Master_Config_Model::~Master_Config_Model()
	{
		//cout << "Master_Config_Model deleted!" << endl;
	}
}