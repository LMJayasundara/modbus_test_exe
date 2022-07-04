/* Modbus_Station body.
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

#include "modbus/station/modbus_station.h"
#include "modbus/address/addr_space.h"
#include "modbus/io/mbs_serial_io.h"
#include "modbus/io/mbs_tcp_io.h"
#include "modbus/diagram/mbs_serial_master.h"
#include "modbus/diagram/mbs_tcp_master.h"
#include "log/log_sender.h"
#include "cms_config.h"

#include <iostream>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#ifdef USE_NS_DEBUG
#include <iostream>
#endif

using namespace std;
using namespace boost;

namespace cms
{
    int Modbus_Station::DTOR_CNT = 0;

    Modbus_Station::Modbus_Station(Modbus_Station::Station_Type, Modbus_Station::Message_Type /*message_type*/)
        : signals_(mbs_io_service_)
    {
        signals_.add(SIGINT);
        signals_.add(SIGTERM);
#if defined(SIGQUIT)
        signals_.add(SIGQUIT);
#endif    //defined(SIGQUIT)
        signals_.async_wait(boost::bind(&Modbus_Station::handle_stop, this));
    }

    // initialize modbus station
    void Modbus_Station::init()
    {
    }

    void Modbus_Station::set_station_type(MBS_STATION_TYPE mbs_type)
    {
        mbs_type_ = mbs_type;
    }

    void Modbus_Station::run()
    {
        if ((mbs_type_ == MBS_SERIAL_MASTER) || (mbs_type_ == MBS_SERIAL_SLAVE))
        {
            mbs_serial_io_->start_session(mbs_type_);
        }
        else if ((mbs_type_ == MBS_TCP_MASTER) || (mbs_type_ == MBS_TCP_SLAVE))
        {
            mbs_tcp_io_->start_session(mbs_type_);
        }

        //mbs_io_service_.run();
        //io_service_thread_ = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&boost::asio::io_service::run, &mbs_io_service_)));
        boost::shared_ptr<boost::thread> io_service_thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&boost::asio::io_service::run, &mbs_io_service_)));
        //io_service_thread->join();
        //boost::thread t(boost::bind(&boost::asio::io_service::run, &mbs_io_service_));
    }

    void Modbus_Station::stop()
    {    
        if (mbs_tcp_io_.use_count() >= 1)
            this->mbs_tcp_io_->stop();

        if (mbs_serial_io_.use_count() >= 1)
            this->mbs_serial_io_->stop();

        if (!mbs_io_service_.stopped())
        {
            mbs_io_service_.reset();        
            //io_service_thread_->join();
        }
        //io_service_thread_->interrupt();
    }

    void Modbus_Station::handle_stop()
    {
        this->stop();
    }

    bool Modbus_Station::create_serial_io(const Serial_Port_Info& serial_port_info)
    {
        try
        {
            mbs_serial_io_ = boost::shared_ptr<Mbs_Serial_IO>(
                new Mbs_Serial_IO(mbs_io_service_, *this, serial_port_info));
        }
        catch (const std::exception &exp)
        {
            Log_Sender::send_msg(exp.what());
            return false;
        }

        return true;
    }

    bool Modbus_Station::create_tcp_io(const TCP_Port_Info& tcp_port_info)
    {
        try
        {
            mbs_tcp_io_ = boost::shared_ptr<Mbs_Tcp_IO>(
                new Mbs_Tcp_IO(mbs_io_service_, *this, tcp_port_info));
        }
        catch (const std::exception &exp)
        {
            Log_Sender::send_msg(exp.what());
            return false;
        }

        return true;
    }

    // get modbus point data
    // get modbus point's high byte(small address)
    MBS_BYTE Modbus_Station::get_point_data_hi(unsigned int index, enum MBS_FC addr_type)
    {
        MBS_BYTE data = 0x00;
        if (addr_type == REF_3X)
        {
            data = input_reg_addr_space_->get_point_high(index);
        }
        else if (addr_type == REF_4X)
        {
            data = holding_reg_addr_space_->get_point_high(index);
        }

        return data;
    }

    // get modbus point's low byte(big address)
    MBS_BYTE Modbus_Station::get_point_data_lo(unsigned int index, enum MBS_FC addr_type)
    {
        MBS_BYTE data = 0x00;
        if (addr_type == REF_3X)
        {
            data = input_reg_addr_space_->get_point_low(index);
        }
        else if (addr_type == REF_4X)
        {
            data = holding_reg_addr_space_->get_point_low(index);
        }

        return data;
    }

    // get modbus point's byte, only for REF_0X, REF_1X
    MBS_BYTE Modbus_Station::get_point_byte_data(unsigned int index, enum MBS_FC addr_type)
    {
        MBS_BYTE data = 0x00;
        if (addr_type == REF_0X)
        {
            data = coil_addr_space_->get_byte_data(index);
        }
        else if (addr_type == REF_1X)
        {
            data = dis_input_addr_space_->get_byte_data(index);
        }
        return data;
    }

    // get modbus point's bit, only for REF_0X, REF_1X
    bool Modbus_Station::get_point_bit_data(unsigned int index, enum MBS_FC addr_type)
    {
        bool bit_data;
        if (addr_type == REF_0X)
        {
            bit_data = coil_addr_space_->get_bit_data(index);
        }
        else if (addr_type == REF_1X)
        {
            bit_data = dis_input_addr_space_->get_bit_data(index);
        }
        return bit_data;
    };

    // set modbus point data
    // set modbus point's high byte(small address), only for REF_3X, REF_4X
    void Modbus_Station::set_point_data_hi(unsigned int index, unsigned char data_byte, enum MBS_FC addr_type)
    {
        if (addr_type == REF_3X)
        {
            input_reg_addr_space_->set_point_high(index, data_byte);
        }
        else if (addr_type == REF_4X)
        {
            holding_reg_addr_space_->set_point_high(index, data_byte);
        }
    }

    // set modbus point's low byte(big address), only for REF_3X, REF_4X
    void Modbus_Station::set_point_data_lo(unsigned int index, unsigned char data_byte, enum MBS_FC addr_type)
    {
        if (addr_type == REF_3X)
        {
            input_reg_addr_space_->set_point_low(index, data_byte);
        }
        else if (addr_type == REF_4X)
        {
            holding_reg_addr_space_->set_point_low(index, data_byte);
        }
    }

    // set modbus point's high and low bytes, only for REF_3X, REF_4X
    void Modbus_Station::set_point_data(unsigned int index, unsigned short data_byte, enum MBS_FC addr_type)
    {
        if ((addr_type == REF_3X) || (addr_type == REF_4X))
        {
            this->set_point_data_hi(index,static_cast<unsigned char>(data_byte >> 8),addr_type);
            this->set_point_data_lo(index,static_cast<unsigned char>(data_byte & 0x00FF),addr_type);
        }
    }

    // set modbus point's bit, only for REF_0X, REF_1X(when master use RIS)
    void Modbus_Station::set_point_bit_data(unsigned int index, bool is_set_true, enum MBS_FC addr_type)
    {
        if (addr_type == REF_0X)
        {
            coil_addr_space_->set_bit_data(index, is_set_true);
        }
        else if (addr_type == REF_1X)
        {
            dis_input_addr_space_->set_bit_data(index, is_set_true);
        }
    }

    bool Modbus_Station::load_master_diagram_group_config(const std::string &file_name, MBS_STATION_TYPE station_type)
    {
        ifstream input_file;
        input_file.open(file_name.c_str());
        if (input_file)
        {

            string data;
            typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
            boost::char_separator<char> sep(",");

            const unsigned int data_colum_cnt = 6;
            unsigned int data_line[data_colum_cnt];
            unsigned int index = 0;
            while (input_file)
            {
                input_file >> data;
                ++index;
                if (index == 1)
                    continue; // skip first line

                tokenizer tokens(data, sep);
                unsigned int i=0;
                for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
                {
                    if (i < data_colum_cnt)
                    {
                        try
                        {
                            data_line[i] = boost::lexical_cast<unsigned int>(tok_iter.current_token());
                            ++i;
                        }
                        catch(boost::bad_lexical_cast &)
                        {
                            Log_Sender::send_msg(QString("bad lexical cast"));
                        }
                    }
                }

                if (station_type == MBS_SERIAL_MASTER)
                {
                    boost::shared_ptr<Mbs_Serial_Master> serial_master = boost::shared_ptr<Mbs_Serial_Master>(new Mbs_Serial_Master(*this));
                    serial_master_group_.push_back(serial_master);
                    serial_master->set_slave_addr(data_line[0]);
                    serial_master->set_function_code(data_line[1]);
                    serial_master->set_point_start_addr(data_line[2]);
                    serial_master->set_point_count(data_line[3]);      
                    serial_master->set_retries(data_line[4]);  
                    serial_master->set_time_out(data_line[5]);
                }
                else // MBS_TCP_MASTER
                {
                    boost::shared_ptr<Mbs_Tcp_Master> tcp_master = boost::shared_ptr<Mbs_Tcp_Master>(new Mbs_Tcp_Master(*this));
                    tcp_master_group_.push_back(tcp_master);
                    tcp_master->set_slave_addr(data_line[0]);
                    tcp_master->set_function_code(data_line[1]);
                    tcp_master->set_point_start_addr(data_line[2]);
                    tcp_master->set_point_count(data_line[3]);      
                    tcp_master->set_retries(data_line[4]);  
                    tcp_master->set_time_out(data_line[5]);
                }
            }
            return true;
        }
        else
        {
            return false;
        }
    }

    // load file data to fill Addr_Space data
    // file format: column 0 - REF0X; column 1 - REF1X; column 2 - REF3X; column 3 - REF4X;
    void Modbus_Station::load_csv_data(const std::string &file_name)
    {
        ifstream input_file;
        input_file.open(file_name.c_str());
        string data;
        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        boost::char_separator<char> sep(",");

        const unsigned int data_colum_cnt = 4;
        unsigned short data_line[data_colum_cnt];
        unsigned int index = 0;
        while (input_file)
        {
            input_file >> data;
            tokenizer tokens(data, sep);
            unsigned int i=0;
            for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
            {
                if (i < data_colum_cnt)
                {
                    try
                    {
                        data_line[i] = boost::lexical_cast<unsigned short>(tok_iter.current_token());
                        ++i;
                    }
                    catch(bad_lexical_cast &)
                    {
                        cerr << "bad lexical cast";
                    }
                }
            }

            //unsigned short ref0x_data_block = data_line[0];
            //unsigned short ref1x_data_block = data_line[1];
            unsigned short ref3x_data = data_line[2];
            this->set_point_data(index, ref3x_data, REF_3X);
            unsigned short ref4x_data = data_line[3];
            this->set_point_data(index, ref4x_data, REF_4X);
            ++index;
        }

    }

    // set modbus address space reference
    void Modbus_Station::set_addr_space_ref(boost::shared_ptr<Addr_Space> coil_addr_space,boost::shared_ptr<Addr_Space> dis_input_addr_space,
        boost::shared_ptr<Addr_Space> input_reg_addr_space, boost::shared_ptr<Addr_Space> holding_reg_addr_space)
    {
        coil_addr_space_ = coil_addr_space;
        coil_addr_space_->fill_zero_data();
        dis_input_addr_space_ = dis_input_addr_space;
        dis_input_addr_space_->fill_zero_data();
        input_reg_addr_space_ = input_reg_addr_space;
        input_reg_addr_space_->fill_zero_data();
        holding_reg_addr_space_ = holding_reg_addr_space;
        holding_reg_addr_space_->fill_zero_data();
    }

    unsigned int Modbus_Station::get_serial_master_group_count() const
    {
        return this->serial_master_group_.size();
    }

    boost::shared_ptr<Mbs_Serial_Master> Modbus_Station::get_serial_master_group(unsigned int item_id) const
    {
        return this->serial_master_group_[item_id];
    }

    unsigned int Modbus_Station::get_tcp_master_group_count() const
    {
        return this->tcp_master_group_.size();
    }

    boost::shared_ptr<Mbs_Tcp_Master> Modbus_Station::get_tcp_master_group(unsigned int item_id) const
    {
        return this->tcp_master_group_[item_id];
    }

    Modbus_Station::~Modbus_Station()
    {
#ifdef USE_NS_DEBUG
        ++DTOR_CNT;
        std::cerr << "Modbus_Station DTOR count: " << DTOR_CNT << std::endl;
#endif
    }
} // namespace cms

