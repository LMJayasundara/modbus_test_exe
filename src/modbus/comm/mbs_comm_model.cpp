/* Mbs_Comm_Model Body.
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

#include "modbus/comm/mbs_comm_model.h"
#include "modbus/analyzer/mbs_analyzer.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDateTime>

#include <iostream>
#include <boost/thread/thread.hpp>

using namespace std;

namespace cms
{
	Mbs_Comm_Model::Mbs_Comm_Model(std::string mbs_analyzer_name, QObject *parent)
		: QStandardItemModel(parent), MAX_ROW_CNT(0)
	{
		mbs_analyzer_ = boost::shared_ptr<Mbs_Analyzer>(new Mbs_Analyzer(mbs_analyzer_name));
        mbs_analyzer_->init();
        MAX_ROW_CNT = mbs_analyzer_->get_max_record_content();

        comm_types.push_back("UNKNOWN");
        comm_types.push_back("TX");
        comm_types.push_back("RX");
		comm_types.push_back("DISCARD");

        this->init();
	}

	int Mbs_Comm_Model::rowCount(const QModelIndex &/*parent*/) const
	{
		return MAX_ROW_CNT;
	}

	int Mbs_Comm_Model::columnCount(const QModelIndex &/*parent*/) const
	{
		return COLUMN_CNT;
	}

	QVariant Mbs_Comm_Model::data(const QModelIndex &index, int role) const
	{
		if (!index.isValid())
			return QVariant();
		
		if (role == Qt::DisplayRole)
		{
			if (index.column() == ID_INDEX)
			{
				return mbs_analyzer_->get_record_id(index.row());
			}
			else if (index.column() == LENGTH_INDEX)
			{
				return mbs_analyzer_->get_record_content_length(index.row());
			}
            else if (index.column() == TYPE_INDEX)
            {
                unsigned int comm_type = mbs_analyzer_->get_comm_type(index.row());
                return this->get_comm_type_str(comm_type).c_str();
            }
			else if (index.column() == CONTENT_INDEX)
			{
				return mbs_analyzer_->get_record_content_by_str(index.row()).c_str();
			}
			else
				return QVariant();
		}
		else
			return QVariant();
	}

	QVariant Mbs_Comm_Model::headerData(int section, Qt::Orientation orientation, int role) const
	{
		if (role != Qt::DisplayRole)
			return QVariant();
		if (orientation == Qt::Horizontal)
		{
			switch (section)
			{
			case ID_INDEX:
				return  tr("Id");
			case LENGTH_INDEX:
				return  tr("Length");
            case TYPE_INDEX:
                return  tr("Type");
			case CONTENT_INDEX:
				return tr("Content");
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

	bool Mbs_Comm_Model::setData(const QModelIndex &index, const QVariant &/*value*/, int role)
	{
		if (index.isValid() && role == Qt::EditRole)
		{
			emit dataChanged(index, index);
			return true;
		}
		return false;
	}

	Qt::ItemFlags Mbs_Comm_Model::flags(const QModelIndex &index) const
	{
		Qt::ItemFlags flags = QStandardItemModel::flags(index);
		if (index.row() != index.column())
			flags |= Qt::ItemIsEditable;
		return flags;
	}

	void Mbs_Comm_Model::init()
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

    std::string Mbs_Comm_Model::get_comm_type_str(unsigned int comm_type) const
    {
        if (comm_type<0 || comm_type>=comm_types.size())
        {
            return comm_types[0];
        }
        else
        {
            return comm_types[comm_type];
        }
    }

	Mbs_Comm_Model::~Mbs_Comm_Model()
	{
		//cout << "Addr_Space_Model deleted!" << endl;
	}
}