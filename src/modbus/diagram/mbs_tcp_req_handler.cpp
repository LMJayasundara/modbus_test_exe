/* Mbs_Tcp_Req_Handler header.
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

#include "modbus/diagram/mbs_tcp_req_handler.h"
#include "modbus/diagram/mbs_tcp_request.h"
#include "modbus/diagram/mbs_tcp_response.h"
#include "modbus/station/modbus_station.h"

#include "log/log_sender.h"

#include <iostream>
#include <algorithm>

using namespace std;

namespace cms
{
    // MODBUS On TCP/IP Application Data Unit 
	// ADU + PDU
	// MBAP Header | Function code | Data
	// MBAP: MODBUS Application Protocol header
    // MBAP: Transaction Identifier(2Bytes) + Protocol Identifier(2Bytes, 0=MODBUS PROTOCOL) + Length(2Bytes) + Unit Identifier(1Byte, like slave address)
    // Handle a modbus master request and produce a response.
    void Mbs_Tcp_Req_Handler::handle_request(const Mbs_Tcp_Request &request, Mbs_Tcp_Response &response, Modbus_Station &mbs_station)
    {
    	response.transaction_identifier = request.transaction_identifier;
    	response.protocol_identifier = request.protocol_identifier;
    	response.unit_identifier = request.unit_identifier;
    	response.function_code = request.function_code;
		unsigned int response_data_size = 11; // it should larger than 11

    	if ((request.function_code == RIR) || (request.function_code == RHR))
    	{
    		starting_addr_ = request.diagram[MBS_TCP_START_ADDR_POS] * 0x100 + request.diagram[MBS_TCP_START_ADDR_POS+1];   // request start point address
    		num_of_points_ = request.diagram[MBS_TCP_PTSCNT_POS] * 0x100 + request.diagram[MBS_TCP_PTSCNT_POS+1]; // request points count

    		response_data_size = MBS_TCP_START_ADDR_POS + 1 + num_of_points_ * 2; // after response head is a data bytes count

    		response.byte_count = static_cast<unsigned char>(num_of_points_ * 2);
    		response.length = response.byte_count + 3; // modbus tcp PDU length	

    		response.diagram.resize(response_data_size);
    		copy(request.diagram.begin(), request.diagram.begin() + MBS_TCP_START_ADDR_POS, response.diagram.begin());
    		response.diagram[MBS_TCP_START_ADDR_POS] = response.byte_count;
			response.diagram[MBS_TCP_LENGTH_POS+1] = response.length;

    		if (request.function_code == RIR)
    		{
    			this->handle_RIR_request(request, response, mbs_station);
    		}
    		else if (request.function_code == RHR)
    		{
    			this->handle_RHR_request(request, response, mbs_station);
    		}
    	}
		else if ((request.function_code == RCS) || (request.function_code == RIS))
		{
			starting_addr_ = request.diagram[MBS_TCP_START_ADDR_POS] * 0x100 + request.diagram[MBS_TCP_START_ADDR_POS+1];   // request start point address
			num_of_points_ = request.diagram[MBS_TCP_PTSCNT_POS] * 0x100 + request.diagram[MBS_TCP_PTSCNT_POS+1]; // request points count

			// response_head_length + points_count*2 / sizeof(mbs_byte) 
			response.byte_count = static_cast<unsigned char>(num_of_points_/ (sizeof(MBS_BYTE) * MBS_BYTE_BITS) + (num_of_points_%(sizeof(MBS_BYTE) * MBS_BYTE_BITS) ? 1:0));
			response_data_size = MBS_TCP_START_ADDR_POS + 1 + (unsigned int)response.byte_count;

			response.diagram.resize(response_data_size);
			copy(request.diagram.begin(), request.diagram.begin() + MBS_TCP_START_ADDR_POS, response.diagram.begin());
			response.diagram[MBS_TCP_START_ADDR_POS] = response.byte_count;

			response.length = response.byte_count + 3; // modbus tcp PDU length	
			response.diagram[MBS_TCP_LENGTH_POS+1] = response.length;

			if (request.function_code == RCS)
			{
				this->handle_RCS_request(request, response, mbs_station);
			}
			else if (request.function_code == RIS)
			{
				this->handle_RIS_request(request, response, mbs_station);
			}
		}
		else if (request.function_code == FSC)
		{
			starting_addr_ = request.diagram[MBS_TCP_START_ADDR_POS] * 0x100 + request.diagram[MBS_TCP_START_ADDR_POS+1];   // request start point address

			// Slave Address + Function + Coil Address Hi + Coil Address Lo + Force Data Hi + Force Data Lo
			response_data_size = 12;

			response.byte_count = 2;
			num_of_points_ = 1;

			response.diagram.resize(response_data_size);
			copy(request.diagram.begin(), request.diagram.begin() + MBS_TCP_START_ADDR_POS, response.diagram.begin());

			response.length = response.byte_count + 4; // modbus tcp PDU length	
			response.diagram[MBS_TCP_LENGTH_POS+1] = response.length;

			response.diagram[8] = request.diagram[8];
			response.diagram[9] = request.diagram[9];
			response.diagram[10] = request.diagram[10];
			response.diagram[11] = request.diagram[11];

			if (request.function_code == FSC)
			{
				this->handle_FSC_request(request, response, mbs_station);
			}
		}
		else if (request.function_code == PSR)
		{
			starting_addr_ = request.diagram[MBS_TCP_START_ADDR_POS] * 0x100 + request.diagram[MBS_TCP_START_ADDR_POS+1];   // request start point address

			// Slave Address + Function + Coil Address Hi + Coil Address Lo + Force Data Hi + Force Data Lo
			response_data_size = 12;

			response.byte_count = 2;
			num_of_points_ = 1;

			response.diagram.resize(response_data_size);
			copy(request.diagram.begin(), request.diagram.begin() + MBS_TCP_START_ADDR_POS, response.diagram.begin());

			response.length = response.byte_count + 4; // modbus tcp PDU length	
			response.diagram[MBS_TCP_LENGTH_POS+1] = response.length;

			response.diagram[8] = request.diagram[8];
			response.diagram[9] = request.diagram[9];
			response.diagram[10] = request.diagram[10];
			response.diagram[11] = request.diagram[11];

			if (request.function_code == PSR)
			{
				this->handle_PSR_request(request, response, mbs_station);
			}
		}
		else if (request.function_code == FMC)
		{
			starting_addr_ = request.diagram[MBS_TCP_START_ADDR_POS] * 0x100 + request.diagram[MBS_TCP_START_ADDR_POS+1];   // request start point address
			num_of_points_ = request.diagram[MBS_TCP_PTSCNT_POS] * 0x100 + request.diagram[MBS_TCP_PTSCNT_POS+1]; // request points count
			response.byte_count = static_cast<unsigned char>(num_of_points_/ (sizeof(MBS_BYTE) * MBS_BYTE_BITS) + (num_of_points_%(sizeof(MBS_BYTE) * MBS_BYTE_BITS) ? 1:0));

			// Slave Address + Function + Coil Address Hi + Coil Address Lo + Quantity Data Hi + Quantity Data Lo
			response_data_size = 12;

			response.diagram.resize(response_data_size);
			copy(request.diagram.begin(), request.diagram.begin() + MBS_TCP_REQUEST_DATA_POS - 1, response.diagram.begin());

			response.length = response.byte_count + 4; // modbus tcp PDU length	
			
			if (request.function_code == FMC)
			{
				this->handle_FMC_request(request, response, mbs_station);
			}
		}
		else if (request.function_code == PMR)
		{
			starting_addr_ = request.diagram[MBS_TCP_START_ADDR_POS] * 0x100 + request.diagram[MBS_TCP_START_ADDR_POS+1];   // request start point address
			num_of_points_ = request.diagram[MBS_TCP_PTSCNT_POS] * 0x100 + request.diagram[MBS_TCP_PTSCNT_POS+1]; // request points count
			response.byte_count = num_of_points_ * 2;

			// Slave Address + Function + Coil Address Hi + Coil Address Lo + Quantity Data Hi + Quantity Data Lo
			response_data_size = 12;

			response.diagram.resize(response_data_size);
			copy(request.diagram.begin(), request.diagram.begin() + MBS_TCP_REQUEST_DATA_POS - 1, response.diagram.begin());

			response.length = response.byte_count + 4; // modbus tcp PDU length	
			
			if (request.function_code == PMR)
			{
				this->handle_PMR_request(request, response, mbs_station);
			}
		}
    }

	// Handle a modbus RCS request and produce a response.
	void Mbs_Tcp_Req_Handler::handle_RCS_request(const Mbs_Tcp_Request &/*request*/, Mbs_Tcp_Response &response, Modbus_Station &mbs_station)
	{
		if ((starting_addr_+num_of_points_-1)>MBS_LARGEST_ADDR)
		{
			Log_Sender::send_msg(QString("RCS request address : %1 exceed the number of register").arg(starting_addr_));
			// error, exceed the number of register
			return;
		}
		
		for (unsigned int byte_index=0; byte_index<response.byte_count; ++byte_index)
		{
			// fill all data with zero
			response.diagram[MBS_TCP_RESPONSE_DATA_POS+byte_index] = 0x0000; 
		}
		MBS_BYTE current_byte_data;
		for (unsigned int pt_index=0; pt_index<num_of_points_; pt_index+=MBS_BYTE_BITS)
		{
			for (unsigned int bit_index=0; bit_index<MBS_BYTE_BITS; ++bit_index)
			{
				bool bit_data = mbs_station.get_point_bit_data(starting_addr_+pt_index+bit_index, REF_0X);
				if (bit_data == true)
				{
					current_byte_data |= 1 << (bit_index%MBS_BYTE_BITS);
				}
				else
				{
					current_byte_data &= ~(1 << (bit_index%MBS_BYTE_BITS));		        
				}
			}
			response.diagram[MBS_TCP_RESPONSE_DATA_POS+pt_index/(sizeof(MBS_BYTE) * MBS_BYTE_BITS)] = current_byte_data;
		}
	}

	// Handle a modbus RIS request and produce a response.
	void Mbs_Tcp_Req_Handler::handle_RIS_request(const Mbs_Tcp_Request &/*request*/, Mbs_Tcp_Response &response, Modbus_Station &mbs_station)
	{
        if ((starting_addr_+num_of_points_-1)>MBS_LARGEST_ADDR)
        {
			Log_Sender::send_msg(QString("RIS request address : %1 exceed the number of register").arg(starting_addr_));
            // error, exceed the number of register
            return;
        }

        for (unsigned int byte_index=0; byte_index<response.byte_count; ++byte_index)
        {
            // fill all data with zero
            response.diagram[MBS_TCP_RESPONSE_DATA_POS+byte_index] = 0x0000; 
        }
        MBS_BYTE current_byte_data;
        for (unsigned int pt_index=0; pt_index<num_of_points_; pt_index+=MBS_BYTE_BITS)
        {
            for (unsigned int bit_index=0; bit_index<MBS_BYTE_BITS; ++bit_index)
            {
                bool bit_data = mbs_station.get_point_bit_data(starting_addr_+pt_index+bit_index, REF_1X);
                if (bit_data == true)
                {
                    current_byte_data |= 1 << (bit_index%MBS_BYTE_BITS);
                }
                else
                {
                    current_byte_data &= ~(1 << (bit_index%MBS_BYTE_BITS));		        
                }
            }
            response.diagram[MBS_TCP_RESPONSE_DATA_POS+pt_index/(sizeof(MBS_BYTE) * MBS_BYTE_BITS)] = current_byte_data;
        }
	}

    // Handle a modbus RIR request and produce a response.
    void Mbs_Tcp_Req_Handler::handle_RIR_request(const Mbs_Tcp_Request &/*request*/, Mbs_Tcp_Response &response, Modbus_Station &mbs_station)
    {
		if ((starting_addr_+num_of_points_-1)>MBS_LARGEST_ADDR)
		{
			Log_Sender::send_msg(QString("RIR request address : %1 exceed the number of register").arg(starting_addr_));
			// error, exceed the number of register
			return;
		}
    	for (unsigned int pt_index=0; pt_index<num_of_points_; ++pt_index)
    	{
    		response.diagram[MBS_TCP_START_ADDR_POS+1+pt_index*2] = mbs_station.get_point_data_hi(starting_addr_+pt_index, REF_3X);
    		response.diagram[MBS_TCP_START_ADDR_POS+1+pt_index*2+1] = mbs_station.get_point_data_lo(starting_addr_+pt_index, REF_3X);
    	}
    }

    // Handle a modbus RHR request and produce a response.
    void Mbs_Tcp_Req_Handler::handle_RHR_request(const Mbs_Tcp_Request &/*request*/, Mbs_Tcp_Response &response, Modbus_Station &mbs_station)
    {
		if ((starting_addr_+num_of_points_-1)>MBS_LARGEST_ADDR)
		{
			Log_Sender::send_msg(QString("RHR request address : %1 exceed the number of register").arg(starting_addr_));
			// error, exceed the number of register
			return;
		}
    	for (unsigned int pt_index=0; pt_index<num_of_points_; ++pt_index)
    	{
    		response.diagram[MBS_TCP_START_ADDR_POS+1+pt_index*2] = mbs_station.get_point_data_hi(starting_addr_+pt_index, REF_4X);
    		response.diagram[MBS_TCP_START_ADDR_POS+1+pt_index*2+1] = mbs_station.get_point_data_lo(starting_addr_+pt_index, REF_4X);
    	}
    }

	// Handle a modbus FSC request and produce a response.
    void Mbs_Tcp_Req_Handler::handle_FSC_request(const Mbs_Tcp_Request &request, Mbs_Tcp_Response &/*response*/, Modbus_Station &mbs_station)
    {
		if ((starting_addr_+num_of_points_-1)>MBS_LARGEST_ADDR)
		{
			Log_Sender::send_msg(QString("FSC request address : %1 exceed the number of register").arg(starting_addr_));
			// error, exceed the number of register
			return;
		}
    	
		if ((request.diagram[10] == 0xFF) && (request.diagram[11] == 0x00))
		{
			mbs_station.set_point_bit_data(starting_addr_, true, REF_0X);
		}
		else if ((request.diagram[10] == 0x00) && (request.diagram[11] == 0x00))
		{
			mbs_station.set_point_bit_data(starting_addr_, false, REF_0X);
		}
		else
		{
			// it's illegal request
			Log_Sender::send_msg(QString("It's an illegal FSC request: %1").arg(request.diagram[10]*0x100+request.diagram[11]));
		}
    }

	// Handle a modbus PSR request and produce a response.
    void Mbs_Tcp_Req_Handler::handle_PSR_request(const Mbs_Tcp_Request &request, Mbs_Tcp_Response &/*response*/, Modbus_Station &mbs_station)
    {
		if ((starting_addr_+num_of_points_-1)>MBS_LARGEST_ADDR)
		{
			Log_Sender::send_msg(QString("PSR request address : %1 exceed the number of register").arg(starting_addr_));
			// error, exceed the number of register
			return;
		}
    	
		mbs_station.set_point_data_hi(starting_addr_, request.diagram[10], REF_4X);
		mbs_station.set_point_data_lo(starting_addr_, request.diagram[11], REF_4X);
    }

	// Handle a modbus FMC request and produce a response.
	void Mbs_Tcp_Req_Handler::handle_FMC_request(const Mbs_Tcp_Request &request, Mbs_Tcp_Response &/*response*/, Modbus_Station &mbs_station)
	{
		if ((starting_addr_+num_of_points_-1)>MBS_LARGEST_ADDR)
		{
			Log_Sender::send_msg(QString("FMC request address : %1 exceed the number of register").arg(starting_addr_));
			// error, exceed the number of register
			return;
		}
		
		unsigned int pts_setted = 0;
		for (unsigned int pt_index=0; pt_index<num_of_points_; pt_index+=MBS_BYTE_BITS)
		{
			unsigned short bits_mask = 0x0001;
			for (unsigned int bit_idx=0; bit_idx<MBS_BYTE_BITS; ++bit_idx)
			{
				if ((request.diagram[MBS_TCP_REQUEST_DATA_POS+pt_index/MBS_BYTE_BITS] & bits_mask) == bits_mask)
					mbs_station.set_point_bit_data(starting_addr_+pt_index+bit_idx, true, REF_0X);
				else
					mbs_station.set_point_bit_data(starting_addr_+pt_index+bit_idx, false, REF_0X);

				++pts_setted;
				bits_mask <<= 1;
				if (pts_setted == num_of_points_)
					return;
			}
		}
	}


	// Handle a modbus PMR request and produce a response.
	void Mbs_Tcp_Req_Handler::handle_PMR_request(const Mbs_Tcp_Request &request, Mbs_Tcp_Response &/*response*/, Modbus_Station &mbs_station)
	{
		if ((starting_addr_+num_of_points_-1)>MBS_LARGEST_ADDR)
		{
			Log_Sender::send_msg(QString("PMR request address : %1 exceed the number of register").arg(starting_addr_));
			// error, exceed the number of register
			return;
		}
		
		for (unsigned int pt_index=0; pt_index<num_of_points_; ++pt_index)
		{
			mbs_station.set_point_data_hi(starting_addr_+pt_index, request.diagram[MBS_TCP_REQUEST_DATA_POS+pt_index*2], REF_4X);
			mbs_station.set_point_data_lo(starting_addr_+pt_index, request.diagram[MBS_TCP_REQUEST_DATA_POS+pt_index*2+1], REF_4X);
		}
	}
} // namespace cms

