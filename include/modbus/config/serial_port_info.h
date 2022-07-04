/* Serial_Port_Info header.
   Copyright (C) 2011
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


#ifndef CMS_SERIAL_PORT_INFO_H
#define CMS_SERIAL_PORT_INFO_H

#include <string>

namespace cms
{
    class Serial_Port_Info
	{
	public:
		std::string port_name;
		unsigned int baud_rate;
	    unsigned int data_bits;
	    std::string parity;
   	    std::string stop_bits;
        std::string flow_ctrl;   
    };
} // namespace cms

#endif
