/* Point_Item_delegate Body.
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

#include "modbus/config/mbs_def.h"
#include "point/point_item_delegate.h"

#include <QModelIndex>
#include <QMessageBox>
#include <QScrollBar>
#include <QTimer>
#include <QComboBox>
#include <QLineEdit>

#include <iostream>

using namespace std;
using namespace cms;

namespace cms
{
	/**************************Pt_Spin_Box_Delegate*****************************************************/
	Pt_Spin_Box_Delegate::Pt_Spin_Box_Delegate(int min, int max, int step, QObject *parent)
		: min_(min), max_(max), step_(step), QItemDelegate(parent)
	{
	}

	QWidget *Pt_Spin_Box_Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
	{
		QSpinBox *editor = new QSpinBox(parent);
		editor->setMinimum(min_);
		editor->setMaximum(max_);
		editor->setSingleStep(step_);

		return editor;
	}

	void Pt_Spin_Box_Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		int value = index.model()->data(index, Qt::EditRole).toInt();
	
		QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
		spinBox->setValue(value);
	}

	void Pt_Spin_Box_Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
	{
		QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
		spinBox->interpretText();
		int value = spinBox->value();

		model->setData(index, value, Qt::EditRole);
	}

	void Pt_Spin_Box_Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
	{
		editor->setGeometry(option.rect);
	}

	/**************************Pt_Value_Delegate*****************************************************/
	Pt_Value_Delegate::Pt_Value_Delegate(QObject *parent)
		: QItemDelegate(parent)
	{
	}

	QWidget *Pt_Value_Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
	{
		QLineEdit *line_edit = new QLineEdit(parent);

		return line_edit;
	}

	void Pt_Value_Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		QString value_str = index.model()->data(index, Qt::EditRole).toString();

		QLineEdit *line_edit = static_cast<QLineEdit*>(editor);
		line_edit->setText(value_str);
	}

	void Pt_Value_Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
	{
		QLineEdit *line_edit = static_cast<QLineEdit*>(editor);
		QString value_str = line_edit->text();

		model->setData(index, value_str, Qt::EditRole);
	}

	void Pt_Value_Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
	{
		editor->setGeometry(option.rect);
	}

    /********************************Pt_Combo_Box_Delegate*************************************************************/
	Pt_Combo_Box_Delegate::Pt_Combo_Box_Delegate(QObject *parent)
		: QItemDelegate(parent)
	{
	}

	QWidget *Pt_Combo_Box_Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex & index) const
	{
		QComboBox *editor = new QComboBox(parent);
		editor->addItem("RCS");
		editor->addItem("RIS");
		editor->addItem("RHR");
		editor->addItem("RIR");
		editor->addItem("FSC");
		editor->addItem("PSR");
		editor->addItem("FMC");
		editor->addItem("PMR");
		editor->setEditText(index.model()->data(index, Qt::EditRole).toString());
		return editor;
	}

	void Pt_Combo_Box_Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		QString value = index.model()->data(index, Qt::EditRole).toString();

		QComboBox *combo_box = static_cast<QComboBox*>(editor);
		combo_box->setEditText(value);
	}

	void Pt_Combo_Box_Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
	{
		QComboBox *combo_box = static_cast<QComboBox*>(editor);
		QString value = combo_box->currentText();
		unsigned int i_value = convert_str_to_function_code(value.toLocal8Bit().data());
		model->setData(index, i_value, Qt::EditRole);
	}

	void Pt_Combo_Box_Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
	{
		editor->setGeometry(option.rect);
	}

	/***************************************************************************************************************/
	Mbs_Addr_Type_Delegate::Mbs_Addr_Type_Delegate(QObject *parent)
		: QItemDelegate(parent)
	{
	}

	QWidget *Mbs_Addr_Type_Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex & index) const
	{
		QComboBox *editor = new QComboBox(parent);
		editor->addItem("0X Coil Status");
		editor->addItem("1X Input Status");
		editor->addItem("3X Input Register");
		editor->addItem("4X Holding Register");
		editor->setEditText(index.model()->data(index, Qt::EditRole).toString());
		return editor;
	}

	void Mbs_Addr_Type_Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		QString value = index.model()->data(index, Qt::EditRole).toString();

		QComboBox *combo_box = static_cast<QComboBox*>(editor);
		combo_box->setEditText(value);
	}

	void Mbs_Addr_Type_Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
	{
		QComboBox *combo_box = static_cast<QComboBox*>(editor);
		QString value = combo_box->currentText();
		unsigned int i_value = convert_str_to_mbs_addr_type(value.toLocal8Bit().data());
		model->setData(index, i_value, Qt::EditRole);
	}

	void Mbs_Addr_Type_Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
	{
		editor->setGeometry(option.rect);
	}

	/***************************************************************************************************************/
	IO_Type_Delegate::IO_Type_Delegate(QObject *parent)
		: QItemDelegate(parent)
	{
	}

	QWidget *IO_Type_Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex & index) const
	{
		QComboBox *editor = new QComboBox(parent);
		editor->addItem("Input");
		editor->addItem("Output");
		editor->setEditText(index.model()->data(index, Qt::EditRole).toString());
		return editor;
	}

	void IO_Type_Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		QString value = index.model()->data(index, Qt::EditRole).toString();

		QComboBox *combo_box = static_cast<QComboBox*>(editor);
		combo_box->setEditText(value);
	}

	void IO_Type_Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
	{
		QComboBox *combo_box = static_cast<QComboBox*>(editor);
		QString value = combo_box->currentText();
		unsigned int i_value = convert_str_to_io_type(value.toLocal8Bit().data());
		model->setData(index, i_value, Qt::EditRole);
	}

	void IO_Type_Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
	{
		editor->setGeometry(option.rect);
	}

	/***************************************************************************************************************/
	Value_Type_Delegate::Value_Type_Delegate(QObject *parent)
		: QItemDelegate(parent)
	{
	}

	QWidget *Value_Type_Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex & index) const
	{
		QComboBox *editor = new QComboBox(parent);
		// 0 - int16, 1 -  uint16, 2 - int32, 3 - int32rev, 4 - int32_bs, 5 - int32_ws_bs,
		// 6 - uint32, 7 - uint32rev, 8 - uint32_bs, 9 - uint32_ws_bs, 10 - float,
		// 11 - floatrev, 12 - float_bs, 13 - floot_ws_bs, 14 - boolean
		editor->addItem("INT16"); editor->addItem("UINT16"); editor->addItem("INT32");
		editor->addItem("INT32REV"); editor->addItem("INT32_BS"); editor->addItem("INT32_WS_BS");
		editor->addItem("UINT32"); editor->addItem("UINT32REV"); editor->addItem("UINT32_BS");
		editor->addItem("UINT32_WS_BS"); editor->addItem("FLOAT"); editor->addItem("FLOATREV");
		editor->addItem("FLOAT_BS"); editor->addItem("FLOAT_WS_BS"); editor->addItem("BOOLEAN");
		editor->setEditText(index.model()->data(index, Qt::EditRole).toString());
		return editor;
	}

	void Value_Type_Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		QString value = index.model()->data(index, Qt::EditRole).toString();

		QComboBox *combo_box = static_cast<QComboBox*>(editor);
		combo_box->setEditText(value);
	}

	void Value_Type_Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
	{
		QComboBox *combo_box = static_cast<QComboBox*>(editor);
		QString value = combo_box->currentText();
		unsigned int i_value = convert_str_to_value_type(value.toLocal8Bit().data());
		model->setData(index, i_value, Qt::EditRole);
	}

	void Value_Type_Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
	{
		editor->setGeometry(option.rect);
	}
}