/* Master_Config_View Header.
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

#ifndef CMS_MASTER_CONFIG_VIEW_H
#define CMS_MASTER_CONFIG_VIEW_H

#include <QTableView>
#include <QString>
#include <QAction>

#include <vector>
#include <boost/shared_ptr.hpp>

class QTimer;

namespace cms
{
    class Master_Config_View : public QTableView
    {
        Q_OBJECT
    public:
        Master_Config_View(QWidget * parent = 0);
        ~Master_Config_View();

		void setup_ui();

        void goto_row(int row_number);

	protected:
		bool edit(const QModelIndex &index, EditTrigger trigger, QEvent *event);

    public slots:
		void update_actions();

        void update_view();

		void inser_row();

		void remove_row();

    protected slots:
        virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

    private:
		QAction *insert_row_action_;
		QAction *delete_row_action_;
        //boost::shared_ptr<QTimer> update_view_timer_;
    };
}

#endif