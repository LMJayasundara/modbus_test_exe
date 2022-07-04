/* Diagram_Validator body.
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

#include <iostream>
#include <algorithm>

#include "modbus/diagram/diagram_validator.h"
#include "modbus/config/mbs_def.h"

using namespace std;

namespace cms
{
	Diagram_Validator::Diagram_Validator()
		: bytes_received_(0)
	{
		swap_buffer_.resize(DIAGRAM_MAX_LEN);
		discard_buffer_.resize(DIAGRAM_MAX_LEN);
	}

	// use when act as a slave station
	MBS_VALIDATE_RC Diagram_Validator::validate_serial_master_requst(std::vector<unsigned char>& recv_buffer, std::vector<unsigned char>& complete_buffer, unsigned int recv_bytes)
	{
		if (recv_bytes >= DIAGRAM_MAX_LEN)
		{
			return RECV_TOO_MANY; // discard all, it is impossible
		}

		if (bytes_received_ + recv_bytes >= DIAGRAM_MAX_LEN)
		{
			cout << "recv exceed, bytes_received_: " << bytes_received_ << ", recv_bytes: " << recv_bytes << "\n";
			discard_prev_recv_bytes_until_not_full(recv_buffer, complete_buffer, recv_bytes);
			return DISCARD_UNTIL_NOT_FULL;
		}
		else
		{
			// validate master request data depend on function code
			std::copy(recv_buffer.begin(), recv_buffer.begin() + recv_bytes, complete_buffer.begin() + bytes_received_);
			cout << "byte recv: " << bytes_received_ << "\n";
			bytes_received_ += recv_bytes;
			recv_bytes_cnt_queue_.push(recv_bytes);

			//bytes_received_ += recv_bytes;
			if (bytes_received_ > SERIAL_FUNCTION_CODE_POS) // at least 2 bytes
			{
				unsigned short function_code = complete_buffer[SERIAL_FUNCTION_CODE_POS];
				if (function_code >= RCS && function_code <= PSR)
				{
					if (bytes_received_ == 8) // RCS, RIS, RHR, RIR, FSC, PSR is 8 bytes lenght
					{
						return V_OK;
					}
					else if (bytes_received_ > 8)
					{
						discard_all_recv_bytes(recv_buffer, complete_buffer); 
						// uncorrect diagram, too long
						return DISCARD_ALL;
					}
					else
					{
						return RECV_AGAIN;
					}
				}
				else if ((function_code == FMC) || (function_code == PMR))
				{
					if (bytes_received_ <= SERIAL_BYTE_COUNT_POS)
					{
						return RECV_AGAIN;
					}

					unsigned short byte_count = complete_buffer[SERIAL_BYTE_COUNT_POS];
					if (bytes_received_ == (byte_count + 9)) // FMC, PMR is byte_count + 9 bytes
					{                        
						return V_OK;
					}
					else if (bytes_received_ > (byte_count + 9))
					{
						discard_all_recv_bytes(recv_buffer, complete_buffer); 
						// uncorrect diagram, too long
						return DISCARD_ALL;
					}
					else
					{
						return RECV_AGAIN;
					}
				}
				else
				{
					discard_all_recv_bytes(recv_buffer, complete_buffer);
					// unsupported function code or is not an function code
					return DISCARD_ALL;
				}
			}
			if (bytes_received_ < MIN_MBS_SERIAL_DIAGRAM_LENGTH)
				return RECV_AGAIN;
		}     
		return V_OK;
	}

	// use when act as a master station
	MBS_VALIDATE_RC Diagram_Validator::validate_serial_slave_response(std::vector<unsigned char>& recv_buffer, 
		std::vector<unsigned char>& complete_buffer, unsigned int recv_bytes,
		Function_Code mbs_fc, unsigned int response_length)
	{
		if (recv_bytes >= DIAGRAM_MAX_LEN)
		{
			return RECV_TOO_MANY; // discard all, it is impossible
		}

		if (bytes_received_ > response_length)
		{
			discard_fisrt_recv_bytes(recv_buffer, complete_buffer, recv_bytes);
			return DISCARD_FIRST;
		}
		else if (bytes_received_ == response_length)
		{
			// validate master request data depend on function code
			std::copy(recv_buffer.begin(), recv_buffer.begin() + recv_bytes, complete_buffer.begin() + bytes_received_);
			bytes_received_ += recv_bytes;
			recv_bytes_cnt_queue_.push(recv_bytes);
			//bytes_received_ += recv_bytes;
			if (bytes_received_ > SERIAL_FUNCTION_CODE_POS)
			{
				unsigned short function_code = complete_buffer[SERIAL_FUNCTION_CODE_POS];
				if (function_code == mbs_fc)
				{
					return V_OK;
				}
				else
				{
					return RECV_AGAIN;
				}
			}
			else
			{
				return RECV_AGAIN;
			}
		}    
		else // bytes_received_ + recv_bytes < response_length
		{
			return RECV_AGAIN;
		} 
		return V_OK;
	}

	void Diagram_Validator::clear_complete_buffer()
	{
		bytes_received_ = 0;
		recv_bytes_cnt_queue_.empty();
	}

	void Diagram_Validator::discard_fisrt_recv_bytes(std::vector<unsigned char>& recv_buffer, std::vector<unsigned char>& complete_buffer, unsigned int recv_bytes)
	{
		if (recv_bytes_cnt_queue_.empty())
			return;

		discard_bytes_count_ = 0;
		unsigned int fisrt_recv_bytes = recv_bytes_cnt_queue_.front();
		std::copy(complete_buffer.begin() + fisrt_recv_bytes, complete_buffer.begin() + bytes_received_, swap_buffer_.begin());
		std::copy(complete_buffer.begin(), complete_buffer.begin() + fisrt_recv_bytes, discard_buffer_.begin() + discard_bytes_count_);
		bytes_received_ -= fisrt_recv_bytes;
		discard_bytes_count_ += fisrt_recv_bytes;
		recv_bytes_cnt_queue_.pop();
		cout << "discard  bytes once: " << discard_bytes_count_ << "\n";
		std::copy(swap_buffer_.begin(), swap_buffer_.begin() + bytes_received_, complete_buffer.begin());
		std::copy(recv_buffer.begin(), recv_buffer.begin() + recv_bytes, complete_buffer.begin() + bytes_received_);
		recv_bytes_cnt_queue_.push(recv_bytes);
	}

	void Diagram_Validator::discard_prev_recv_bytes_until_not_full(std::vector<unsigned char>& recv_buffer, std::vector<unsigned char>& complete_buffer, unsigned int recv_bytes)
	{
		if (recv_bytes_cnt_queue_.empty())
			return;

		discard_bytes_count_ = 0;
		while (bytes_received_ + recv_bytes >= DIAGRAM_MAX_LEN)
		{
			unsigned int fisrt_recv_bytes = recv_bytes_cnt_queue_.front();
			std::copy(complete_buffer.begin() + fisrt_recv_bytes, complete_buffer.begin() + bytes_received_, swap_buffer_.begin());
			std::copy(complete_buffer.begin(), complete_buffer.begin() + fisrt_recv_bytes, discard_buffer_.begin() + discard_bytes_count_);
			bytes_received_ -= fisrt_recv_bytes;
			discard_bytes_count_ += fisrt_recv_bytes;
			recv_bytes_cnt_queue_.pop();
			std::copy(swap_buffer_.begin(), swap_buffer_.begin() + bytes_received_, complete_buffer.begin());
		}
		
		cout << "discard  bytes all: " << discard_bytes_count_ << "\n";
		std::copy(recv_buffer.begin(), recv_buffer.begin() + recv_bytes, complete_buffer.begin() + bytes_received_);
		recv_bytes_cnt_queue_.push(recv_bytes);
	}

	void Diagram_Validator::discard_all_recv_bytes(std::vector<unsigned char>& recv_buffer, std::vector<unsigned char>& complete_buffer)
	{
		discard_bytes_count_ = 0;
		while (!recv_bytes_cnt_queue_.empty())
		{
			discard_bytes_count_ = recv_bytes_cnt_queue_.front();
			recv_bytes_cnt_queue_.pop();
		}
		std::copy(complete_buffer.begin(), complete_buffer.begin() + discard_bytes_count_, discard_buffer_.begin());
		this->bytes_received_ = 0;
	}

	unsigned int Diagram_Validator::get_recv_bytes_count()
	{
		return this->bytes_received_;
	}

	std::vector<unsigned char>& Diagram_Validator::get_discard_buffer()
	{
		return this->discard_buffer_;
	}

	unsigned int Diagram_Validator::get_discard_bytes_count()
	{
		return this->discard_bytes_count_;
	}
} // namespace cms

