/* Config_Item_delegate Body.
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
#include "modbus/config/config_item_delegate.h"

#include <QModelIndex>
#include <QMessageBox>
#include <QScrollBar>
#include <QTimer>
#include <QComboBox>

#include <iostream>

using namespace std;
using namespace cms;

namespace cms
{
	Spin_Box_Delegate::Spin_Box_Delegate(int min, int max, int step, QObject *parent)
		: min_(min), max_(max), step_(step), QItemDelegate(parent)
	{
	}

	QWidget *Spin_Box_Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
	{
		QSpinBox *editor = new QSpinBox(parent);
		editor->setMinimum(min_);
		editor->setMaximum(max_);
		editor->setSingleStep(step_);

		return editor;
	}

	void Spin_Box_Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		int value = index.model()->data(index, Qt::EditRole).toInt();
	
		QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
		spinBox->setValue(value);
	}

	void Spin_Box_Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
	{
		QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
		spinBox->interpretText();
		int value = spinBox->value();

		model->setData(index, value, Qt::EditRole);
	}

	void Spin_Box_Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
	{
		editor->setGeometry(option.rect);
	}

/***************************************************************************************************************/
	Combo_Box_Delegate::Combo_Box_Delegate(QObject *parent)
		: QItemDelegate(parent)
	{
	}

	QWidget *Combo_Box_Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex & index) const
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

	void Combo_Box_Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
	{
		QString value = index.model()->data(index, Qt::EditRole).toString();

		QComboBox *combo_box = static_cast<QComboBox*>(editor);
		combo_box->setEditText(value);
	}

	void Combo_Box_Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
	{
		QComboBox *combo_box = static_cast<QComboBox*>(editor);
		QString value = combo_box->currentText();
		unsigned int i_value = convert_str_to_function_code(value.toLocal8Bit().data());
		model->setData(index, i_value, Qt::EditRole);
	}

	void Combo_Box_Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
	{
		editor->setGeometry(option.rect);
	}
}