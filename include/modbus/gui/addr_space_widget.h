/* Addr_Space_Widget Header.
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

#ifndef CMS_ADDR_SPACE_WIDGET_H
#define CMS_ADDR_SPACE_WIDGET_H

#include <boost/shared_ptr.hpp>

#include <QWidget>
#include <QPointer>

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QSpinBox;

namespace cms
{
	class Addr_Space;
	class Addr_Space_Model;
	class Addr_Space_View;

	class Addr_Space_Widget : public QWidget
	{
	Q_OBJECT
	public:
		Addr_Space_Widget(boost::shared_ptr<Addr_Space> coil, boost::shared_ptr<Addr_Space> dis_input,
			boost::shared_ptr<Addr_Space> input_reg, boost::shared_ptr<Addr_Space> holding_reg, 
			QWidget *parent=0);
		~Addr_Space_Widget();

		void init();

		boost::shared_ptr<Addr_Space> get_coil_addr_space() const;
		boost::shared_ptr<Addr_Space> get_dis_input_addr_space() const;
		boost::shared_ptr<Addr_Space> get_input_reg_addr_space() const;
		boost::shared_ptr<Addr_Space> get_holding_reg_addr_space() const;

	public slots:
        void goto_row(int row_number);

    protected:
        void closeEvent(QCloseEvent *event);

	private:
		void setup_ui();

		// only holding reference
		boost::shared_ptr<Addr_Space> coil_addr_space_;
		boost::shared_ptr<Addr_Space> dis_input_addr_space_;
		boost::shared_ptr<Addr_Space> input_reg_addr_space_;
		boost::shared_ptr<Addr_Space> holding_reg_addr_space_;

		boost::shared_ptr<Addr_Space_Model> addr_space_model_;
		QPointer<Addr_Space_View> addr_space_view_;

		QPointer<QVBoxLayout> main_layout_;
        QPointer<QHBoxLayout> goto_row_layout_;

        QPointer<QLabel> goto_row_lbl_;
        QPointer<QSpinBox> goto_row_sb_;
	};
}

#endif 