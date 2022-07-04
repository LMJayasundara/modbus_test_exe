/* Master_Config Header.
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

#ifndef CMS_MASTER_CONFIG_H
#define CMS_MASTER_CONFIG_H

namespace cms
{
	const unsigned int MAX_MASTER_CONFIG_RECORD = 128;

	class Master_Config_Record
	{
	public:
		unsigned char slave_addr;
		unsigned char function_code;
		unsigned int start_addr;
		unsigned int point_count;
		unsigned int retries;
		unsigned int timeout;
	};
}

#endif