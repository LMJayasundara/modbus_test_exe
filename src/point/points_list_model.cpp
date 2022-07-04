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

#include "point/points_list_model.h"
#include "modbus/address/addr_space.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDateTime>

#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/numeric/conversion/cast.hpp>

using namespace std;

namespace cms
{
	Points_List_Model::Points_List_Model(boost::shared_ptr<Addr_Space> coil, boost::shared_ptr<Addr_Space> dis_input,
		boost::shared_ptr<Addr_Space> input_reg, boost::shared_ptr<Addr_Space> holding_reg,
		std::vector<Point_Record> &points_list, QObject *parent)
		: coil_addr_space_(coil), dis_input_addr_space_(dis_input),
		input_reg_addr_space_(input_reg), holding_reg_addr_space_(holding_reg),
		points_list_ref_(points_list), last_row_count_(points_list_ref_.size()), QStandardItemModel(parent)
	{
		this->init();
	}

	int Points_List_Model::rowCount(const QModelIndex &/*parent*/) const
	{
		return this->points_list_ref_.size();
	}

	int Points_List_Model::columnCount(const QModelIndex &/*parent*/) const
	{
		return COLUMN_CNT;
	}

	QVariant Points_List_Model::data(const QModelIndex &index, int role) const
	{
		if (!index.isValid())
			return QVariant();

		if (role == Qt::DisplayRole || role ==Qt::EditRole)
		{
			if (index.column() == PT_NAME_INDEX)
			{
				return points_list_ref_[index.row()].point_name.c_str();
			}
			if (index.column() == VALUE_INDEX)
			{
				if ((points_list_ref_[index.row()].value_type == INT16) || (points_list_ref_[index.row()].value_type == INT32)
					|| (points_list_ref_[index.row()].value_type == INT32REV) || (points_list_ref_[index.row()].value_type == INT32_BS)
					|| (points_list_ref_[index.row()].value_type == INT32_WS_BS))
				{
					return points_list_ref_[index.row()].union_value.i_value;
				}
				else if ((points_list_ref_[index.row()].value_type == UINT16) || (points_list_ref_[index.row()].value_type == UINT32)
					|| (points_list_ref_[index.row()].value_type == UINT32REV) || (points_list_ref_[index.row()].value_type == UINT32_BS)
					|| (points_list_ref_[index.row()].value_type == UINT32_WS_BS))
				{
					return points_list_ref_[index.row()].union_value.ui_value;
				}
				else if ((points_list_ref_[index.row()].value_type == FLOAT) || (points_list_ref_[index.row()].value_type == FLOATREV)
					|| (points_list_ref_[index.row()].value_type == FLOAT_BS) || (points_list_ref_[index.row()].value_type == FLOAT_WS_BS))
				{
					return points_list_ref_[index.row()].union_value.f_value;
				}
				else if ((points_list_ref_[index.row()].value_type == BOOLEAN))
				{
					return points_list_ref_[index.row()].union_value.b_value;
				}
			}
			if (index.column() == VALUE_TYPE_INDEX)
			{
				return convert_value_type_to_str(points_list_ref_[index.row()].value_type).c_str();
			}
			else if (index.column() == IO_TYPE_INDEX)
			{
				return convert_io_type_to_str(points_list_ref_[index.row()].io_type).c_str();
			}
			else if (index.column() == MBS_ADDR_REF_INDEX)
			{
				return convert_mbs_addr_type_to_str(points_list_ref_[index.row()].mbs_addr_ref).c_str();
			}
			else if (index.column() == MBS_ADDR_INDEX)
			{
				return points_list_ref_[index.row()].mbs_addr;
			}
			else
				return QVariant();
		}
		else
			return QVariant();
	}

	QVariant Points_List_Model::headerData(int section, Qt::Orientation orientation, int role) const
	{
		if (role != Qt::DisplayRole)
			return QVariant();
		if (orientation == Qt::Horizontal)
		{
			switch (section)
			{
			case PT_NAME_INDEX:
				return tr("Point Name");
			case VALUE_INDEX:
				return tr("Value");
			case VALUE_TYPE_INDEX:
				return tr("Value Type");
			case IO_TYPE_INDEX:
				return tr("IO Type");
			case MBS_ADDR_REF_INDEX:
				return tr("Modbus Addr Type");
			case MBS_ADDR_INDEX:
				return tr("Modbus Start Addr");
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

	bool Points_List_Model::setData(const QModelIndex &index, const QVariant &value, int role)
	{
		if (index.isValid() && role == Qt::EditRole)
		{
			this->edit_data(index.row(), index.column(), value);
			emit dataChanged(index, index);
			return true;
		}
		return false;
	}

	Qt::ItemFlags Points_List_Model::flags(const QModelIndex &index) const
	{
		Qt::ItemFlags flags = QStandardItemModel::flags(index);
		if (index.row() != index.column())
			flags |= Qt::ItemIsEditable;
		return flags;
	}

	void Points_List_Model::init()
	{
		unsigned int row_cnt = points_list_ref_.size();
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

		update_pts_timer_ = boost::shared_ptr<QTimer>(new QTimer(this));    
		update_pts_timer_->start(1000);
		connect(update_pts_timer_.get(), SIGNAL(timeout()), this, SLOT(update_points_value()));
	}

	void Points_List_Model::edit_data(unsigned int row, unsigned int col, const QVariant &data)
	{
		if (col == PT_NAME_INDEX)
		{
			if (row < this->points_list_ref_.size())
				this->points_list_ref_[row].point_name = data.toString().toLocal8Bit().data();
		}
		if (col == VALUE_INDEX)
		{
			if ((points_list_ref_[row].value_type == INT16) || (points_list_ref_[row].value_type == INT32)
				|| (points_list_ref_[row].value_type == INT32REV) || (points_list_ref_[row].value_type == INT32_BS)
				|| (points_list_ref_[row].value_type == INT32_WS_BS))
			{
				points_list_ref_[row].union_value.i_value = data.toInt();
			}
			else if ((points_list_ref_[row].value_type == UINT16) || (points_list_ref_[row].value_type == UINT32)
				|| (points_list_ref_[row].value_type == UINT32REV) || (points_list_ref_[row].value_type == UINT32_BS)
				|| (points_list_ref_[row].value_type == UINT32_WS_BS))
			{
				points_list_ref_[row].union_value.ui_value = data.toUInt();
			}
			else if ((points_list_ref_[row].value_type == FLOAT) || (points_list_ref_[row].value_type == FLOATREV)
				|| (points_list_ref_[row].value_type == FLOAT_BS) || (points_list_ref_[row].value_type == FLOAT_WS_BS))
			{
				points_list_ref_[row].union_value.f_value = data.toFloat();
			}
			else if ((points_list_ref_[row].value_type == BOOLEAN))
			{
				points_list_ref_[row].union_value.b_value = data.toBool();
			}
		}
		if (col == VALUE_TYPE_INDEX)
		{
			this->points_list_ref_[row].value_type = data.toUInt();
		}
		else if (col == IO_TYPE_INDEX)
		{
			this->points_list_ref_[row].io_type = data.toUInt();
		}
		else if (col == MBS_ADDR_REF_INDEX)
		{
			this->points_list_ref_[row].mbs_addr_ref = data.toUInt();
		}
		else if (col == MBS_ADDR_INDEX)
		{
			this->points_list_ref_[row].mbs_addr = data.toUInt();
		}

		QStandardItem *item = this->item(row, col);
		QModelIndex index = this->indexFromItem(item);
		emit dataChanged(index, index);
	}

	void Points_List_Model::insert_row(unsigned int row)
	{
		Point_Record record;
		record.point_name = "new_pts";
		std::vector<Point_Record>::iterator idx = this->points_list_ref_.begin();
		this->points_list_ref_.insert(idx+row, record);
		this->insertRow(row); // insert actual item row
	}

	void Points_List_Model::remove_row(unsigned int row)
	{
		if (row < this->points_list_ref_.size())
		{
			std::vector<Point_Record>::iterator idx = this->points_list_ref_.begin();
			this->points_list_ref_.erase(idx + row);
			this->removeRow(row);
		}
	}

	void Points_List_Model::update_row_count()
	{
		this->removeRows(0, this->rowCount(QModelIndex()), QModelIndex());
		for (unsigned int row_idx=0; row_idx<this->points_list_ref_.size(); ++row_idx)
		{
			for (unsigned int col_idx=0; col_idx<COLUMN_CNT; ++col_idx)
			{
				QStandardItem *item = this->item(row_idx, col_idx);
				this->setItem(row_idx,col_idx,item);                
			}
		}
		last_row_count_ = points_list_ref_.size();
	}

	void Points_List_Model::update_points_value()
	{
		for (std::vector<Point_Record>::iterator pt_index=points_list_ref_.begin(); pt_index!=points_list_ref_.end(); ++pt_index)
		{
			update_pt_value(*pt_index);
		}
	}

	void Points_List_Model::update_pt_value(Point_Record& pt)
	{
		try
		{
			if (pt.mbs_addr > MBS_LARGEST_ADDR)
				return;

			if (pt.io_type == INPUT)
			{
				if (pt.value_type == INT16)
				{
					if (pt.mbs_addr_ref == REF_3X)
					{
						pt.union_value.i_value = (CMS_INT16)((CMS_UINT16)input_reg_addr_space_->get_point_data(pt.mbs_addr));
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						pt.union_value.i_value = (CMS_INT16)((CMS_UINT16)holding_reg_addr_space_->get_point_data(pt.mbs_addr));
					}
				}
				else if (pt.value_type == UINT16)
				{
					if (pt.mbs_addr_ref == REF_3X)
					{
						pt.union_value.ui_value = input_reg_addr_space_->get_point_data(pt.mbs_addr);
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						pt.union_value.ui_value = holding_reg_addr_space_->get_point_data(pt.mbs_addr);
					}
				}
                else if (pt.value_type == INT32)
				{
					if (pt.mbs_addr + 1 > MBS_LARGEST_ADDR)
						return;
					if (pt.mbs_addr_ref == REF_3X)
					{
						pt.union_value.i_value = input_reg_addr_space_->get_point_data(pt.mbs_addr)
                            + input_reg_addr_space_->get_point_data(pt.mbs_addr+1) * 0x10000;
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						pt.union_value.i_value = holding_reg_addr_space_->get_point_data(pt.mbs_addr)
                            + holding_reg_addr_space_->get_point_data(pt.mbs_addr+1) * 0x10000;
					}
				}
                else if (pt.value_type == INT32REV)
				{
					if (pt.mbs_addr + 1 > MBS_LARGEST_ADDR)
						return;
					if (pt.mbs_addr_ref == REF_3X)
					{
						pt.union_value.i_value = input_reg_addr_space_->get_point_data(pt.mbs_addr+1)
                            + input_reg_addr_space_->get_point_data(pt.mbs_addr) * 0x10000;
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						pt.union_value.i_value = holding_reg_addr_space_->get_point_data(pt.mbs_addr+1)
                            + holding_reg_addr_space_->get_point_data(pt.mbs_addr) * 0x10000;
					}
				}
				else if (pt.value_type == INT32_BS)
				{
					if (pt.mbs_addr + 1 > MBS_LARGEST_ADDR)
						return;
					if (pt.mbs_addr_ref == REF_3X)
					{
						pt.union_value.i_value = input_reg_addr_space_->get_point_low(pt.mbs_addr)*0x100 + input_reg_addr_space_->get_point_high(pt.mbs_addr)
							+ (input_reg_addr_space_->get_point_low(pt.mbs_addr+1)*0x100 + input_reg_addr_space_->get_point_high(pt.mbs_addr+1))* 0x10000;
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						pt.union_value.i_value = holding_reg_addr_space_->get_point_low(pt.mbs_addr)*0x100 + holding_reg_addr_space_->get_point_high(pt.mbs_addr)
                            + (holding_reg_addr_space_->get_point_low(pt.mbs_addr+1)*0x100 + holding_reg_addr_space_->get_point_high(pt.mbs_addr+1)) * 0x10000;
					}
				}
				else if (pt.value_type == INT32_WS_BS)
				{
					if (pt.mbs_addr + 1 > MBS_LARGEST_ADDR)
						return;
					if (pt.mbs_addr_ref == REF_3X)
					{
						pt.union_value.i_value = (input_reg_addr_space_->get_point_low(pt.mbs_addr)*0x100 + input_reg_addr_space_->get_point_high(pt.mbs_addr)) * 0x10000
							+ input_reg_addr_space_->get_point_low(pt.mbs_addr+1)*0x100 + input_reg_addr_space_->get_point_high(pt.mbs_addr+1);
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						pt.union_value.i_value = (holding_reg_addr_space_->get_point_low(pt.mbs_addr)*0x100 + holding_reg_addr_space_->get_point_high(pt.mbs_addr)) * 0x10000
                            + holding_reg_addr_space_->get_point_low(pt.mbs_addr+1)*0x100 + holding_reg_addr_space_->get_point_high(pt.mbs_addr+1);
					}
				}
				else if ((pt.value_type == UINT32) || (pt.value_type == FLOAT))
				{
					if (pt.mbs_addr + 1 > MBS_LARGEST_ADDR)
						return;
					if (pt.mbs_addr_ref == REF_3X)
					{
						pt.union_value.i_value = (CMS_UINT32)input_reg_addr_space_->get_point_data(pt.mbs_addr)
                            + (CMS_UINT32)input_reg_addr_space_->get_point_data(pt.mbs_addr+1) * 0x10000;
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						pt.union_value.i_value = (CMS_UINT32)holding_reg_addr_space_->get_point_data(pt.mbs_addr)
                            + (CMS_UINT32)holding_reg_addr_space_->get_point_data(pt.mbs_addr+1) * 0x10000;
					}
				}
                else if ((pt.value_type == UINT32REV) || (pt.value_type == FLOATREV))
				{
					if (pt.mbs_addr + 1 > MBS_LARGEST_ADDR)
						return;
					if (pt.mbs_addr_ref == REF_3X)
					{
						pt.union_value.i_value = (CMS_UINT32)input_reg_addr_space_->get_point_data(pt.mbs_addr+1)
                            + (CMS_UINT32)input_reg_addr_space_->get_point_data(pt.mbs_addr) * 0x10000;
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						pt.union_value.i_value = (CMS_UINT32)holding_reg_addr_space_->get_point_data(pt.mbs_addr+1)
                            + (CMS_UINT32)holding_reg_addr_space_->get_point_data(pt.mbs_addr) * 0x10000;
					}
				}
				else if ((pt.value_type == UINT32_BS) || (pt.value_type == FLOAT_BS))
				{
					if (pt.mbs_addr + 1 > MBS_LARGEST_ADDR)
						return;
					if (pt.mbs_addr_ref == REF_3X)
					{
						pt.union_value.i_value = (CMS_UINT16)input_reg_addr_space_->get_point_low(pt.mbs_addr)*0x100 + (CMS_UINT16)input_reg_addr_space_->get_point_high(pt.mbs_addr)
							+ (CMS_UINT16)(input_reg_addr_space_->get_point_low(pt.mbs_addr+1)*0x100 + (CMS_UINT32)input_reg_addr_space_->get_point_high(pt.mbs_addr+1))* 0x10000;
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						pt.union_value.i_value = (CMS_UINT16)holding_reg_addr_space_->get_point_low(pt.mbs_addr)*0x100 + (CMS_UINT16)holding_reg_addr_space_->get_point_high(pt.mbs_addr)
                            + (CMS_UINT16)(holding_reg_addr_space_->get_point_low(pt.mbs_addr+1)*0x100 + (CMS_UINT32)holding_reg_addr_space_->get_point_high(pt.mbs_addr+1)) * 0x10000;
					}
				}
				else if ((pt.value_type == UINT32_WS_BS) || (pt.value_type == FLOAT_WS_BS))
				{
					if (pt.mbs_addr + 1 > MBS_LARGEST_ADDR)
						return;
					if (pt.mbs_addr_ref == REF_3X)
					{
						pt.union_value.i_value = (CMS_UINT16)(input_reg_addr_space_->get_point_low(pt.mbs_addr)*0x100 + (CMS_UINT32)input_reg_addr_space_->get_point_high(pt.mbs_addr)) * 0x10000
							+ (CMS_UINT16)input_reg_addr_space_->get_point_low(pt.mbs_addr+1)*0x100 + (CMS_UINT16)input_reg_addr_space_->get_point_high(pt.mbs_addr+1);
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						pt.union_value.i_value = (CMS_UINT16)(holding_reg_addr_space_->get_point_low(pt.mbs_addr)*0x100 + (CMS_UINT32)holding_reg_addr_space_->get_point_high(pt.mbs_addr)) * 0x10000
                            + (CMS_UINT16)holding_reg_addr_space_->get_point_low(pt.mbs_addr+1)*0x100 + (CMS_UINT16)holding_reg_addr_space_->get_point_high(pt.mbs_addr+1);
					}
				}
				else if (pt.value_type == BOOLEAN)
				{
					if (pt.mbs_addr_ref == REF_0X)
					{
						pt.union_value.b_value = coil_addr_space_->get_bit_data(pt.mbs_addr);
					}
					else if (pt.mbs_addr_ref == REF_1X)
					{
						pt.union_value.b_value = dis_input_addr_space_->get_bit_data(pt.mbs_addr);
					}
				}
			}
			else if (pt.io_type == OUTPUT)
			{
				if (pt.value_type == INT16)
				{
					if (pt.mbs_addr_ref == REF_3X)
					{
						CMS_UINT16 val = (CMS_UINT16)pt.union_value.i_value;
						input_reg_addr_space_->set_point_data(pt.mbs_addr, val);
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						CMS_UINT16 val = (CMS_UINT16)pt.union_value.i_value;
						holding_reg_addr_space_->set_point_data(pt.mbs_addr, val);
					}
				}
				else if (pt.value_type == UINT16)
				{
					if (pt.mbs_addr_ref == REF_3X)
					{
						input_reg_addr_space_->set_point_data(pt.mbs_addr, pt.union_value.ui_value);
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						holding_reg_addr_space_->set_point_data(pt.mbs_addr, pt.union_value.ui_value);
					}
				}
                else if (pt.value_type == INT32)
				{
					if (pt.mbs_addr + 1 > MBS_LARGEST_ADDR)
						return;
					if (pt.mbs_addr_ref == REF_3X)
					{
						CMS_UINT16 high_val = (CMS_UINT16)(pt.union_value.i_value >> 16);
						CMS_UINT16 low_val = (CMS_UINT16)(pt.union_value.i_value & 0x0000FFFF);
						input_reg_addr_space_->set_point_data(pt.mbs_addr, low_val);
						input_reg_addr_space_->set_point_data(pt.mbs_addr+1, high_val);
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						CMS_UINT16 high_val = (CMS_UINT16)(pt.union_value.i_value >> 16);
						CMS_UINT16 low_val = (CMS_UINT16)(pt.union_value.i_value & 0x0000FFFF);
						holding_reg_addr_space_->set_point_data(pt.mbs_addr, low_val);
						holding_reg_addr_space_->set_point_data(pt.mbs_addr+1, high_val);
					}
				}
                else if (pt.value_type == INT32REV)
				{
					if (pt.mbs_addr + 1 > MBS_LARGEST_ADDR)
						return;
					if (pt.mbs_addr_ref == REF_3X)
					{
						CMS_UINT16 low_val = (CMS_UINT16)(pt.union_value.i_value >> 16);
						CMS_UINT16 high_val = (CMS_UINT16)(pt.union_value.i_value & 0x0000FFFF);
						input_reg_addr_space_->set_point_data(pt.mbs_addr, low_val);
						input_reg_addr_space_->set_point_data(pt.mbs_addr+1, high_val);
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						CMS_UINT16 low_val = (CMS_UINT16)(pt.union_value.i_value >> 16);
						CMS_UINT16 high_val = (CMS_UINT16)(pt.union_value.i_value & 0x0000FFFF);
						holding_reg_addr_space_->set_point_data(pt.mbs_addr, low_val);
						holding_reg_addr_space_->set_point_data(pt.mbs_addr+1, high_val);
					}
				}
				else if (pt.value_type == INT32_BS)
				{
					if (pt.mbs_addr + 1 > MBS_LARGEST_ADDR)
						return;
                    if (pt.mbs_addr_ref == REF_3X)
					{
						CMS_UINT16 high_val = (CMS_UINT16)(pt.union_value.i_value >> 16);
						CMS_UINT16 low_val = (CMS_UINT16)(pt.union_value.i_value & 0x0000FFFF);
						input_reg_addr_space_->set_point_low(pt.mbs_addr, low_val >> 8);
                        input_reg_addr_space_->set_point_high(pt.mbs_addr, low_val & 0xFF);
                        input_reg_addr_space_->set_point_low(pt.mbs_addr+1, high_val >> 8);
						input_reg_addr_space_->set_point_high(pt.mbs_addr+1, high_val & 0xFF);
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
                        CMS_UINT16 high_val = (CMS_UINT16)(pt.union_value.i_value >> 16);
						CMS_UINT16 low_val = (CMS_UINT16)(pt.union_value.i_value & 0x0000FFFF);
						holding_reg_addr_space_->set_point_low(pt.mbs_addr, low_val >> 8);
                        holding_reg_addr_space_->set_point_high(pt.mbs_addr, low_val & 0xFF);
                        holding_reg_addr_space_->set_point_low(pt.mbs_addr+1, high_val >> 8);
						holding_reg_addr_space_->set_point_high(pt.mbs_addr+1, high_val & 0xFF);
					}
				}
				else if (pt.value_type == INT32_WS_BS)
				{
					if (pt.mbs_addr + 1 > MBS_LARGEST_ADDR)
						return;
					if (pt.mbs_addr_ref == REF_3X)
					{
						CMS_UINT16 low_val = (CMS_UINT16)(pt.union_value.i_value >> 16);
						CMS_UINT16 high_val = (CMS_UINT16)(pt.union_value.i_value & 0x0000FFFF);
						input_reg_addr_space_->set_point_low(pt.mbs_addr, low_val >> 8);
						input_reg_addr_space_->set_point_high(pt.mbs_addr, low_val & 0xFF);
						input_reg_addr_space_->set_point_low(pt.mbs_addr+1, high_val >> 8);
						input_reg_addr_space_->set_point_high(pt.mbs_addr+1, high_val & 0xFF);
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						CMS_UINT16 low_val = (CMS_UINT16)(pt.union_value.i_value >> 16);
						CMS_UINT16 high_val = (CMS_UINT16)(pt.union_value.i_value & 0x0000FFFF);
						holding_reg_addr_space_->set_point_low(pt.mbs_addr, low_val >> 8);
						holding_reg_addr_space_->set_point_high(pt.mbs_addr, low_val & 0xFF);
						holding_reg_addr_space_->set_point_low(pt.mbs_addr+1, high_val >> 8);
						holding_reg_addr_space_->set_point_high(pt.mbs_addr+1, high_val & 0xFF);
					}
				}
				else if ((pt.value_type == UINT32) || (pt.value_type == FLOAT))
				{
					if (pt.mbs_addr + 1 > MBS_LARGEST_ADDR)
						return;
					if (pt.mbs_addr_ref == REF_3X)
					{
						CMS_UINT16 high_val = (CMS_UINT16)(pt.union_value.ui_value >> 16);
						CMS_UINT16 low_val = (CMS_UINT16)(pt.union_value.ui_value & 0x0000FFFF);
						input_reg_addr_space_->set_point_data(pt.mbs_addr, low_val);
						input_reg_addr_space_->set_point_data(pt.mbs_addr+1, high_val);
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						CMS_UINT16 high_val = (CMS_UINT16)(pt.union_value.ui_value >> 16);
						CMS_UINT16 low_val = (CMS_UINT16)(pt.union_value.ui_value & 0x0000FFFF);
						holding_reg_addr_space_->set_point_data(pt.mbs_addr, low_val);
						holding_reg_addr_space_->set_point_data(pt.mbs_addr+1, high_val);
					}
				}
                else if ((pt.value_type == UINT32REV) || (pt.value_type == FLOATREV))
				{
					if (pt.mbs_addr + 1 > MBS_LARGEST_ADDR)
						return;
					if (pt.mbs_addr_ref == REF_3X)
					{
						CMS_UINT16 low_val = (CMS_UINT16)(pt.union_value.ui_value >> 16);
						CMS_UINT16 high_val = (CMS_UINT16)(pt.union_value.ui_value & 0x0000FFFF);
						input_reg_addr_space_->set_point_data(pt.mbs_addr, low_val);
						input_reg_addr_space_->set_point_data(pt.mbs_addr+1, high_val);
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						CMS_UINT16 low_val = (CMS_UINT16)(pt.union_value.ui_value >> 16);
						CMS_UINT16 high_val = (CMS_UINT16)(pt.union_value.ui_value & 0x0000FFFF);
						holding_reg_addr_space_->set_point_data(pt.mbs_addr, low_val);
						holding_reg_addr_space_->set_point_data(pt.mbs_addr+1, high_val);
					}
				}
				else if ((pt.value_type == UINT32_BS) || (pt.value_type == FLOAT_BS))
				{
					if (pt.mbs_addr + 1 > MBS_LARGEST_ADDR)
						return;
					if (pt.mbs_addr_ref == REF_3X)
					{
						CMS_UINT16 high_val = (CMS_UINT16)(pt.union_value.ui_value >> 16);
						CMS_UINT16 low_val = (CMS_UINT16)(pt.union_value.ui_value & 0x0000FFFF);
						input_reg_addr_space_->set_point_low(pt.mbs_addr, low_val >> 8);
						input_reg_addr_space_->set_point_high(pt.mbs_addr, low_val & 0xFF);
						input_reg_addr_space_->set_point_low(pt.mbs_addr+1, high_val >> 8);
						input_reg_addr_space_->set_point_high(pt.mbs_addr+1, high_val & 0xFF);
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						CMS_UINT16 high_val = (CMS_UINT16)(pt.union_value.ui_value >> 16);
						CMS_UINT16 low_val = (CMS_UINT16)(pt.union_value.ui_value & 0x0000FFFF);
						holding_reg_addr_space_->set_point_low(pt.mbs_addr, low_val >> 8);
						holding_reg_addr_space_->set_point_high(pt.mbs_addr, low_val & 0xFF);
						holding_reg_addr_space_->set_point_low(pt.mbs_addr+1, high_val >> 8);
						holding_reg_addr_space_->set_point_high(pt.mbs_addr+1, high_val & 0xFF);
					}
				}
				else if ((pt.value_type == UINT32_WS_BS) || (pt.value_type == FLOAT_WS_BS))
				{
					if (pt.mbs_addr + 1 > MBS_LARGEST_ADDR)
						return;
					if (pt.mbs_addr_ref == REF_3X)
					{
						CMS_UINT16 low_val = (CMS_UINT16)(pt.union_value.ui_value >> 16);
						CMS_UINT16 high_val = (CMS_UINT16)(pt.union_value.ui_value & 0x0000FFFF);
						input_reg_addr_space_->set_point_low(pt.mbs_addr, low_val >> 8);
						input_reg_addr_space_->set_point_high(pt.mbs_addr, low_val & 0xFF);
						input_reg_addr_space_->set_point_low(pt.mbs_addr+1, high_val >> 8);
						input_reg_addr_space_->set_point_high(pt.mbs_addr+1, high_val & 0xFF);
					}
					else if (pt.mbs_addr_ref == REF_4X)
					{
						CMS_UINT16 low_val = (CMS_UINT16)(pt.union_value.ui_value >> 16);
						CMS_UINT16 high_val = (CMS_UINT16)(pt.union_value.ui_value & 0x0000FFFF);
						holding_reg_addr_space_->set_point_low(pt.mbs_addr, low_val >> 8);
						holding_reg_addr_space_->set_point_high(pt.mbs_addr, low_val & 0xFF);
						holding_reg_addr_space_->set_point_low(pt.mbs_addr+1, high_val >> 8);
						holding_reg_addr_space_->set_point_high(pt.mbs_addr+1, high_val & 0xFF);
					}
				}
				else if (pt.value_type == BOOLEAN)
				{
					if (pt.mbs_addr_ref == REF_0X)
					{
						coil_addr_space_->set_bit_data(pt.mbs_addr, pt.union_value.b_value);
					}
					else if (pt.mbs_addr_ref == REF_1X)
					{
						dis_input_addr_space_->set_bit_data(pt.mbs_addr, pt.union_value.b_value);
					}
				}
			}
		}
		catch (const exception& ex)
		{
			cerr << ex.what();
		}
	}

	Points_List_Model::~Points_List_Model()
	{
		//cout << "Points_List_Model deleted!" << endl;
	}
}