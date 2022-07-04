/* Mbs_Widget Header.
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
along with TurbineTools; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#ifndef CMS_MBS_WIDGET_H
#define CMS_MBS_WIDGET_H

#include <boost/shared_ptr.hpp>

#include <QWidget>
#include <QPointer>

#include "ui/ui_mbs_widget.h"

#include "modbus/config/mbs_def.h"

namespace cms
{
	class Modbus_Station;
	class Addr_Space_Widget;
    class Master_Config_Widget;
	class Mbs_Comm_Model;
	class Mbs_Comm_View;
    class Mbs_Analyzer;
    class CMS_Config_DB;
}


namespace boost
{
    class thread;
}

namespace cms
{
	class Addr_Space;

	class Mbs_Widget : public QWidget
	{
	Q_OBJECT
	public:
		explicit Mbs_Widget(boost::shared_ptr<Addr_Space> coil, boost::shared_ptr<Addr_Space> dis_input,
			boost::shared_ptr<Addr_Space> input_reg, boost::shared_ptr<Addr_Space> holding_reg,
			MBS_STATION_TYPE mbs_type, QWidget *parent=0);
		~Mbs_Widget();

		void set_mbs_type(cms::MBS_STATION_TYPE mbs_type);
		void set_gui_type(cms::MBS_STATION_TYPE mbs_type);

        void load_recent_comm_config();

	public slots:
		void start_modbus_station();
		void stop_modbus_station();

        void update_comm_config();

        void load_master_cfg_file();

    protected:
        void closeEvent(QCloseEvent *event);

	private:
		void setup_ui();

		Ui::Mbs_Widget ui;

		boost::shared_ptr<cms::Modbus_Station> mbs_station_;
		
		QPointer<Addr_Space_Widget> addr_space_widget_;

        QPointer<Master_Config_Widget> master_config_widget_;

		boost::shared_ptr<cms::Mbs_Comm_Model> mbs_comm_model_;
		QPointer<cms::Mbs_Comm_View> mbs_comm_view_;

		// update address space data display
		long update_interval_;
		//boost::shared_ptr<boost::asio::deadline_timer> update_addr_space_timer_;

		cms::MBS_STATION_TYPE mbs_type_;

        boost::shared_ptr<Mbs_Analyzer> mbs_analyzer_;

        boost::shared_ptr<CMS_Config_DB> config_db_;

        boost::shared_ptr<boost::thread> station_thread_;

        std::string serial_master_cfg_file_name_;
        std::string tcp_master_cft_file_name_;

		// only holding reference
		boost::shared_ptr<cms::Addr_Space> coil_addr_space_;
		boost::shared_ptr<cms::Addr_Space> dis_input_addr_space_;
		boost::shared_ptr<cms::Addr_Space> input_reg_addr_space_;
		boost::shared_ptr<cms::Addr_Space> holding_reg_addr_space_;
	};
}

#endif 
