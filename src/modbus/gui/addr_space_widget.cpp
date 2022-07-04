/* Addr_Space_Widget Body.
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

#include <string>

#include <boost/thread/thread.hpp>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QMessageBox>

#include "modbus/gui/addr_space_widget.h"
#include "modbus/address/addr_space.h"
#include "modbus/address/addr_space_model.h"
#include "modbus/address/addr_space_view.h"
#include "modbus/config/mbs_def.h"

using namespace std;
using namespace cms;


namespace cms
{
	Addr_Space_Widget::Addr_Space_Widget(boost::shared_ptr<Addr_Space> coil, boost::shared_ptr<Addr_Space> dis_input,
		boost::shared_ptr<Addr_Space> input_reg, boost::shared_ptr<Addr_Space> holding_reg, QWidget *parent)
		: coil_addr_space_(coil), dis_input_addr_space_(dis_input),
		input_reg_addr_space_(input_reg), holding_reg_addr_space_(holding_reg),
		QWidget(parent)
	{
		this->setup_ui();
	}

	void Addr_Space_Widget::init()
	{	
		addr_space_model_ = boost::shared_ptr<Addr_Space_Model>(new Addr_Space_Model(coil_addr_space_, dis_input_addr_space_,
			input_reg_addr_space_, holding_reg_addr_space_));
		addr_space_view_ = new Addr_Space_View(this);
		connect(addr_space_model_.get(), SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), addr_space_view_, SLOT(dataChanged(const QModelIndex &, const QModelIndex &)));

	    addr_space_view_->setModel(addr_space_model_.get());
		main_layout_->addWidget(addr_space_view_);
	}

	boost::shared_ptr<cms::Addr_Space> Addr_Space_Widget::get_coil_addr_space() const
	{
		return coil_addr_space_;
	}

	boost::shared_ptr<cms::Addr_Space> Addr_Space_Widget::get_dis_input_addr_space() const
	{
		return dis_input_addr_space_;
	}

	boost::shared_ptr<cms::Addr_Space> Addr_Space_Widget::get_input_reg_addr_space() const
	{
		return input_reg_addr_space_;
	}
	
	boost::shared_ptr<cms::Addr_Space> Addr_Space_Widget::get_holding_reg_addr_space() const
	{
		return holding_reg_addr_space_;
	}

	void Addr_Space_Widget::setup_ui()
	{
		main_layout_ = new QVBoxLayout(this);
		this->setLayout(main_layout_);
        goto_row_layout_ = new QHBoxLayout();
        main_layout_->addLayout(goto_row_layout_);

        goto_row_lbl_ = new QLabel(tr("Goto Address Space Row: "));
        goto_row_sb_ = new QSpinBox();
        goto_row_sb_->setMinimum(0);
        goto_row_sb_->setMaximum(MBS_LARGEST_ADDR);
        connect(goto_row_sb_, SIGNAL(valueChanged(int)), this, SLOT(goto_row(int)));
        goto_row_layout_->addWidget(goto_row_lbl_);
        goto_row_layout_->addWidget(goto_row_sb_);

		this->init();
	}

    void Addr_Space_Widget::goto_row(int row_number)
    {
        this->addr_space_view_->goto_row(row_number);
    }

    void Addr_Space_Widget::closeEvent(QCloseEvent* /*event*/)
    {
    }

	Addr_Space_Widget::~Addr_Space_Widget()
	{
	}
}
