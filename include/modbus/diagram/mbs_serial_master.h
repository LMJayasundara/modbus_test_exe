/* Mbs_Serial_Master header.
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


#ifndef CMS_MBS_SERIAL_MASTER_H
#define CMS_MBS_SERIAL_MASTER_H

#include <vector>

namespace cms
{
	class Modbus_Station;

    // Class: Mbs_Serial_Master
    class Mbs_Serial_Master
    {
    public:
		bool is_last_request_success;
        unsigned int time_out; // ms

	public:
		explicit Mbs_Serial_Master(Modbus_Station &mbs_station);
		
        void set_slave_addr(unsigned char slave_addr);
        unsigned char get_slave_addr() const;

        void set_function_code(unsigned char function_code);
        unsigned char get_function_code() const;

        void set_point_start_addr(unsigned int point_start_addr);
        unsigned int get_point_start_addr() const;

        void set_point_count(unsigned int point_count);
        unsigned int get_point_count() const;

        void set_retries(unsigned int retries);
        unsigned int get_retries() const;

        void set_time_out(unsigned int time_out);
        unsigned int get_time_out() const;

        std::vector<unsigned char>& get_request_buffer();
		unsigned int get_request_length() const;

        std::vector<unsigned char>& get_response_buffer();
		unsigned int get_response_length() const;

        void make_request();
        void handle_response();

    private:
        static const unsigned int MBS_SERIAL_START_ADDR_POS = 2;
    	static const unsigned int MBS_SERIAL_PTSCNT_POS = 4;
        static const unsigned int MBS_SERIAL_REQUEST_DATA_POS = 7;
    	static const unsigned int MBS_SERIAL_RESPONSE_DATA_POS = 3;
        static const unsigned int MBS_SERIAL_SLAVE_ADDR_POS = 0;
        static const unsigned int MBS_SERIAL_FUNCTION_CODE_POS = 1;
        static const unsigned int MBS_SERIAL_RESPONSE_BYTE_CNT_POS = 2;

    private:
		void make_RCS_request();
		void make_RIS_request();
        void make_RHR_request();
		void make_RIR_request();
		void make_FSC_request();
		void make_PSR_request();
		void make_FMC_request();
		void make_PMR_request();

		void handle_RCS_response();
		void handle_RIS_response();
		void handle_RHR_response();
        void handle_RIR_response();
		void handle_FSC_response();
		void handle_PSR_response();
		void handle_FMC_response();
		void handle_PMR_response();

    private:
        unsigned char slave_addr_;
        unsigned char function_code_;
        unsigned int point_start_addr_;
        unsigned int point_count_;
        unsigned int byte_count_;
        unsigned int retries_;
        bool is_last_request_success_;
        unsigned int time_out_; // ms

        std::vector<unsigned char> request_buffer_;
		unsigned int mbs_serial_req_diagram_len_;
        std::vector<unsigned char> response_buffer_;
		unsigned int mbs_serial_rsp_diagram_len_;

		Modbus_Station &mbs_station_ref_;
    };
} // namespace cms

#endif
