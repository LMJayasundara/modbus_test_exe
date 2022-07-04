/* Modbus_Station header.
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


#ifndef CMS_MODBUS_STATION_H
#define CMS_MODBUS_STATION_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

#include "modbus/config/serial_port_info.h"
#include "modbus/config/tcp_port_info.h"
#include "modbus/config/mbs_def.h"

namespace boost
{
    class thread;
}

namespace cms
{
    class Mbs_Serial_IO;
    class Mbs_Tcp_IO;
    class Mbs_Serial_Master;
    class Mbs_Tcp_Master;

    class Addr_Space;

    // Class: Modbus_Station
    // Function: It can work as a modbus master/slave station
    class Modbus_Station
    {
        static int DTOR_CNT;
    public:
        enum Station_Type {MASTER, SLAVE, HYBIRD, UNKNOWN};
        Modbus_Station::Station_Type station_type;

        enum Message_Type {RTU, ASCII};
        Modbus_Station::Message_Type message_type;

        Modbus_Station(Modbus_Station::Station_Type = HYBIRD, Modbus_Station::Message_Type message_type = RTU);
        ~Modbus_Station();

        // initialize modbus station
        void init();

        void set_station_type(MBS_STATION_TYPE mbs_type);

        void run();

        void stop();

        bool create_serial_io(const Serial_Port_Info& serial_port_info);

        bool create_tcp_io(const TCP_Port_Info& tcp_port_info);

        // get modbus point data
        // get modbus point's high byte(small address), only for REF_3X, REF_4X
        MBS_BYTE get_point_data_hi(unsigned int index, enum MBS_FC addr_type);
        // get modbus point's low byte(big address), only for REF_3X, REF_4X
        MBS_BYTE get_point_data_lo(unsigned int index, enum MBS_FC addr_type);
        // get modbus point's byte, only for REF_0X, REF_1X
        MBS_BYTE get_point_byte_data(unsigned int index, enum MBS_FC addr_type);
        // get modbus point's bit, only for REF_0X, REF_1X
        bool get_point_bit_data(unsigned int index, enum MBS_FC addr_type);

        // set modbus point data
        // set modbus point's high byte(small address), only for REF_3X, REF_4X
        void set_point_data_hi(unsigned int index, unsigned char data_byte, enum MBS_FC addr_type);
        // set modbus point's low byte(big address), only for REF_3X, REF_4X
        void set_point_data_lo(unsigned int index, unsigned char data_byte, enum MBS_FC addr_type);
        // set modbus point's high and low bytes, only for REF_3X, REF_4X
        void set_point_data(unsigned int index, unsigned short data_byte, enum MBS_FC addr_type);
        // set modbus point's bit, only for REF_0X
        void set_point_bit_data(unsigned int index, bool is_set_true, enum MBS_FC addr_type);

        bool load_master_diagram_group_config(const std::string &file_name, MBS_STATION_TYPE station_type);

        // load file data to fill Addr_Space data
        void load_csv_data(const std::string &file_name);

        unsigned int get_serial_master_group_count() const;
        boost::shared_ptr<Mbs_Serial_Master> get_serial_master_group(unsigned int item_id) const; 

        unsigned int get_tcp_master_group_count() const;
        boost::shared_ptr<Mbs_Tcp_Master> get_tcp_master_group(unsigned int item_id) const;

    public:
        // set modbus address space reference
        void set_addr_space_ref(boost::shared_ptr<Addr_Space> coil_addr_space,boost::shared_ptr<Addr_Space> dis_input_addr_space,
            boost::shared_ptr<Addr_Space> input_reg_addr_space, boost::shared_ptr<Addr_Space> holding_reg_addr_space);

    private:
        void handle_stop();

        boost::shared_ptr<Addr_Space> coil_addr_space_;
        boost::shared_ptr<Addr_Space> dis_input_addr_space_;
        boost::shared_ptr<Addr_Space> input_reg_addr_space_;
        boost::shared_ptr<Addr_Space> holding_reg_addr_space_;

        boost::asio::io_service mbs_io_service_;

        boost::shared_ptr<Mbs_Serial_IO> mbs_serial_io_;
        boost::shared_ptr<Mbs_Tcp_IO> mbs_tcp_io_;

        std::vector<boost::shared_ptr<Mbs_Serial_Master> > serial_master_group_;
        std::vector<boost::shared_ptr<Mbs_Tcp_Master> > tcp_master_group_;

        boost::shared_ptr<boost::thread> io_service_thread_;
        /// The signal_set is used to register for process termination notifications.
        boost::asio::signal_set signals_;

        MBS_STATION_TYPE mbs_type_;
    };
} // namespace cms

#endif
