/* Main Window Header.
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

#ifndef CMS_MAIN_WINDOW_H
#define CMS_MAIN_WINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSystemTrayIcon>

#include <boost/shared_ptr.hpp>

class QWorkspace;
class QSignalMapper;
class QToolBar;
class QToolButton;
class QSplitter;
class QDockWidget;
class QMdiArea;
class QLineEdit;
class QComboBox;
class QFontComboBox;
class QFont;
class QMenu;

namespace cms
{
	class Addr_Space;
	class Mbs_Widget;
    class Log_Widget;
    class Serial_Test_Widget;
	class Points_List_Widget;
	class Hex_Convertor_Widget;

    class Main_Window : public QMainWindow
    {
        Q_OBJECT
    public:
        Main_Window();
        ~Main_Window();

        virtual void setVisible(bool visible);
        virtual QSize sizeHint() const;

    private:
        QWidget* active_mdi_child();
      
    private slots:
		void init_addr_space();

        // Communication menu function
		void show_mbs_widget();
		void show_mbs_widget_with_serial_master();
		void show_mbs_widget_with_serial_slave();
		void show_mbs_widget_with_tcp_master();
		void show_mbs_widget_with_tcp_slave();

		// Points menu function
		void show_points_list_widget();

        // Tools menu function
        void show_log_widget();
        void show_serial_test_tool_widget();
		void show_hex_convertor_widget();

        void load_recent_file();
        void set_current_file(const QString &fileName);
        void update_recent_file_actions();
        
        void update_window_menus();
        void update_menus();

        void about();

        void set_active_subwindow(QWidget *window);

        void iconActivated(QSystemTrayIcon::ActivationReason reason);

    protected:
        void closeEvent(QCloseEvent *event);
        
    private:
        QWorkspace* workspace_;
        QSignalMapper* signal_mapper_;
        QMdiArea* mdi_area_;
        
        // Main Window Dock Widget
		Mbs_Widget* mbs_widget_;
        Log_Widget* log_widget_;
        Serial_Test_Widget *serial_test_widget_;
		Points_List_Widget *points_list_widget_;
		Hex_Convertor_Widget *hex_convertor_widget_;

        QString current_file_;
        
        // Main Window ToolBars
        QToolBar* main_toolbar_;
        QToolButton* mbs_serial_master_btn_;
        QToolButton* mbs_serial_slave_btn_;
        QToolButton* mbs_tcp_master_btn_;
        QToolButton* mbs_tcp_slave_btn_;
		QToolButton* points_list_btn_;
        QToolButton* log_view_btn_;
		QToolButton* serial_test_btn_;

        // Main Window Menus
        QMenu* file_menu_;
        QMenu* comm_menu_;     
		QMenu* points_menu_;
        QMenu* tools_menu_;
        QMenu* window_menu_;
        QMenu* help_menu_;
        
        // Main Window Actions
        QAction* print_action_;
        QAction* print_preview_action_;

        enum { MaxRecentFiles = 12 };
        QAction* recent_file_acts_[MaxRecentFiles];
        QAction* file_separator_action_;

        QAction* exit_action_;
     
		QAction* mbs_serial_master_action_;
		QAction* mbs_serial_slave_action_;
		QAction* mbs_tcp_master_action_;
		QAction* mbs_tcp_slave_action_;

		QAction* points_list_action_;

        QAction* serial_test_action_;

		QAction* hex_convertor_action_;

        QAction* about_action_;

        QAction* close_action_;
        QAction* close_all_action_;
        QAction* tile_action_;
        QAction* cascade_action_;
        QAction* next_action_;
        QAction* previous_action_;
        QAction* separator_action_;
        QAction *minimize_action_;
        QAction *maximize_action_;
        QAction *restore_action_;
        QAction *quit_action_;

        QSystemTrayIcon* cms_tray_icon_;
        QMenu* cms_tray_icon_menu_;

		boost::shared_ptr<cms::Addr_Space> coil_addr_space_;
		boost::shared_ptr<cms::Addr_Space> dis_input_addr_space_;
		boost::shared_ptr<cms::Addr_Space> input_reg_addr_space_;
		boost::shared_ptr<cms::Addr_Space> holding_reg_addr_space_;

        void create_actions();
        void create_menus();
        void create_toolbars();
        void create_dock_windows();
        void create_statusbar();       
        void create_tray_icon();
    };
}

#endif
