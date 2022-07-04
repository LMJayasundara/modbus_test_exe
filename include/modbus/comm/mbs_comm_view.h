/* Mbs_Comm_View Header.
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

#ifndef CMS_MBS_COMM_VIEW_H
#define CMS_MBS_COMM_VIEW_H

#include <QTableView>
#include <QString>

#include <vector>
#include <boost/shared_ptr.hpp>

class QTimer;

namespace cms
{
	class Mbs_Comm_View : public QTableView
	{
		Q_OBJECT
	public:
		Mbs_Comm_View(QWidget * parent = 0);
		~Mbs_Comm_View();

	public slots:
		void update_view();

	protected slots:
		virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

	private:
		boost::shared_ptr<QTimer> update_view_timer_;
	};
}

#endif