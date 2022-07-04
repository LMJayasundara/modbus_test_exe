/* Points_List_Widget Header.
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

#ifndef CMS_POINTS_WIDGET_H
#define CMS_POINTS_WIDGET_H

#include <boost/shared_ptr.hpp>
#include <vector>

#include <QWidget>
#include <QPointer>

#include "point/point_record.h"

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QSpinBox;
class QLineEdit;
class QPushButton;

namespace cms
{
	class Addr_Space;
	class CMS_Config_DB;
	class Points_List_View;
	class Points_List_Model;
	class Pt_Spin_Box_Delegate;
	class Mbs_Addr_Type_Delegate;
	class IO_Type_Delegate;
	class Value_Type_Delegate;
	class Pt_Value_Delegate;

	class Points_List_Widget : public QWidget
	{
    Q_OBJECT
	public:
		explicit Points_List_Widget(boost::shared_ptr<Addr_Space> coil, boost::shared_ptr<Addr_Space> dis_input,
			boost::shared_ptr<Addr_Space> input_reg, boost::shared_ptr<Addr_Space> holding_reg,
			QWidget *parent=0);
		~Points_List_Widget();

		void init();

		public slots:
			void goto_row(int row_number);

			void load_points_list_file();

			void load_points_list_file(const std::string& pts_list_file_name);

			void load_recent_points_list();

			void save_points_list_file();

			void save_points_list_file(const std::string& pts_list_file_name);

			void update_points_value();

	protected:
		void closeEvent(QCloseEvent *event);

	private:
		void setup_ui();

		QPointer<QVBoxLayout> main_layout_;
		QPointer<QHBoxLayout> master_config_load_layout_;
		QPointer<QHBoxLayout> goto_row_layout_;

		QPointer<QLabel> points_list_file_lbl_;
		QPointer<QLineEdit> points_list_file_txt_;
		QPointer<QPushButton> points_list_load_btn_;
		QPointer<QPushButton> points_save_btn_;

		QPointer<QLabel> goto_row_lbl_;
		QPointer<QSpinBox> goto_row_sb_;

		boost::shared_ptr<Points_List_Model> points_list_model_;
		QPointer<Points_List_View> points_list_view_;	
		
		QPointer<Pt_Value_Delegate> pt_value_delegate_;
		QPointer<Value_Type_Delegate> value_type_delegate_;
		QPointer<IO_Type_Delegate> io_type_delegate_;
		QPointer<Mbs_Addr_Type_Delegate> mbs_addr_type_delegate_;
		QPointer<Pt_Spin_Box_Delegate> mbs_addr_delegate_;

		std::vector<Point_Record> points_list_;

		boost::shared_ptr<CMS_Config_DB> config_db_;

		// only holding reference
		boost::shared_ptr<cms::Addr_Space> coil_addr_space_;
		boost::shared_ptr<cms::Addr_Space> dis_input_addr_space_;
		boost::shared_ptr<cms::Addr_Space> input_reg_addr_space_;
		boost::shared_ptr<cms::Addr_Space> holding_reg_addr_space_;
	};
}

#endif 