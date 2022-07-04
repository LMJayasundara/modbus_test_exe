/* Addr_Space body.
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

#include <cstdlib>

#include "modbus/address/addr_space.h"
#include "modbus/config/mbs_def.h"

namespace cms
{
    Addr_Space::Addr_Space(MBS_FC type)
        : ref_type(type)
    {
    	if ((ref_type == REF_0X) || (ref_type == REF_1X))
    		addr_.resize(MAX_SIZE / (sizeof(MBS_BYTE) * 8));
    	else
    		addr_.resize(MAX_SIZE * 2);  // for REF_3X, REF_4X
    }

	// for REF_3X, REF_4X only
	MBS_BYTE Addr_Space::get_point_high(unsigned int index)
	{
		return addr_[index*2];
	}

	// for REF_3X, REF_4X only
	MBS_BYTE Addr_Space::get_point_low(unsigned int index)
	{
		return addr_[index*2+1];
	}

	// for REF_3X, REF_4X only
	unsigned int Addr_Space::get_point_data(unsigned int index)
	{
		return addr_[index*2] * 0x0100 + addr_[index*2+1];
	}

	// for REF_0X, REF_1X only
	MBS_BYTE Addr_Space::get_bit_data(unsigned int index)
	{
		if (index > MBS_LARGEST_ADDR)
			return false;
		else
        {
            bool a = (addr_[index/(sizeof(MBS_BYTE)*8)] >> (index%8)) & 0x0001;
			return a;
        }
	}

	// REF_0X, REF_1X only
	MBS_BYTE Addr_Space::get_byte_data(unsigned int index)
	{
		return addr_[index/(sizeof(MBS_BYTE)*8)];
	}

	// for REF_3X, REF_4X only, set point word data's high byte
	void Addr_Space::set_point_high(unsigned int index, unsigned char data)
	{
		addr_[index*2] = data;
	}

	// for REF_3X, REF_4X only, set point word data's low byte
	void Addr_Space::set_point_low(unsigned int index, unsigned char data)
	{
		addr_[index*2+1] = data;
	}

	// for REF_3X, REF_4X only
	void Addr_Space::set_point_data(unsigned int index, unsigned int data)
	{
		addr_[index*2] = data >> 8;
		addr_[index*2+1] = data & 0xFF;
	}

	// for REF_0X, REF_1X only
	void Addr_Space::set_bit_data(unsigned int index, bool is_set_true)
	{
		if (is_set_true)
		{
			addr_[index/(sizeof(MBS_BYTE)*8)] |= 1 << (index%8);
		}
		else
		{
			addr_[index/(sizeof(MBS_BYTE)*8)] &= ~(1 << (index%8));		        
		}
	}

    // fill random data in address space
    void Addr_Space::fill_random_data()
    {
    	for (std::vector<MBS_BYTE>::size_type i=0; i< addr_.size(); ++i)
    	{
    		std::srand(i);
    		addr_[i] =  std::rand() % MAX_SIZE;
    	}
    }

    // fill zero data in address space
    void Addr_Space::fill_zero_data()
    {
    	for (std::vector<MBS_BYTE>::size_type i=0; i< addr_.size(); ++i)
    	{
    		addr_[i] =  0;
    	}
    }
} // namespace cms

