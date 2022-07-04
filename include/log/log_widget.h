/* Log_Widget Header.
Copyright (C) 2010
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

#ifndef CMS_LOG_WIDGET_H
#define CMS_LOG_WIDGET_H

#include <QDockWidget>
#include <QPointer>
#include <QString>
#include <QModelIndex>

#include <boost/shared_ptr.hpp>

class QLocalServer;
class QLocalSocket;
class QTableView;
class QVBoxLayout;
class QDockWidget;

namespace cms
{
	class Log_Data_Model;
	
    class Log_Widget : public QDockWidget
    {
        Q_OBJECT
    public:
        Log_Widget();

		void init();

        virtual QSize sizeHint() const;

	public slots:
		void handle_connection();
        void read_data();

        void update_log_list_view();

    private:
        void setup_ui();
    
	protected:
		void closeEvent(QCloseEvent *event);

    private:
        boost::shared_ptr<QLocalServer> local_sever_;

		QTableView *log_list_;
		Log_Data_Model *log_data_model_;
    };
}

#endif