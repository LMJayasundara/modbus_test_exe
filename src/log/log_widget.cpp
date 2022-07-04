/* Log_Widget.
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

#include <vector>
#include <utility>

#include <QLocalServer>
#include <QLocalSocket>
#include <QDataStream>
#include <QTextStream>
#include <QTableView>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QString>
#include <QStringList>
#include <QDateTime>

#include "log/log_widget.h"
#include "log/log_data_model.h"

using namespace std;

namespace cms
{
    Log_Widget::Log_Widget()
    {
        this->setup_ui();        
        this->resize(sizeHint());
        setWindowTitle(tr("OMT-Log")); 
    }

	void Log_Widget::init()
	{
		local_sever_ = boost::shared_ptr<QLocalServer>(new QLocalServer);
		connect(local_sever_.get(), SIGNAL(newConnection()), this, SLOT(handle_connection()));
		local_sever_->listen("OMT-Log");

		log_data_model_ = new Log_Data_Model();
		log_list_->setModel(log_data_model_);
        connect(log_data_model_, SIGNAL(rowsInserted(const QModelIndex&, int, int)),
            this, SLOT(update_log_list_view()));
	}

    QSize Log_Widget::sizeHint() const
    {
		QWidget *parent_widget =  this->parentWidget();
		if (parent_widget != 0)
			return QSize(parent_widget->width(), parent_widget->height()/5);
		else
			return QSize(320, 240);
    }

	void Log_Widget::handle_connection()
	{
        QLocalSocket *msg_rcv_socket = local_sever_->nextPendingConnection(); 
        connect(msg_rcv_socket, SIGNAL(readyRead()), this, SLOT(read_data()));       
	}

    void Log_Widget::read_data()
    {
        QLocalSocket *msg_rcv_socket = static_cast<QLocalSocket *>(sender());
        if (!msg_rcv_socket)
            return;

		QTextStream in(msg_rcv_socket);
		QString read_msg;
		read_msg = in.readAll();

		log_data_model_->append_msg(read_msg);
    }

    void Log_Widget::update_log_list_view()
    {
        log_list_->resizeColumnsToContents();
        log_list_->scrollToBottom();
    }

    void Log_Widget::setup_ui()
    {
		log_list_ = new QTableView();
		this->setWidget(log_list_);
    }   

	void Log_Widget::closeEvent(QCloseEvent* event)
	{
		this->hide();
	}
}
