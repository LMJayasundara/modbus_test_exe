/* Modbus Defines Body.
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


#ifndef CMS_MBS_DEF_H
#define CMS_MBS_DEF_H

#include <string>

#include "modbus/config/mbs_def.h"

namespace cms
{
	std::string convert_function_code_to_str(unsigned int fc_code)
	{
		switch (fc_code)
		{
		case 1:
			return "RCS";
		case 2:
			return "RIS";
		case 3:
			return "RHR";
		case 4:
			return "RIR";
		case 5:
			return "FSC";
		case 6:
			return "PSR";
		case 15:
			return "FMC";
		case 16:
			return "PMR";
		default:
			return "UKNOWN";
		}
	}

	unsigned int convert_str_to_function_code(const std::string& fc_str)
	{
		if (fc_str == "RCS")
			return 1;
		else if (fc_str == "RIS")
			return 2;
		else if (fc_str == "RHR")
			return 3;
		else if (fc_str == "RIR")
			return 4;
		else if (fc_str == "FSC")
			return 5;
		else if (fc_str == "PSR")
			return 6;
		else if (fc_str == "FMC")
			return 15;
		else if (fc_str == "PMR")
			return 16;
		else
			return 0;
	}

	unsigned int convert_str_to_mbs_addr_type(const std::string& addr_type_str)
	{
		if (addr_type_str == "0X Coil Status")
			return 0;
		else if (addr_type_str == "1X Input Status")
			return 1;
		else if (addr_type_str == "3X Input Register")
			return 3;
		else if (addr_type_str == "4X Holding Register")
			return 4;
		else return 0;
	}

	std::string convert_mbs_addr_type_to_str(unsigned int mbs_addr_type)
	{
		switch (mbs_addr_type)
		{
		case 0:
			return "0X Coil Status";
		case 1:
			return "1X Input Status";
		case 3:
			return "3X Input Register";
		case 4:
			return "4X Holding Register";
		default:
			return "UKNOWN";
		}
	}

	unsigned int convert_str_to_io_type(const std::string& io_type_str)
	{
		if (io_type_str == "Input")
			return 0;
		else if (io_type_str == "Output")
			return 1;
		else return 0;
	}

	std::string convert_io_type_to_str(unsigned int io_type)
	{
		switch (io_type)
		{
		case 0:
			return "Input";
		case 1:
			return "Output";
		default:
			return "UKNOWN";
		}
	}

	unsigned int convert_str_to_value_type(const std::string& value_type_str)
	{
		// 0 - int16, 1 -  uint16, 2 - int32, 3 - int32rev, 4 - int32_bs, 5 - int32_ws_bs,
		// 6 - uint32, 7 - uint32rev, 8 - uint32_bs, 9 - uint32_ws_bs, 10 - float,
		// 11 - floatrev, 12 - float_bs, 13 - floot_ws_bs, 14 - boolean
		if (value_type_str == "INT16")
			return 0;
		else if (value_type_str == "UINT16")
			return 1;
		else if (value_type_str == "INT32")
			return 2;
		else if (value_type_str == "INT32REV")
			return 3;
		else if (value_type_str == "INT32_BS")
			return 4;
		else if (value_type_str == "INT32_WS_BS")
			return 5;
		else if (value_type_str == "UINT32")
			return 6;
		else if (value_type_str == "UINT32REV")
			return 7;
		else if (value_type_str == "UINT32_BS")
			return 8;
		else if (value_type_str == "UINT32_WS_BS")
			return 9;
		else if (value_type_str == "FLOAT")
			return 10;
		else if (value_type_str == "FLOATREV")
			return 11;
		else if (value_type_str == "FLOAT_BS")
			return 12;
		else if (value_type_str == "FLOAT_WS_BS")
			return 13;
		else if (value_type_str == "BOOLEAN")
			return 14;
		else return 0;
	}

	std::string convert_value_type_to_str(unsigned int value_type)
	{
		// 0 - int16, 1 -  uint16, 2 - int32, 3 - int32rev, 4 - int32_bs, 5 - int32_ws_bs,
		// 6 - uint32, 7 - uint32rev, 8 - uint32_bs, 9 - uint32_ws_bs, 10 - float,
		// 11 - floatrev, 12 - float_bs, 13 - floot_ws_bs, 14 - boolean
		switch (value_type)
		{
		case 0:
			return "INT16";
		case 1:
			return "UINT16";
		case 2:
			return "INT32";
		case 3:
			return "INT32REV";
		case 4:
			return "INT32_BS";
		case 5:
			return "INT32_WS_BS";
		case 6:
			return "UINT32";
		case 7:
			return "UINT32REV";
		case 8:
			return "UINT32_BS";
		case 9:
			return "UINT32_WS_BS";
		case 10:
			return "FLOAT";
		case 11:
			return "FLOATREV";
		case 12:
			return "FLOAT_BS";
		case 13:
			return "FLOAT_WS_BS";
		case 14:
			return "BOOLEAN";
		default:
			return "UKNOWN";
		}
	}
}    

#endif
