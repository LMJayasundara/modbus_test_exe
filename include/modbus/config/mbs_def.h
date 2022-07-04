/* Modbus Defines header.
   Copyright (C) 2013, 2014
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

namespace cms
{
#define MBS_LARGEST_ADDR 65535

#define MBS_LARGEST_MASTER_GROUP 1024

#define DIAGRAM_MAX_LEN 288

    typedef unsigned char MBS_BYTE;

	enum MBS_FC { REF_0X=0, REF_1X=1, REF_3X=3, REF_4X=4};   // 0X-Coil, 1X-Input Status, 3X-Input Register, 4X-Holding Register

    enum Function_Code { RCS = 1, RIS = 2, RHR = 3, RIR = 4, FSC = 5, PSR = 6, FMC = 15, PMR = 16};

	enum MBS_STATION_TYPE {MBS_SERIAL_MASTER, MBS_SERIAL_SLAVE, MBS_TCP_MASTER, MBS_TCP_SLAVE};

    enum MBS_VALIDATE_RC {V_OK = 0, DISCARD_FIRST, DISCARD_ALL, RECV_AGAIN, SLAVE_ID_UNMATCHED, RECV_TOO_MANY, DISCARD_UNTIL_NOT_FULL, ABNORMAL};

    enum MBS_DIAGRAM_TYPE {UNKNOWN=0, TX, RX, DISCARD};

	const unsigned int MBS_BYTE_BITS = 8;

    const unsigned int MAX_REQUSET_POINTS = 255;

    const int MIN_MBS_SERIAL_DIAGRAM_LENGTH = 4; // at lease, SLAVE_ID + FUNCTION_CODE + CRC(16bits)

	std::string convert_function_code_to_str(unsigned int fc_code);

	unsigned int convert_str_to_function_code(const std::string& fc_str);

	unsigned int convert_str_to_mbs_addr_type(const std::string& addr_type_str);

	std::string convert_mbs_addr_type_to_str(unsigned int mbs_addr_type);

	unsigned int convert_str_to_io_type(const std::string& io_type_str);

	std::string convert_io_type_to_str(unsigned int io_type);

	unsigned int convert_str_to_value_type(const std::string& value_type_str);

	std::string convert_value_type_to_str(unsigned int value_type);
}    

#endif
