/* Master_Config_View Body.
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
#include "modbus/config/master_config_view.h"

#include <QModelIndex>
#include <QMessageBox>
#include <QScrollBar>
#include <QTimer>

using namespace std;
using namespace cms;

namespace cms
{
	Master_Config_View::Master_Config_View(QWidget *parent)
		: QTableView(parent)
	{
		this->setup_ui();
	}

	void Master_Config_View::setup_ui()
	{
		insert_row_action_ = new QAction(tr("inser row"), this);
		delete_row_action_ = new QAction(tr("delete row"), this);
		connect(insert_row_action_, SIGNAL(triggered()), this, SLOT(inser_row()));
		connect(delete_row_action_, SIGNAL(triggered()), this, SLOT(remove_row()));
		this->addAction(insert_row_action_);
		this->addAction(delete_row_action_);
		this->setContextMenuPolicy(Qt::ActionsContextMenu);
		this->setSelectionBehavior(QAbstractItemView::SelectRows);
		update_actions();
		connect(this, SIGNAL(clicked(const QModelIndex &)), this, SLOT(update_actions()));
	}

	void Master_Config_View::goto_row(int row_number)
	{
		this->verticalScrollBar()->setSliderPosition(row_number);
	}

	void Master_Config_View::update_actions()
	{
		bool enable_insert = qobject_cast<QStandardItemModel *>(this->model());
		bool enable_deletel = enable_insert && this->currentIndex().isValid();

		insert_row_action_->setEnabled(enable_insert);
		delete_row_action_->setEnabled(enable_deletel);
	}

	bool Master_Config_View::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event)
	{
		return QTableView::edit(index, trigger, event);
	}

	void Master_Config_View::update_view()
	{
		viewport()->update();
	}

	void Master_Config_View::inser_row()
	{
		Master_Config_Model *model = qobject_cast<Master_Config_Model *>(this->model());
		if (!model)
			return;

		QModelIndex insert_index = this->currentIndex();
		int row = insert_index.row() == -1 ? 0 : insert_index.row();
		model->insert_row(row);
		this->setCurrentIndex(insert_index);
	}

	void Master_Config_View::remove_row()
	{
		Master_Config_Model *model = qobject_cast<Master_Config_Model *>(this->model());
		if (!model)
			return;

		QModelIndexList cur_selection = this->selectionModel()->selectedIndexes();
		for (int i = 0; i < cur_selection.count(); ++i) 
		{
			// only remove rows
			if (cur_selection.at(i).column() != 0)
				continue;
			model->remove_row(cur_selection.at(i).row());
		}

		update_actions();
	}

	void Master_Config_View::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
	{
		QTableView::dataChanged(topLeft, bottomRight);
		viewport()->update();
		update_actions();
	}

	Master_Config_View::~Master_Config_View()
	{
	}
}