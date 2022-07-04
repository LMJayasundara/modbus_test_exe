/* Master_Config_Widget Header.
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
along with TurbineTools; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#ifndef CMS_MASTER_CONFIG_WIDGET_H
#define CMS_MASTER_CONFIG_WIDGET_H

#include <boost/shared_ptr.hpp>
#include <vector>

#include <QWidget>
#include <QPointer>

#include "modbus/config/mbs_def.h"
#include "modbus/config/master_config.h"

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QSpinBox;
class QLineEdit;
class QPushButton;

namespace cms
{
    class CMS_Config_DB;
	class Spin_Box_Delegate;
	class Combo_Box_Delegate;
	class Master_Config_Model;
	class Master_Config_View;

	class Master_Config_Widget : public QWidget
	{
	Q_OBJECT
	public:
		explicit Master_Config_Widget(cms::MBS_STATION_TYPE mbs_type, QWidget *parent=0);
		~Master_Config_Widget();

        void init();

        void set_mbs_type(cms::MBS_STATION_TYPE mbs_type);

	public slots:
        void goto_row(int row_number);

        void load_master_config_file();

		void load_master_config_file(const std::string& config_file_name);

		void load_recent_master_config();

	    void save_master_config_file();

		void save_master_config_file(const std::string& config_file_name);

    protected:
        void closeEvent(QCloseEvent *event);

	private:
		void setup_ui();

		boost::shared_ptr<Master_Config_Model> master_config_model_;
		QPointer<Master_Config_View> master_config_view_;

		QPointer<QVBoxLayout> main_layout_;
        QPointer<QHBoxLayout> master_config_load_layout_;
        QPointer<QHBoxLayout> goto_row_layout_;

        QPointer<QLabel> master_config_file_lbl_;
        QPointer<QLineEdit> master_config_file_txt_;
        QPointer<QPushButton> master_config_load_btn_;
        QPointer<QPushButton> master_config_save_btn_;

        QPointer<QLabel> goto_row_lbl_;
        QPointer<QSpinBox> goto_row_sb_;
    
		QPointer<Spin_Box_Delegate> slave_addr_delegate_;
		QPointer<Combo_Box_Delegate> combo_box_delegate_;
		QPointer<Spin_Box_Delegate> start_addr_delegate_;
		QPointer<Spin_Box_Delegate> pt_count_delegate_;
		QPointer<Spin_Box_Delegate> retries_delegate_;
		QPointer<Spin_Box_Delegate> timeout_delegate_;

        cms::MBS_STATION_TYPE mbs_type_;

		std::vector<Master_Config_Record> master_configuration_;

        boost::shared_ptr<CMS_Config_DB> config_db_;
	};
}

#endif 