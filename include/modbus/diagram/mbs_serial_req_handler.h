/* Mbs_Serial_Req_Handler header.
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


#ifndef CMS_MBS_SERIAL_REQ_HANDLER_H
#define CMS_MBS_SERIAL_REQ_HANDLER_H

#include <vector>

namespace cms
{
    class Mbs_Serial_Request;
    class Mbs_Serial_Response;
    class Modbus_Station;

    class Mbs_Serial_Req_Handler
    {
    public:
        static const unsigned int MBS_SERIAL_START_ADDR_POS = 2;
        static const unsigned int MBS_SERIAL_PTSCNT_POS = 4;
        static const unsigned int MBS_SERIAL_REQUEST_DATA_POS = 7;
        static const unsigned int MBS_SERIAL_RESPONSE_DATA_POS = 3;

        // Handle a modbus master request and produce a response.
        void handle_request(const Mbs_Serial_Request &request, Mbs_Serial_Response &response, Modbus_Station &mbs_station);

        // Handle a modbus RCS request and produce a response.
        void handle_RCS_request(const Mbs_Serial_Request &request, Mbs_Serial_Response &response, Modbus_Station &mbs_station);

        // Handle a modbus RIS request and produce a response.
        void handle_RIS_request(const Mbs_Serial_Request &request, Mbs_Serial_Response &response, Modbus_Station &mbs_station);

        // Handle a modbus RHR request and produce a response.
        void handle_RHR_request(const Mbs_Serial_Request &request, Mbs_Serial_Response &response, Modbus_Station &mbs_station);

        // Handle a modbus RIR request and produce a response.
        void handle_RIR_request(const Mbs_Serial_Request &request, Mbs_Serial_Response &response, Modbus_Station &mbs_station);

        // Handle a modbus FSC request and produce a response.
        void handle_FSC_request(const Mbs_Serial_Request &request, Mbs_Serial_Response &response, Modbus_Station &mbs_station);

        // Handle a modbus PSR request and produce a response.
        void handle_PSR_request(const Mbs_Serial_Request &request, Mbs_Serial_Response &response, Modbus_Station &mbs_station);

        // Handle a modbus FMC request and produce a response.
        void handle_FMC_request(const Mbs_Serial_Request &request, Mbs_Serial_Response &response, Modbus_Station &mbs_station);

        // Handle a modbus PMR request and produce a response.
        void handle_PMR_request(const Mbs_Serial_Request &request, Mbs_Serial_Response &response, Modbus_Station &mbs_station);

    private:   
        unsigned int starting_addr_;
        unsigned int num_of_points_;
    };

} // namespace cms

#endif
