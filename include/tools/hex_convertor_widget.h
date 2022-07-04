/* Hex_Convertor_Widget Header.
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

#ifndef CMS_HEX_CONVERTOR_WIDGET_H
#define CMS_HEX_CONVERTOR_WIDGET_H

#include <QMainWindow>
#include <QPointer>

#include "ui/ui_hex_convertor_widget.h"

namespace cms
{
	class Hex_Convertor_Widget : public QMainWindow
	{
		Q_OBJECT
	public:
		Hex_Convertor_Widget();

		public slots:
			void execute_num_convert();
			void convert_hex_to_uint32();
			void convert_hex_to_int32();
			void convert_hex_to_float32();

			void execute_hex_convert();
			void convert_uint32_to_hex();
			void convert_int32_to_hex();
			void convert_float32_to_hex();

	private:
		Ui::hex_convertor_widget ui;

	private:
	};
}

#endif