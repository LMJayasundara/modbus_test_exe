/* Addr_Space_View Body.
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

#include "modbus/address/addr_space_model.h"
#include "modbus/address/addr_space_view.h"

#include <QModelIndex>
#include <QMessageBox>
#include <QScrollBar>
#include <QTimer>

using namespace std;
using namespace cms;

namespace cms
{
	Addr_Space_View::Addr_Space_View(QWidget *parent)
		: QTableView(parent)
	{
		update_view_timer_ = boost::shared_ptr<QTimer>(new QTimer(this));    
		update_view_timer_->start(1000);
		connect(update_view_timer_.get(), SIGNAL(timeout()), this, SLOT(update_view()));
	}

    void Addr_Space_View::goto_row(int row_number)
    {
        this->verticalScrollBar()->setSliderPosition(row_number);
    }

	void Addr_Space_View::update_view()
	{
		viewport()->update();
	}

	void Addr_Space_View::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
	{
		QTableView::dataChanged(topLeft, bottomRight);
		viewport()->update();
	}

	Addr_Space_View::~Addr_Space_View()
	{
		update_view_timer_->stop();
	}
}