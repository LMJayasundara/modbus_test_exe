/* Main Window.
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
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,w
Boston, MA 02110-1301, USA.  */

#include <QFileInfo>
#include <QIcon>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QDockWidget>
#include <QMdiSubWindow>
#include <QList>
#include <QProgressDialog>
#include <QMessageBox>
#include <QMdiArea>
#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QToolButton>
#include <QStatusBar>
#include <QApplication>
#include <QFontComboBox>
#include <QSignalMapper>
#include <QSettings>
#include <QProcessEnvironment>
#include <QSystemTrayIcon>
#include <QCloseEvent>

#include <fstream>
#include <exception>

#include <memory>

#include "main_window.h"
#include "modbus/gui/mbs_widget.h"
#include "log/log_widget.h"
#include "tools/serial_test/serial_test_widget.h"
#include "tools/hex_convertor_widget.h"
#include "point/points_list_widget.h"
#include "modbus/address/addr_space.h"

namespace cms
{
    // public method
    Main_Window::Main_Window()
    {
		this->init_addr_space();

        signal_mapper_ = new QSignalMapper(this);

        mdi_area_ = new QMdiArea();
        mdi_area_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        mdi_area_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        this->setCentralWidget(mdi_area_);

        connect(mdi_area_, SIGNAL(subWindowActivated(QMdiSubWindow* )), this, SLOT(update_menus()));

        connect(signal_mapper_, SIGNAL(mapped(QWidget* )), this, SLOT(set_active_subwindow(QWidget* )));

        this->setWindowTitle(tr("SmartDCS.CommServer"));
        this->resize(sizeHint());
        this->move(0, 0);

        this->create_actions();
        this->create_menus();
        this->create_toolbars();
        this->create_dock_windows();
        this->create_statusbar();
        this->create_tray_icon();

        cms_tray_icon_->show();

        QIcon appIcon(QLatin1String(":/cms_rc/images/cms.ico"));
        qApp->setWindowIcon(appIcon);

		this->show_points_list_widget();
    }

	void Main_Window::init_addr_space()
	{
		coil_addr_space_ = boost::shared_ptr<Addr_Space>(new Addr_Space(REF_0X));
		coil_addr_space_->fill_zero_data();
		dis_input_addr_space_ = boost::shared_ptr<Addr_Space>(new Addr_Space(REF_1X));
		dis_input_addr_space_->fill_zero_data();
		input_reg_addr_space_ = boost::shared_ptr<Addr_Space>(new Addr_Space(REF_3X));
		input_reg_addr_space_->fill_zero_data();
		holding_reg_addr_space_ = boost::shared_ptr<Addr_Space>(new Addr_Space(REF_4X));
		holding_reg_addr_space_->fill_zero_data();
	}

	void Main_Window::show_mbs_widget()
	{
        /*this should be removed*/
		mbs_widget_ = new Mbs_Widget(coil_addr_space_, dis_input_addr_space_,
			input_reg_addr_space_, holding_reg_addr_space_,
			MBS_SERIAL_MASTER, this);
		this->mdi_area_->addSubWindow(mbs_widget_);
		mbs_widget_->showNormal();		
	}

	void Main_Window::show_mbs_widget_with_serial_master()
	{
		mbs_widget_ = new Mbs_Widget(coil_addr_space_, dis_input_addr_space_,
			input_reg_addr_space_, holding_reg_addr_space_,
			cms::MBS_SERIAL_MASTER, this);
        this->mbs_widget_->set_mbs_type(MBS_SERIAL_MASTER);
		this->mdi_area_->addSubWindow(mbs_widget_);
		mbs_widget_->showNormal();		
	}

	void Main_Window::show_mbs_widget_with_serial_slave()
	{
		mbs_widget_ = new Mbs_Widget(coil_addr_space_, dis_input_addr_space_,
			input_reg_addr_space_, holding_reg_addr_space_, 
			MBS_SERIAL_SLAVE, this);
        this->mbs_widget_->set_mbs_type(MBS_SERIAL_SLAVE);
		this->mdi_area_->addSubWindow(mbs_widget_);
		mbs_widget_->showNormal();		
	}

	void Main_Window::show_mbs_widget_with_tcp_master()
	{
		mbs_widget_ = new Mbs_Widget(coil_addr_space_, dis_input_addr_space_,
			input_reg_addr_space_, holding_reg_addr_space_,
			MBS_TCP_MASTER, this);
        this->mbs_widget_->set_mbs_type(MBS_TCP_MASTER);
		this->mdi_area_->addSubWindow(mbs_widget_);
		mbs_widget_->showNormal();		
	}

	void Main_Window::show_mbs_widget_with_tcp_slave()
	{
		mbs_widget_ = new Mbs_Widget(coil_addr_space_, dis_input_addr_space_,
			input_reg_addr_space_, holding_reg_addr_space_,
			MBS_TCP_SLAVE, this);
        this->mbs_widget_->set_mbs_type(MBS_TCP_SLAVE);
		this->mdi_area_->addSubWindow(mbs_widget_);
		mbs_widget_->showNormal();		
	}

	void Main_Window::show_points_list_widget()
	{
		points_list_widget_ = new Points_List_Widget(coil_addr_space_, dis_input_addr_space_,
			input_reg_addr_space_, holding_reg_addr_space_, this);
		this->mdi_area_->addSubWindow(points_list_widget_);
		points_list_widget_->showMaximized();
	}

    void Main_Window::show_log_widget()
    {
        if (log_widget_)
        {
            log_widget_->setAllowedAreas(Qt::BottomDockWidgetArea);
            this->addDockWidget(Qt::BottomDockWidgetArea, log_widget_);
			log_widget_->show();
        }
    }

    void Main_Window::show_serial_test_tool_widget()
    {
        serial_test_widget_ = new Serial_Test_Widget();
        this->mdi_area_->addSubWindow(serial_test_widget_);
        serial_test_widget_->showNormal();	
    }

	void Main_Window::show_hex_convertor_widget()
	{
		hex_convertor_widget_ = new Hex_Convertor_Widget();
		this->mdi_area_->addSubWindow(hex_convertor_widget_);
		hex_convertor_widget_->showNormal();	
	}

    void Main_Window::setVisible(bool visible)
    {
        minimize_action_->setEnabled(visible);
        maximize_action_->setEnabled(!isMaximized());
        restore_action_->setEnabled(isMaximized() || !visible);
        QMainWindow::setVisible(visible);
    }

    QSize Main_Window::sizeHint() const
    {
        return QSize(1000, 700);
    }

    void Main_Window::load_recent_file()
    {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
        {
            QFileInfo file_info(action->data().toString());
            QString file_type = file_info.suffix();

            if (file_type.toLower() == "svg")
            {
                QString file_path = file_info.path().append("/");
            }
            else if(file_type.toLower() == "src")
            {
                QString file_path = file_info.path().append("/");
            }
            else
            {
                QMessageBox::about(this, tr("Error"), tr("Unsupported file type!"));
            }
        }

    }

    void Main_Window::set_current_file(const QString &file_name)
    {
        current_file_ = file_name;
        if (current_file_.isEmpty())
        {
            setWindowTitle(tr("Recent Files"));
        }
        else
        {
            setWindowTitle(tr("%1 - %2").arg(current_file_).arg(tr("Recent Files")));
        }

        QSettings settings("Newsuppy", "CommunicationServer");
        QStringList files = settings.value("recentFileList").toStringList();
        files.removeAll(file_name);
        files.prepend(file_name);
        while (files.size() > MaxRecentFiles)
        {
            files.removeLast();
        }

        settings.setValue("recentFileList", files);

        // Update recent file list immediately
        foreach (QWidget *widget, QApplication::topLevelWidgets()) 
        {
            Main_Window *mainWin = qobject_cast<Main_Window *>(widget);
            if (mainWin)
            {
                mainWin->update_recent_file_actions();
            }
        }
    }

    void Main_Window::update_recent_file_actions()
    {
        QSettings settings("Newsuppy", "CommunicationServer");
        QStringList files = settings.value("recentFileList").toStringList();

        int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

        for (int i = 0; i < numRecentFiles; ++i) {
            QString text = tr("&%1 %2").arg(i + 1).arg(files[i]);
            recent_file_acts_[i]->setText(text);
            recent_file_acts_[i]->setData(files[i]);
            recent_file_acts_[i]->setVisible(true);
        }
        for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
            recent_file_acts_[j]->setVisible(false);

        file_separator_action_->setVisible(numRecentFiles > 0);
    }

    void Main_Window::about()
    {
        QMessageBox::about(this, tr("About Communication"),
            tr("presented by newsuppy, version 1.10\ne-mail:newsuppy@outlook.com"));
    }

    void Main_Window::set_active_subwindow(QWidget *window)
    {
        if (!window)
            return;

        mdi_area_->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
    }

    void Main_Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
    {
        switch (reason) 
        {
        case QSystemTrayIcon::DoubleClick:
            this->showNormal();
            break;
        default:
            ;
        }
    }

    // protected method
    void Main_Window::closeEvent(QCloseEvent *event)
    {
        if (cms_tray_icon_->isVisible()) 
        {
            //QMessageBox::information(this, tr("Systray"), tr("The program will keep running in the system tray. To terminate the program, "
            //"choose <b>Quit</b> in the context menu of the system tray entry."));
            QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
            cms_tray_icon_->showMessage("Tools remain in tray", "Wait for your command", icon);

            this->hide();
            event->ignore();
        }
    }

    // private method
    void Main_Window::create_actions()
    {
        // File menu action
        for (int i = 0; i < MaxRecentFiles; ++i) 
        {
            recent_file_acts_[i] = new QAction(this);
            recent_file_acts_[i]->setVisible(false);
            connect(recent_file_acts_[i], SIGNAL(triggered()),
                this, SLOT(load_recent_file()));
        }

        this->exit_action_ = new QAction(tr("E&xit"), this);
        this->exit_action_ ->setShortcut(QKeySequence("CTRL+Q"));
        this->exit_action_->setStatusTip(tr("Exit the application"));
        connect(exit_action_, SIGNAL(triggered()), qApp, SLOT(quit()));

        // Window menu action
        close_action_ = new QAction(tr("Cl&ose"), this);
        close_action_->setShortcut(tr("Ctrl+F4"));
        close_action_->setStatusTip(tr("Close the active window"));
        connect(close_action_, SIGNAL(triggered()), mdi_area_, SLOT(closeActiveSubWindow()));

        close_all_action_ = new QAction(tr("Close &All"), this);
        close_all_action_->setStatusTip(tr("Close all the windows"));
        connect(close_all_action_, SIGNAL(triggered()), mdi_area_, SLOT(closeAllSubWindows()));

        tile_action_ = new QAction(tr("&Tile"), this);
        tile_action_->setStatusTip(tr("Tile the windows"));
        connect(tile_action_, SIGNAL(triggered()), mdi_area_, SLOT(tileSubWindows()));

        cascade_action_ = new QAction(tr("&Cascade"), this);
        cascade_action_->setStatusTip(tr("Cascade the windows"));
        connect(cascade_action_, SIGNAL(triggered()), mdi_area_, SLOT(cascadeSubWindows()));

        next_action_ = new QAction(tr("Ne&xt"), this);
        next_action_->setStatusTip(tr("Move the focus to the next window"));
        connect(next_action_, SIGNAL(triggered()), mdi_area_, SLOT(activateNextSubWindow()));

        previous_action_ = new QAction(tr("Pre&vious"), this);
        previous_action_->setStatusTip(tr("Move the focus to the previous window"));
        connect(previous_action_, SIGNAL(triggered()), mdi_area_, SLOT(activatePreviousSubWindow()));

        separator_action_ = new QAction(this);
        separator_action_->setSeparator(true);

        // Communication menu action
		this->mbs_serial_master_action_ = new QAction(tr("Modbus Serial Master"), this);
		this->mbs_serial_master_action_->setShortcut(QKeySequence("CTRL+ALT+M"));
		this->mbs_serial_master_action_->setStatusTip(tr("Modbus Serial Master"));
		connect(mbs_serial_master_action_, SIGNAL(triggered()), this, SLOT(show_mbs_widget_with_serial_master()));

		this->mbs_serial_slave_action_ = new QAction(tr("Modbus Serial Slave"), this);
		this->mbs_serial_slave_action_->setShortcut(QKeySequence("CTRL+ALT+S"));
		this->mbs_serial_slave_action_->setStatusTip(tr("Modbus Serial Slave"));
		connect(mbs_serial_slave_action_, SIGNAL(triggered()), this, SLOT(show_mbs_widget_with_serial_slave()));

		this->mbs_tcp_master_action_ = new QAction(tr("Modbus TCP Master"), this);
		this->mbs_tcp_master_action_->setShortcut(QKeySequence("CTRL+ALT+T"));
		this->mbs_tcp_master_action_->setStatusTip(tr("Modbus TCP Master"));
		connect(mbs_tcp_master_action_, SIGNAL(triggered()), this, SLOT(show_mbs_widget_with_tcp_master()));

		this->mbs_tcp_slave_action_ = new QAction(tr("Modbus TCP Slave"), this);
		this->mbs_tcp_slave_action_->setShortcut(QKeySequence("CTRL+ALT+V"));
		this->mbs_tcp_slave_action_->setStatusTip(tr("Modbus TCP Slave"));
		connect(mbs_tcp_slave_action_, SIGNAL(triggered()), this, SLOT(show_mbs_widget_with_tcp_slave()));
		
		// Points menu action
		this->points_list_action_ = new QAction(tr("Points List"), this);
		this->points_list_action_->setShortcut(QKeySequence("CTRL+ALT+L"));
		this->points_list_action_->setStatusTip(tr("Points List"));
		connect(points_list_action_, SIGNAL(triggered()), this, SLOT(show_points_list_widget()));

        // Tools menu action
        this->serial_test_action_ = new QAction(tr("Serial Test Tool"), this);
        this->serial_test_action_->setShortcut(QKeySequence("CTRL+ALT+I"));
        this->serial_test_action_->setStatusTip(tr("Serial Test Tool"));
        connect(serial_test_action_, SIGNAL(triggered()), this, SLOT(show_serial_test_tool_widget()));

		this->hex_convertor_action_ = new QAction(tr("Hex Convertor Tool"), this);
		this->hex_convertor_action_->setShortcut(QKeySequence("CTRL+ALT+C"));
		this->hex_convertor_action_->setStatusTip(tr("Hex Convertor Tool"));
		connect(hex_convertor_action_, SIGNAL(triggered()), this, SLOT(show_hex_convertor_widget()));

        // Help menu action
        this->about_action_ = new QAction(tr("About &Communication Server"), this);
        this->about_action_->setStatusTip(tr("About Communication Server"));
        connect(about_action_, SIGNAL(triggered()), this, SLOT(about()));

        // Tray menu action
        minimize_action_ = new QAction(tr("Mi&nimize"), this);
        connect(minimize_action_, SIGNAL(triggered()), this, SLOT(hide()));

        maximize_action_ = new QAction(tr("Ma&ximize"), this);
        connect(maximize_action_, SIGNAL(triggered()), this, SLOT(showMaximized()));

        restore_action_ = new QAction(tr("&Restore"), this);
        connect(restore_action_, SIGNAL(triggered()), this, SLOT(showNormal()));

        quit_action_ = new QAction(tr("&Quit"), this);
        connect(quit_action_, SIGNAL(triggered()), qApp, SLOT(quit()));

    }

    void Main_Window::update_window_menus()
    {
        window_menu_->clear();
        window_menu_->addAction(close_action_);
        window_menu_->addAction(close_all_action_);
        window_menu_->addSeparator();
        window_menu_->addAction(tile_action_);
        window_menu_->addAction(cascade_action_);
        window_menu_->addSeparator();
        window_menu_->addAction(next_action_);
        window_menu_->addAction(previous_action_);
        window_menu_->addAction(separator_action_);

        QList<QMdiSubWindow* > sub_windows = mdi_area_->subWindowList();
        separator_action_->setVisible(!sub_windows.isEmpty());

        for (int i = 0; i < sub_windows.size(); ++i) 
        {
            QWidget* child_widget = sub_windows.at(i)->widget();

            QString text;
            if (i < 9) 
            {
                text = child_widget->windowTitle();//tr("&%1 %2").arg(i + 1).arg(child_widget->windowTitle);
            }
            else
            {
                text = child_widget->windowTitle();//tr("%1 %2").arg(i + 1).arg(child_widget->windowTitle);
            }


            QAction* action  = window_menu_->addAction(text);
            action->setCheckable(true);
            action ->setChecked(child_widget == this->active_mdi_child());
            connect(action, SIGNAL(triggered()), signal_mapper_, SLOT(map()));
            signal_mapper_->setMapping(action, sub_windows.at(i));       
        }
    }

    void Main_Window::update_menus()
    {
        bool has_mdi_child = (this->active_mdi_child() != 0);
        close_action_->setEnabled(has_mdi_child);
        close_all_action_->setEnabled(has_mdi_child);
        tile_action_->setEnabled(has_mdi_child);
        cascade_action_->setEnabled(has_mdi_child);
        next_action_->setEnabled(has_mdi_child);
        previous_action_->setEnabled(has_mdi_child);
        separator_action_->setVisible(has_mdi_child);
    }

    QWidget* Main_Window::active_mdi_child()
    {
        if (QMdiSubWindow* active_sub_window = mdi_area_->activeSubWindow())
            return active_sub_window->widget();

        return 0;
    }

    void Main_Window::create_menus()
    {
        this->file_menu_ = this->menuBar()->addMenu(tr("&File"));
  
        file_separator_action_ = file_menu_->addSeparator();
        for (int i = 0; i < MaxRecentFiles; ++i)
        {
            file_menu_->addAction(recent_file_acts_[i]);
        }

        file_menu_->addSeparator();
        this->file_menu_->addAction(this->exit_action_);	
        update_recent_file_actions();

        this->comm_menu_ = this->menuBar()->addMenu(tr("&Communication"));
		this->comm_menu_->addAction(this->mbs_serial_master_action_);
		this->comm_menu_->addAction(this->mbs_serial_slave_action_);
		this->comm_menu_->addAction(this->mbs_tcp_master_action_);
		this->comm_menu_->addAction(this->mbs_tcp_slave_action_);
		this->comm_menu_->setEnabled(true);

		this->points_menu_ = this->menuBar()->addMenu(tr("&Points"));
		this->points_menu_->addAction(this->points_list_action_);
		this->points_menu_->setEnabled(true);

        this->tools_menu_ = this->menuBar()->addMenu(tr("&Tools"));
        this->tools_menu_->addAction(this->serial_test_action_);
		this->tools_menu_->addAction(this->hex_convertor_action_);

        this->window_menu_ = this->menuBar()->addMenu(tr("&Window"));
        this->update_window_menus();
        connect(window_menu_, SIGNAL(aboutToShow()), this, SLOT(update_window_menus()));

        this->help_menu_ = this->menuBar()->addMenu(tr("&Help"));
        this->help_menu_->addAction(this->about_action_);
    }	

    void Main_Window::create_toolbars()
    {
        main_toolbar_ = this->addToolBar(tr("Main ToolBar"));

        mbs_serial_master_btn_ = new QToolButton();
        mbs_serial_master_btn_->setText(tr("MBS Serial Master"));
        main_toolbar_->addWidget(mbs_serial_master_btn_);
        connect(mbs_serial_master_btn_, SIGNAL(clicked()), this, SLOT(show_mbs_widget_with_serial_master()));

		mbs_serial_slave_btn_ = new QToolButton();
		mbs_serial_slave_btn_->setText(tr("MBS Serial Slave"));
		main_toolbar_->addWidget(mbs_serial_slave_btn_);
		connect(mbs_serial_slave_btn_, SIGNAL(clicked()), this, SLOT(show_mbs_widget_with_serial_slave()));

		mbs_tcp_master_btn_ = new QToolButton();
		mbs_tcp_master_btn_->setText(tr("MBS TCP Master"));
		main_toolbar_->addWidget(mbs_tcp_master_btn_);
		connect(mbs_tcp_master_btn_, SIGNAL(clicked()), this, SLOT(show_mbs_widget_with_tcp_master()));

        mbs_tcp_slave_btn_ = new QToolButton();
        mbs_tcp_slave_btn_->setText(tr("MBS TCP Slave"));
        main_toolbar_->addWidget(mbs_tcp_slave_btn_);
        connect(mbs_tcp_slave_btn_, SIGNAL(clicked()), this, SLOT(show_mbs_widget_with_tcp_slave()));

		points_list_btn_ = new QToolButton();
		points_list_btn_->setText(tr("Points List"));
		main_toolbar_->addWidget(points_list_btn_);
		connect(points_list_btn_, SIGNAL(clicked()), this, SLOT(show_points_list_widget()));

        log_view_btn_ = new QToolButton();
        log_view_btn_->setText(tr("Log"));
        main_toolbar_->addWidget(log_view_btn_);
        connect(log_view_btn_, SIGNAL(clicked()), this, SLOT(show_log_widget()));

		serial_test_btn_ = new QToolButton();
        serial_test_btn_->setText(tr("Serial Test"));
        main_toolbar_->addWidget(serial_test_btn_);
        connect(serial_test_btn_, SIGNAL(clicked()), this, SLOT(show_serial_test_tool_widget()));
    }

    void Main_Window::create_dock_windows()
    {
        log_widget_ = new Log_Widget();
        log_widget_->init();
        show_log_widget();        
    }

    void Main_Window::create_statusbar()
    {
        this->statusBar()->showMessage(tr("Ready"));
    }

    void Main_Window::create_tray_icon()
    {

        cms_tray_icon_menu_ = new QMenu(this);
        cms_tray_icon_menu_->addAction(minimize_action_);
        cms_tray_icon_menu_->addAction(maximize_action_);
        cms_tray_icon_menu_->addAction(restore_action_);
        cms_tray_icon_menu_->addSeparator();
        cms_tray_icon_menu_->addAction(quit_action_);

        cms_tray_icon_ = new QSystemTrayIcon(this);
        cms_tray_icon_->setContextMenu(cms_tray_icon_menu_);
        QIcon tray_icon(QLatin1String("images/cms.png"));
        cms_tray_icon_->setIcon(tray_icon);
		setWindowIcon(tray_icon);

        connect(cms_tray_icon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

		cms_tray_icon_->show();
    }

    Main_Window::~Main_Window()
    {
        for (int wid=0; wid<mdi_area_->subWindowList().count(); ++wid)
        {
            QMdiSubWindow* sub_window = mdi_area_->subWindowList().at(wid);
            sub_window->close();
        }
    }
}
