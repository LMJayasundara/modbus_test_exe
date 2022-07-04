/* Diagram_Validator header.
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


#ifndef CMS_DIAGRAM_VALIDATOR_H
#define CMS_DIAGRAM_VALIDATOR_H

#include <vector>
#include <queue>

#include "modbus/config/mbs_def.h"

namespace cms
{
    // Class: Diagram_Validator
    // Function: Validator diagram in recieved buffer
    class Diagram_Validator
    {
    public:
        Diagram_Validator();

        MBS_VALIDATE_RC validate_serial_master_requst(std::vector<unsigned char>& recv_buffer, 
            std::vector<unsigned char>& complete_buffer, unsigned int recv_bytes);
        MBS_VALIDATE_RC validate_serial_slave_response(std::vector<unsigned char>& recv_buffer, 
            std::vector<unsigned char>& complete_buffer, unsigned int recv_bytes,
            Function_Code mbs_fc, unsigned int response_length);

        void clear_complete_buffer();
        void discard_fisrt_recv_bytes(std::vector<unsigned char>& recv_buffer, std::vector<unsigned char>& complete_buffer, unsigned int recv_bytes);
		void discard_prev_recv_bytes_until_not_full(std::vector<unsigned char>& recv_buffer, std::vector<unsigned char>& complete_buffer, unsigned int recv_bytes);
		void discard_all_recv_bytes(std::vector<unsigned char>& recv_buffer, std::vector<unsigned char>& complete_buffer);

        unsigned int get_recv_bytes_count();
        std::vector<unsigned char>& get_discard_buffer();
        unsigned int get_discard_bytes_count();

    private:
        static const int SERIAL_FUNCTION_CODE_POS = 1;
        static const int SERIAL_BYTE_COUNT_POS = 6;
        unsigned int bytes_received_;

        unsigned int serial_slave_id_;
        bool ignored_serial_slave_id_;
        unsigned int discard_bytes_count_;

        std::vector<unsigned char> swap_buffer_;
        std::vector<unsigned char> discard_buffer_;
        std::queue<unsigned int> recv_bytes_cnt_queue_;
    };
} // namespace cms

#endif
