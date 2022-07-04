/* Hex_Convertor_Widget.
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

#include <QLabel>
#include <QDateTime>
#include <QDate>
#include <QTime>

#include <cstdlib>

#include "tools/hex_convertor_widget.h"

using namespace std;

namespace cms
{
	Hex_Convertor_Widget::Hex_Convertor_Widget()
	{
		ui.setupUi(this);
		connect(ui.to_number_btn_, SIGNAL(clicked()), this, SLOT(execute_num_convert()));
		connect(ui.to_hex_btn_, SIGNAL(clicked()), this, SLOT(execute_hex_convert()));
	}

	void Hex_Convertor_Widget::execute_num_convert()
	{
		if (ui.type_cb_->currentText() == "UINT32")
		{
			convert_hex_to_uint32();
		}
		else if (ui.type_cb_->currentText() == "INT32")
		{
			convert_hex_to_int32();
		}
		else if (ui.type_cb_->currentText() == "FLOAT32")
		{
			convert_hex_to_float32();
		}
	}

	void Hex_Convertor_Widget::convert_hex_to_uint32()
	{
		unsigned int int_num = ui.hex_txt_->text().toUInt(0, 16);
		ui.number_txt_->setText(QString("%1").arg(int_num));
	}

	void Hex_Convertor_Widget::convert_hex_to_int32()
	{
		unsigned int int_num = ui.hex_txt_->text().toUInt(0, 16);
		int signed_num = 0;
		memcpy(&signed_num, &int_num, sizeof(signed_num));
		ui.number_txt_->setText(QString("%1").arg(signed_num));
	}

	void Hex_Convertor_Widget::convert_hex_to_float32()
	{
		unsigned int int_num = ui.hex_txt_->text().toUInt(0, 16);
		float float_num = 0;
		memcpy(&float_num, &int_num, sizeof(float_num));
		ui.number_txt_->setText(QString("%1").arg(float_num));
	}

	void Hex_Convertor_Widget::execute_hex_convert()
	{
		if (ui.type_cb_->currentText() == "UINT32")
		{
			convert_uint32_to_hex();
		}
		else if (ui.type_cb_->currentText() == "INT32")
		{
			convert_int32_to_hex();
		}
		else if (ui.type_cb_->currentText() == "FLOAT32")
		{
			convert_float32_to_hex();
		}
	}

	void Hex_Convertor_Widget::convert_uint32_to_hex()
	{
		unsigned int int_num = ui.number_txt_->text().toUInt();
		ui.hex_txt_->setText(QString("%1").arg(int_num, 8, 16).toUpper());
	}

	void Hex_Convertor_Widget::convert_int32_to_hex()
	{
		int int_num = ui.number_txt_->text().toInt();
		ui.hex_txt_->setText(QString("%1").arg(int_num, 8, 16).toUpper());
	}

	void Hex_Convertor_Widget::convert_float32_to_hex()
	{
		float float_num = ui.number_txt_->text().toFloat();
		unsigned int int_num = 0;
		memcpy(&int_num, &float_num, sizeof(int_num));
		ui.hex_txt_->setText(QString("%1").arg(int_num, 8, 16).toUpper());
	}
}
