/* Addr_Space header.
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


#ifndef CMS_ADDR_SPACE_H
#define CMS_ADDR_SPACE_H

#include <vector>

#include "modbus/config/mbs_def.h"

namespace cms
{
    class Addr_Space
    {
    public:
        // Coil Status:0X; Input Status: 1X; Input Registers: 3X; Holding Registers: 4X
        // Input Status   | Single bit  | Read-Only  | This type of data can be provided by an I/O system.
        // Coil Status             | Single bit  | Read-Write | This type of data can be alterable by an application program.
        // Input Registers   | 16-bit word | Read-Only  | This type of data can be provided by an I/O system
        // Holding Registers | 16-bit word | Read-Write | This type of data can be alterable by an application program.
        
        
        MBS_FC ref_type;  // modbus address reference type, 0X,1X,3X,4X
        
        static const unsigned int MAX_SIZE = 65536;
        
        explicit Addr_Space(MBS_FC type);
    
        // for REF_3X, REF_4X only
        MBS_BYTE get_point_high(unsigned int index);

        // for REF_3X, REF_4X only
        MBS_BYTE get_point_low(unsigned int index);

		// for REF_3X, REF_4X only
		unsigned int get_point_data(unsigned int index);

        // for REF_0X, REF_1X only
        MBS_BYTE get_bit_data(unsigned int index);

		// REF_0X, REF_1X only
		MBS_BYTE get_byte_data(unsigned int index);

        // for REF_3X, REF_4X only
        void set_point_high(unsigned int index, unsigned char data);

        // for REF_3X, REF_4X only
        void set_point_low(unsigned int index, unsigned char data);

		// for REF_3X, REF_4X only
		void set_point_data(unsigned int index, unsigned int data);

		// for REF_0X, REF_1X only
		void set_bit_data(unsigned int index, bool is_set_true);

        // fill random data in address space
        void fill_random_data();

        // fill zero data in address space
        void fill_zero_data();

    private:   
		// modbus address is word address, one address is 16bits, 2bytes
		// MBS_BYTE is 8bits, so create HR and IR use double max size
        std::vector<MBS_BYTE> addr_;
    };

} // namespace cms

#endif
