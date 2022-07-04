/* Mbs_Tcp_Master body.
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

#include "modbus/diagram/mbs_tcp_master.h"
#include "modbus/station/modbus_station.h"
#include "modbus/config/mbs_def.h"

#include <QString>
#include "log/log_sender.h"

#include <iostream>

using namespace std;

namespace cms
{
    Mbs_Tcp_Master::Mbs_Tcp_Master(Modbus_Station &mbs_station)
        : retries_(3), is_last_request_success_(false), time_out_(1000),
        mbs_station_ref_(mbs_station)
    {
        response_buffer_.resize(DIAGRAM_MAX_LEN);
    }

    void Mbs_Tcp_Master::set_slave_addr(unsigned char slave_addr)
    {
        slave_addr_ = slave_addr;
    }

    unsigned char Mbs_Tcp_Master::get_slave_addr() const
    {
        return slave_addr_;
    }

    void Mbs_Tcp_Master::set_function_code(unsigned char function_code)
    {
        function_code_ = function_code;
    }

    unsigned char Mbs_Tcp_Master::get_function_code() const
    {
        return function_code_;
    }

    void Mbs_Tcp_Master::set_point_start_addr(unsigned int point_start_addr)
    {
        point_start_addr_ = point_start_addr;
    }

    unsigned int Mbs_Tcp_Master::get_point_start_addr() const
    {
        return point_start_addr_;
    }

    void Mbs_Tcp_Master::set_point_count(unsigned int point_count)
    {
        point_count_ = point_count;
    }

    unsigned int Mbs_Tcp_Master::get_point_count() const
    {
        return point_count_;
    }

    void Mbs_Tcp_Master::set_retries(unsigned int retries)
    {
        retries_ = retries;
    }

    unsigned int Mbs_Tcp_Master::get_retries() const
    {
        return retries_;
    }

    void Mbs_Tcp_Master::set_time_out(unsigned int time_out)
    {
        time_out_ = time_out;
    }

    unsigned int Mbs_Tcp_Master::get_time_out() const
    {
        return time_out_;
    }

    std::vector<unsigned char>& Mbs_Tcp_Master::get_request_buffer()
    {
        return request_buffer_;
    }

	unsigned int Mbs_Tcp_Master::get_request_length() const
	{
		return mbs_tcp_req_diagram_len_;
	}

    std::vector<unsigned char>& Mbs_Tcp_Master::get_response_buffer()
    {
        return response_buffer_;
    }

	unsigned int Mbs_Tcp_Master::get_response_length() const
	{
		return mbs_tcp_rsp_diagram_len_;
	}

    void Mbs_Tcp_Master::make_request()
    {
        if (function_code_ == RCS)
        {
            mbs_tcp_req_diagram_len_ = 12;
            request_buffer_.resize(mbs_tcp_req_diagram_len_);
            make_RCS_request();
        }
        else if (function_code_ == RIS)
        {
            mbs_tcp_req_diagram_len_ = 12;
            request_buffer_.resize(mbs_tcp_req_diagram_len_);
            make_RIS_request();
        }
        else if (function_code_ == RHR)
        {
            mbs_tcp_req_diagram_len_ = 12;
            request_buffer_.resize(mbs_tcp_req_diagram_len_);
            make_RHR_request();
        }
        else if (function_code_ == RIR)
        {
            mbs_tcp_req_diagram_len_ = 12;
            request_buffer_.resize(mbs_tcp_req_diagram_len_);
            make_RIR_request();
        }
        else if (function_code_ == FSC)
        {
            mbs_tcp_req_diagram_len_ = 12;
            request_buffer_.resize(mbs_tcp_req_diagram_len_);
            make_FSC_request();
        }
        else if (function_code_ == PSR)
        {
            mbs_tcp_req_diagram_len_ = 12;
            request_buffer_.resize(mbs_tcp_req_diagram_len_);
            make_PSR_request();
        }
        else if (function_code_ == FMC)
        {
            unsigned char byte_count = static_cast<unsigned char>(point_count_/ (sizeof(MBS_BYTE) * MBS_BYTE_BITS) + (point_count_%(sizeof(MBS_BYTE) * MBS_BYTE_BITS) ? 1:0));
            mbs_tcp_req_diagram_len_ = 11 + byte_count + 2;
            request_buffer_.resize(mbs_tcp_req_diagram_len_);
            make_FMC_request();
        }
		else if (function_code_ == PMR)
        {
            mbs_tcp_req_diagram_len_ = 11 + point_count_*2 + 2;
            request_buffer_.resize(mbs_tcp_req_diagram_len_);
            make_PMR_request();
        }
    }

    void Mbs_Tcp_Master::handle_response()
    {
        unsigned char response_slave_addr = response_buffer_[MBS_TCP_UNIT_IDENTIFIER_POS];
        if (response_slave_addr != slave_addr_)
        {
            Log_Sender::send_msg(QString("Not the request slave: %1 from slave: %1").arg(slave_addr_).arg(response_slave_addr));
            return;
        }

        unsigned char response_function_code = response_buffer_[MBS_TCP_PDU_POS];
        if (response_function_code != function_code_)
        {
            if (response_function_code == function_code_ | 0x80)
            {
                Log_Sender::send_msg(QString("Exception from slave: %1").arg(slave_addr_));
            }
            Log_Sender::send_msg(QString("Function code: %1 is not correct").arg(response_function_code));
            return;
        }

        if ((function_code_ == RCS) || (function_code_ == RIS) || (function_code_ == RHR) || (function_code_ == RIR))
        {
            unsigned int byte_count = response_buffer_[MBS_TCP_PDU_POS+1];
            if ((function_code_ == RCS) || (function_code_ == RIS) || (function_code_ == FMC))
            {
                unsigned bc = static_cast<unsigned char>(point_count_/ (sizeof(MBS_BYTE) * MBS_BYTE_BITS) + (point_count_%(sizeof(MBS_BYTE) * MBS_BYTE_BITS) ? 1:0));
                if (byte_count != bc)
                {
                    Log_Sender::send_msg(QString("Byte count: %1 is not correct").arg(byte_count));
                    return;
                }
            }
			else if (function_code_ == RHR) 
			{
				if (byte_count != point_count_*2)
                {
                    Log_Sender::send_msg(QString("Byte count: %1 is not correct").arg(byte_count));
                    return;
                }
			}
			// check byte count is no need for FMC&PMR response 
        }

        if (function_code_ == RCS)
        {
            handle_RCS_response();
        }
        else if (function_code_ == RIS)
        {
            handle_RIS_response();
        }
        else if (function_code_ == RHR)
        {
            handle_RHR_response();
        }
        else if (function_code_ == RIR)
        {
            handle_RIR_response();
        }
        else if (function_code_ == FSC)
        {
            handle_FSC_response();
        }
        else if (function_code_ == PSR)
        {
            handle_PSR_response();
        }
        else if (function_code_ == FMC)
        {
            handle_FMC_response();
        }
		else if (function_code_ == PMR)
        {
            handle_PMR_response();
        }
    }

    void Mbs_Tcp_Master::make_RCS_request()
    {
        request_buffer_[MBS_TCP_TRANSACTION_IDENTIFIER_POS] = 0x00;
        request_buffer_[MBS_TCP_TRANSACTION_IDENTIFIER_POS+1] = 0x00;
        request_buffer_[MBS_TCP_PROTOCOL_IDENTIFIER_POS] = 0x00;
        request_buffer_[MBS_TCP_PROTOCOL_IDENTIFIER_POS+1] = 0x00;
        request_buffer_[MBS_TCP_LENGTH_POS] = 0x00;
        request_buffer_[MBS_TCP_LENGTH_POS+1] = 0x06;
        request_buffer_[MBS_TCP_UNIT_IDENTIFIER_POS] = slave_addr_;
        request_buffer_[MBS_TCP_PDU_POS] = function_code_;
        request_buffer_[MBS_TCP_START_ADDR_POS] = (point_start_addr_ >> 8) & 0x00FF;
        request_buffer_[MBS_TCP_START_ADDR_POS+1] = point_start_addr_ & 0x00FF;
        request_buffer_[MBS_TCP_PTSCNT_POS] = (point_count_ >> 8) & 0x00FF;
        request_buffer_[MBS_TCP_PTSCNT_POS+1] = point_count_ & 0x00FF;   
    }

    void Mbs_Tcp_Master::make_RIS_request()
    {
        request_buffer_[MBS_TCP_TRANSACTION_IDENTIFIER_POS] = 0x00;
        request_buffer_[MBS_TCP_TRANSACTION_IDENTIFIER_POS+1] = 0x00;
        request_buffer_[MBS_TCP_PROTOCOL_IDENTIFIER_POS] = 0x00;
        request_buffer_[MBS_TCP_PROTOCOL_IDENTIFIER_POS+1] = 0x00;
        request_buffer_[MBS_TCP_LENGTH_POS] = 0x00;
        request_buffer_[MBS_TCP_LENGTH_POS+1] = 0x06;
        request_buffer_[MBS_TCP_UNIT_IDENTIFIER_POS] = slave_addr_;
        request_buffer_[MBS_TCP_PDU_POS] = function_code_;
        request_buffer_[MBS_TCP_START_ADDR_POS] = (point_start_addr_ >> 8) & 0x00FF;
        request_buffer_[MBS_TCP_START_ADDR_POS+1] = point_start_addr_ & 0x00FF;
        request_buffer_[MBS_TCP_PTSCNT_POS] = (point_count_ >> 8) & 0x00FF;
        request_buffer_[MBS_TCP_PTSCNT_POS+1] = point_count_ & 0x00FF;   
    }

    void Mbs_Tcp_Master::make_RHR_request()
    {
        request_buffer_[MBS_TCP_TRANSACTION_IDENTIFIER_POS] = 0x00;
        request_buffer_[MBS_TCP_TRANSACTION_IDENTIFIER_POS+1] = 0x00;
        request_buffer_[MBS_TCP_PROTOCOL_IDENTIFIER_POS] = 0x00;
        request_buffer_[MBS_TCP_PROTOCOL_IDENTIFIER_POS+1] = 0x00;
        request_buffer_[MBS_TCP_LENGTH_POS] = 0x00;
        request_buffer_[MBS_TCP_LENGTH_POS+1] = 0x06;
        request_buffer_[MBS_TCP_UNIT_IDENTIFIER_POS] = slave_addr_;
        request_buffer_[MBS_TCP_PDU_POS] = function_code_;
        request_buffer_[MBS_TCP_START_ADDR_POS] = (point_start_addr_ >> 8) & 0x00FF;
        request_buffer_[MBS_TCP_START_ADDR_POS+1] = point_start_addr_ & 0x00FF;
        request_buffer_[MBS_TCP_PTSCNT_POS] = (point_count_ >> 8) & 0x00FF;
        request_buffer_[MBS_TCP_PTSCNT_POS+1] = point_count_ & 0x00FF;   
    }

    void Mbs_Tcp_Master::make_RIR_request()
    {
        request_buffer_[MBS_TCP_TRANSACTION_IDENTIFIER_POS] = 0x00;
        request_buffer_[MBS_TCP_TRANSACTION_IDENTIFIER_POS+1] = 0x00;
        request_buffer_[MBS_TCP_PROTOCOL_IDENTIFIER_POS] = 0x00;
        request_buffer_[MBS_TCP_PROTOCOL_IDENTIFIER_POS+1] = 0x00;
        request_buffer_[MBS_TCP_LENGTH_POS] = 0x00;
        request_buffer_[MBS_TCP_LENGTH_POS+1] = 0x06;
        request_buffer_[MBS_TCP_UNIT_IDENTIFIER_POS] = slave_addr_;
        request_buffer_[MBS_TCP_PDU_POS] = function_code_;
        request_buffer_[MBS_TCP_START_ADDR_POS] = (point_start_addr_ >> 8) & 0x00FF;
        request_buffer_[MBS_TCP_START_ADDR_POS+1] = point_start_addr_ & 0x00FF;
        request_buffer_[MBS_TCP_PTSCNT_POS] = (point_count_ >> 8) & 0x00FF;
        request_buffer_[MBS_TCP_PTSCNT_POS+1] = point_count_ & 0x00FF;   
    }

    void Mbs_Tcp_Master::make_FSC_request()
    {
        request_buffer_[MBS_TCP_TRANSACTION_IDENTIFIER_POS] = 0x00;
        request_buffer_[MBS_TCP_TRANSACTION_IDENTIFIER_POS+1] = 0x00;
        request_buffer_[MBS_TCP_PROTOCOL_IDENTIFIER_POS] = 0x00;
        request_buffer_[MBS_TCP_PROTOCOL_IDENTIFIER_POS+1] = 0x00;
        request_buffer_[MBS_TCP_LENGTH_POS] = 0x00;
        request_buffer_[MBS_TCP_LENGTH_POS+1] = 0x06;
        request_buffer_[MBS_TCP_UNIT_IDENTIFIER_POS] = slave_addr_;
        request_buffer_[MBS_TCP_PDU_POS] = function_code_;
        request_buffer_[MBS_TCP_START_ADDR_POS] = (point_start_addr_ >> 8) & 0x00FF;
        request_buffer_[MBS_TCP_START_ADDR_POS+1] = point_start_addr_ & 0x00FF;
        bool pt_bit_val = mbs_station_ref_.get_point_bit_data(point_start_addr_, REF_0X);
        if (pt_bit_val)
        {
            request_buffer_[MBS_TCP_PDU_POS+3] = 0xFF;
            request_buffer_[MBS_TCP_PDU_POS+4] = 0x00;
        }
        else
        {
            request_buffer_[MBS_TCP_PDU_POS+3] = 0x00;
            request_buffer_[MBS_TCP_PDU_POS+4] = 0x00;
        }
    }

    void Mbs_Tcp_Master::make_PSR_request()
    {
        request_buffer_[MBS_TCP_TRANSACTION_IDENTIFIER_POS] = 0x00;
        request_buffer_[MBS_TCP_TRANSACTION_IDENTIFIER_POS+1] = 0x00;
        request_buffer_[MBS_TCP_PROTOCOL_IDENTIFIER_POS] = 0x00;
        request_buffer_[MBS_TCP_PROTOCOL_IDENTIFIER_POS+1] = 0x00;
        request_buffer_[MBS_TCP_LENGTH_POS] = 0x00;
        request_buffer_[MBS_TCP_LENGTH_POS+1] = 0x06;
        request_buffer_[MBS_TCP_UNIT_IDENTIFIER_POS] = slave_addr_;
        request_buffer_[MBS_TCP_PDU_POS] = function_code_;
        request_buffer_[MBS_TCP_START_ADDR_POS] = (point_start_addr_ >> 8) & 0x00FF;
        request_buffer_[MBS_TCP_START_ADDR_POS+1] = point_start_addr_ & 0x00FF;
        request_buffer_[MBS_TCP_PDU_POS+3] = mbs_station_ref_.get_point_data_hi(point_start_addr_, REF_4X);
        request_buffer_[MBS_TCP_PDU_POS+4] = mbs_station_ref_.get_point_data_lo(point_start_addr_, REF_4X);
    }

    void Mbs_Tcp_Master::make_FMC_request()
    {
        request_buffer_[MBS_TCP_TRANSACTION_IDENTIFIER_POS] = 0x00;
        request_buffer_[MBS_TCP_TRANSACTION_IDENTIFIER_POS+1] = 0x00;
        request_buffer_[MBS_TCP_PROTOCOL_IDENTIFIER_POS] = 0x00;
        request_buffer_[MBS_TCP_PROTOCOL_IDENTIFIER_POS+1] = 0x00;
        request_buffer_[MBS_TCP_UNIT_IDENTIFIER_POS] = slave_addr_;
        request_buffer_[MBS_TCP_PDU_POS] = function_code_;
        request_buffer_[MBS_TCP_START_ADDR_POS] = (point_start_addr_ >> 8) & 0x00FF;
        request_buffer_[MBS_TCP_START_ADDR_POS+1] = point_start_addr_ & 0x00FF;
        request_buffer_[MBS_TCP_PTSCNT_POS] = (point_count_ >> 8) & 0x00FF;
        request_buffer_[MBS_TCP_PTSCNT_POS+1] = point_count_ & 0x00FF;   

        if ((point_start_addr_+point_count_-1)>MBS_LARGEST_ADDR)
        {
            // error, exceed the number of register
            return;
        }

        if (point_count_ > MAX_REQUSET_POINTS)
        {
            // Todo: Gen error
            return;
        }

        unsigned char byte_count = static_cast<unsigned char>(point_count_/ (sizeof(MBS_BYTE) * MBS_BYTE_BITS) + (point_count_%(sizeof(MBS_BYTE) * MBS_BYTE_BITS) ? 1:0));
        unsigned int length = (unsigned int)byte_count+7;
        request_buffer_[MBS_TCP_LENGTH_POS] = 0x00;
        request_buffer_[MBS_TCP_LENGTH_POS+1] = (unsigned char)length;
        request_buffer_[MBS_TCP_PDU_POS+5] = byte_count;
        for (unsigned int byte_index=0; byte_index<byte_count; ++byte_index)
        {
            // fill all data with zero
            request_buffer_[MBS_TCP_REQUEST_DATA_POS+byte_index] = 0x0000; 
        }
        MBS_BYTE current_byte_data;
        for (unsigned int pt_index=0; pt_index<point_count_; pt_index+=MBS_BYTE_BITS)
        {
            for (unsigned int bit_index=0; bit_index<MBS_BYTE_BITS; ++bit_index)
            {
                bool bit_data = mbs_station_ref_.get_point_bit_data(point_start_addr_+pt_index+bit_index, REF_0X);
                if (bit_data == true)
                {
                    current_byte_data |= 1 << (bit_index%MBS_BYTE_BITS);
                }
                else
                {
                    current_byte_data &= ~(1 << (bit_index%MBS_BYTE_BITS));		        
                }
            }
            request_buffer_[MBS_TCP_REQUEST_DATA_POS+pt_index/(sizeof(MBS_BYTE) * MBS_BYTE_BITS)] = current_byte_data;
        }
    }

	void Mbs_Tcp_Master::make_PMR_request()
    {
        request_buffer_[MBS_TCP_TRANSACTION_IDENTIFIER_POS] = 0x00;
        request_buffer_[MBS_TCP_TRANSACTION_IDENTIFIER_POS+1] = 0x00;
        request_buffer_[MBS_TCP_PROTOCOL_IDENTIFIER_POS] = 0x00;
        request_buffer_[MBS_TCP_PROTOCOL_IDENTIFIER_POS+1] = 0x00;
        request_buffer_[MBS_TCP_UNIT_IDENTIFIER_POS] = slave_addr_;
        request_buffer_[MBS_TCP_PDU_POS] = function_code_;
        request_buffer_[MBS_TCP_START_ADDR_POS] = (point_start_addr_ >> 8) & 0x00FF;
        request_buffer_[MBS_TCP_START_ADDR_POS+1] = point_start_addr_ & 0x00FF;
        request_buffer_[MBS_TCP_PTSCNT_POS] = (point_count_ >> 8) & 0x00FF;
        request_buffer_[MBS_TCP_PTSCNT_POS+1] = point_count_ & 0x00FF;   
		
        unsigned int byte_count = point_count_ * 2;
        request_buffer_[MBS_TCP_PDU_POS+5] = byte_count;
        request_buffer_[MBS_TCP_LENGTH_POS] = ((byte_count + 7) >> 8) & 0x00FF;
        request_buffer_[MBS_TCP_LENGTH_POS+1] = (byte_count + 7) & 0x00FF;

		for (unsigned int pt_index=0; pt_index<point_count_; ++pt_index)
        {
            request_buffer_[MBS_TCP_REQUEST_DATA_POS+pt_index*2] = mbs_station_ref_.get_point_data_hi(point_start_addr_+pt_index, REF_4X);
            request_buffer_[MBS_TCP_REQUEST_DATA_POS+pt_index*2+1] = mbs_station_ref_.get_point_data_lo(point_start_addr_+pt_index, REF_4X);
        }
    }

    void Mbs_Tcp_Master::handle_RCS_response()
    {
        unsigned int pts_setted = 0;
        for (unsigned int pt_index=0; pt_index<point_count_; pt_index+=MBS_BYTE_BITS)
        {
            unsigned short bits_mask = 0x0001;
            for (unsigned int bit_idx=0; bit_idx<MBS_BYTE_BITS; ++bit_idx)
            {
                if ((response_buffer_[MBS_TCP_RESPONSE_DATA_POS+pt_index/MBS_BYTE_BITS] & bits_mask) == bits_mask)
                    mbs_station_ref_.set_point_bit_data(point_start_addr_+pt_index+bit_idx, true, REF_0X);
                else
                    mbs_station_ref_.set_point_bit_data(point_start_addr_+pt_index+bit_idx, false, REF_0X);

                ++pts_setted;
                bits_mask <<= 1;
                if (pts_setted == point_count_)
                    return;
            }
        }
    }

    void Mbs_Tcp_Master::handle_RIS_response()
    {
        unsigned int pts_setted = 0;
        for (unsigned int pt_index=0; pt_index<point_count_; pt_index+=MBS_BYTE_BITS)
        {
            unsigned short bits_mask = 0x0001;
            for (unsigned int bit_idx=0; bit_idx<MBS_BYTE_BITS; ++bit_idx)
            {
                if ((response_buffer_[MBS_TCP_RESPONSE_DATA_POS+pt_index/MBS_BYTE_BITS] & bits_mask) == bits_mask)
                    mbs_station_ref_.set_point_bit_data(point_start_addr_+pt_index+bit_idx, true, REF_1X);
                else
                    mbs_station_ref_.set_point_bit_data(point_start_addr_+pt_index+bit_idx, false, REF_1X);

                ++pts_setted;
                bits_mask <<= 1;
                if (pts_setted == point_count_)
                    return;
            }
        }
    }

    void Mbs_Tcp_Master::handle_RHR_response()
    {
        for (unsigned int pt_index=0; pt_index<point_count_; ++pt_index)
        {
            mbs_station_ref_.set_point_data_hi(point_start_addr_+pt_index, response_buffer_[MBS_TCP_RESPONSE_DATA_POS+pt_index*2], REF_4X);
            mbs_station_ref_.set_point_data_lo(point_start_addr_+pt_index, response_buffer_[MBS_TCP_RESPONSE_DATA_POS+pt_index*2+1], REF_4X);
        }
    } 

    void Mbs_Tcp_Master::handle_RIR_response()
    {
        for (unsigned int pt_index=0; pt_index<point_count_; ++pt_index)
        {
            mbs_station_ref_.set_point_data_hi(point_start_addr_+pt_index, response_buffer_[MBS_TCP_RESPONSE_DATA_POS+pt_index*2], REF_3X);
            mbs_station_ref_.set_point_data_lo(point_start_addr_+pt_index, response_buffer_[MBS_TCP_RESPONSE_DATA_POS+pt_index*2+1], REF_3X);
        }
    } 

    void Mbs_Tcp_Master::handle_FSC_response()
    {
        if ((request_buffer_[MBS_TCP_PDU_POS+1] ==  response_buffer_[MBS_TCP_PDU_POS+1]) && (request_buffer_[MBS_TCP_PDU_POS+2] ==  response_buffer_[MBS_TCP_PDU_POS+2])
            && (request_buffer_[MBS_TCP_PDU_POS+3] ==  response_buffer_[MBS_TCP_PDU_POS+3]) && (request_buffer_[MBS_TCP_PDU_POS+4] ==  response_buffer_[MBS_TCP_PDU_POS+4]))
        {
            // It's OK
        }
        else
        {
            Log_Sender::send_msg(QString("FSC response is not OK!"));
        }
    } 

    void Mbs_Tcp_Master::handle_PSR_response()
    {
        if ((request_buffer_[MBS_TCP_PDU_POS+1] ==  response_buffer_[MBS_TCP_PDU_POS+1]) && (request_buffer_[MBS_TCP_PDU_POS+2] ==  response_buffer_[MBS_TCP_PDU_POS+2])
            && (request_buffer_[MBS_TCP_PDU_POS+3] ==  response_buffer_[MBS_TCP_PDU_POS+3]) && (request_buffer_[MBS_TCP_PDU_POS+4] ==  response_buffer_[MBS_TCP_PDU_POS+4]))
        {
            // It's OK
        }
        else
        {
            Log_Sender::send_msg(QString("PSR response is not OK!"));
        }
    } 

    void Mbs_Tcp_Master::handle_FMC_response()
    {
        if ((request_buffer_[MBS_TCP_PDU_POS+1] ==  response_buffer_[MBS_TCP_PDU_POS+1]) && (request_buffer_[MBS_TCP_PDU_POS+2] ==  response_buffer_[MBS_TCP_PDU_POS+2])
            && (request_buffer_[MBS_TCP_PDU_POS+3] ==  response_buffer_[MBS_TCP_PDU_POS+3]) && (request_buffer_[MBS_TCP_PDU_POS+4] ==  response_buffer_[MBS_TCP_PDU_POS+4]))
        {
            // It's OK
        }
        else
        {
            Log_Sender::send_msg(QString("FMC response is not OK!"));
        }
    }

	void Mbs_Tcp_Master::handle_PMR_response()
    {
        if ((request_buffer_[MBS_TCP_PDU_POS+1] ==  response_buffer_[MBS_TCP_PDU_POS+1]) && (request_buffer_[MBS_TCP_PDU_POS+2] ==  response_buffer_[MBS_TCP_PDU_POS+2])
            && (request_buffer_[MBS_TCP_PDU_POS+3] ==  response_buffer_[MBS_TCP_PDU_POS+3]) && (request_buffer_[MBS_TCP_PDU_POS+4] ==  response_buffer_[MBS_TCP_PDU_POS+4]))
        {
            // It's OK
        }
        else
        {
            Log_Sender::send_msg(QString("PMR response is not OK!"));
        }
    } 
}