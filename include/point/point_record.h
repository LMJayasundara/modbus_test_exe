/* Point_Record Header.
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
along with SmartDCS.comm_svr; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#ifndef CMS_POINT_RECORD_H
#define CMS_POINT_RECORD_H

#include <string>

//int16, short, uint16, 
//word, int32, long, 
//uint32, ulong, int32rev, 
//uint32rev, bcd4, bcd8, 
//float, floatrev 

namespace cms
{
    typedef short CMS_INT16;
	typedef unsigned short CMS_UINT16;
	typedef int CMS_INT32;
	typedef unsigned int CMS_UINT32;
	typedef float CMS_FLOAT32;

	enum POINT_VALUE_TYPE { INT16, UINT16, INT32, INT32REV, INT32_BS, INT32_WS_BS,
	                                          UINT32, UINT32REV, UINT32_BS, UINT32_WS_BS, FLOAT,
	                                          FLOATREV, FLOAT_BS, FLOAT_WS_BS, BOOLEAN};

	enum PT_IO_TYPE { INPUT=0, OUTPUT=1};

	const unsigned int MAX_POINTS_RECORD = 1024;

	class Point_Record
	{
	public:
		std::string point_name;
		unsigned int value_type; 
		// 0 - int16, 1 -  uint16, 2 - int32, 3 - int32rev, 4 - int32_bs, 5 - int32_ws_bs,
		// 6 - uint32, 7 - uint32rev, 8 - uint32_bs, 9 - uint32_ws_bs, 10 - float,
		// 11 - floatrev, 12 - float_bs, 13 - floot_ws_bs, 14 - boolean
		unsigned int io_type; // 0: read, 1:write 2:read&write
		unsigned int mbs_addr_ref; // 0x, 1X, 3X, 4X
		unsigned int mbs_addr;
		union
		{
			double d_value;
			long long int ll_value;
			unsigned long long int ull_value;
			float f_value;
			int i_value;
			unsigned int ui_value;
			bool b_value;		
		} union_value;
	};
}

#endif