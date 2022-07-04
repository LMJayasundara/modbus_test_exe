/* Mbs_Tcp_Response header.
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


#ifndef CMS_MBS_TCP_RESPONSE_H
#define CMS_MBS_TCP_RESPONSE_H

#include <vector>

namespace cms
{
    class Mbs_Tcp_Response
    {
    public:
    	// Function Code
		enum Function_Code { RCS = 1, RIS = 2, RHR = 3, RIR = 4, FSC = 5, PSR = 6, FMC = 15, PMR = 16};

    	/// MBAP Header
    	// Identification of a MODBUS Request / Response transaction.
    	// Initialized by the client
    	// Recopied by the server from the received request
    	unsigned int transaction_identifier; // 2bytes

    	// 0 = MODBUS protocol
    	// Initialized by the client
    	// Recopied by the server from the received request
    	unsigned int protocol_identifier; // 2bytes

    	// Number of following bytes
    	// Initialized by the client ( request)
    	// Initialized by the server ( Response)
    	unsigned int length; // 2bytes

    	// Identification of a remote slave connected on a serial line or on other buses.
    	// Initialized by the client
    	// Recopied by the server from the received request
    	// like the slave address in modbus serial
    	unsigned char unit_identifier; // 1byte

    	unsigned char function_code; // 1byte

    	unsigned char byte_count; // 1byte

    	// store the response diagarm
    	std::vector<unsigned char> diagram;

    private:   
        ;
    };

} // namespace cms

#endif
