/* Mbs_Analyzer body.
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

/*******************************************************************************/
// first line [position 0] store shared memory's head info
// head info
// [MBSA] 4bytes | [current record row] 4bytes
// second line [position 1] to last store communication data 
// [record id] 4bytes | [record length] 4bytes | [comm type] 4bytes | [record content] 288bytes
/*******************************************************************************/

#include "modbus/analyzer/mbs_analyzer.h"
#include "modbus/config/mbs_def.h"

#include <Poco/Process.h>
#include <Poco/SharedMemory.h>
#include <Poco/NamedMutex.h>
#include <Poco/Mutex.h>

#include <strstream>
#include <iostream>

using namespace std;
using namespace Poco;

namespace cms
{
	std::string BYTE_STR_TABLE[256] = {"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F",
	                                                       "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F",
	                                                       "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F",
	                                                       "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F",
	                                                       "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F",
	                                                       "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F",
	                                                       "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F",
	                                                       "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F",
	                                                       "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
	                                                       "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F",
	                                                       "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF",
	                                                       "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF",
	                                                       "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF",
	                                                       "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF",
	                                                       "E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF",
	                                                       "F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF"};

    Mbs_Analyzer::Mbs_Analyzer(const std::string name)
    {
        strstream ss;
        Process::PID pid =  Process::id();
		ss << name;
        ss << "-";
        ss << pid;
        ss >> name_;
	}

    void Mbs_Analyzer::init()
    {
        comm_data_buffer_ = boost::shared_ptr<Poco::SharedMemory>
			(new Poco::SharedMemory(name_, MAX_RECORD_SIZE * (MAX_RECORD_COUNT+1), SharedMemory::AM_WRITE)); 
		//nmutex_ = boost::shared_ptr<Poco::Mutex>(new Poco::Mutex("4A4F064C-BA06-42E2-8AFB-21290A91C12B"));
		nmutex_ = boost::shared_ptr<Poco::Mutex>(new Poco::Mutex());
	}

	// commonly use once after init() to write initialize data
	void Mbs_Analyzer::init_data()
	{
		char *data=comm_data_buffer_->begin();
		data[0] = 'M'; data[1] = 'B'; data[2] = 'S'; data[3] = 'A';
		*(unsigned int*)(data + CURR_RECORD_ROW_POS) = 0;
	}

    void Mbs_Analyzer::print_buffer()
    {
        cout << "print data buffer\n";
        for (char *p=comm_data_buffer_->begin(); p!=comm_data_buffer_->end(); ++p)
        {
            cout << *p;
        }
        cout << "\n";
    }

	unsigned int Mbs_Analyzer::get_record_id(unsigned int row) const
	{
		Poco::Mutex::ScopedLock lock(*(this->nmutex_));
		char *data = comm_data_buffer_->begin();
		unsigned int id = *((unsigned int*)(data+((row + 1) * MAX_RECORD_SIZE)));
		return id;
	}

	unsigned int Mbs_Analyzer::get_record_content_length(unsigned int row) const
	{
		Poco::Mutex::ScopedLock lock(*(this->nmutex_));
		char *record_buf = comm_data_buffer_->begin();
		unsigned int length = *((unsigned int*)(record_buf+((row + 1) * MAX_RECORD_SIZE)+RECORD_LENGTH_OFFSET));
		return length;
	}

    unsigned int Mbs_Analyzer::get_comm_type(unsigned int row) const
    {
		Poco::Mutex::ScopedLock lock(*(this->nmutex_));
        char *record_buf = comm_data_buffer_->begin();
        unsigned int comm_type = *((unsigned int*)(record_buf+((row + 1) * MAX_RECORD_SIZE)+COMM_TYPE_OFFSET));
        return comm_type;
    }

    unsigned int Mbs_Analyzer::get_max_record_content() const
    {
        return MAX_RECORD_COUNT;
    }
    void Mbs_Analyzer::append_record(unsigned char *data, unsigned int length, MBS_DIAGRAM_TYPE comm_type)
    {
		Poco::Mutex::ScopedLock lock(*(this->nmutex_));
        char *record_buf = comm_data_buffer_->begin();
        unsigned int current_record_row = *(unsigned int*)(record_buf + CURR_RECORD_ROW_POS);
		//cout << "current_record_row: " << std::dec << current_record_row << "\n";
        unsigned int record_row_offset = (current_record_row % MAX_RECORD_COUNT + 1) * MAX_RECORD_SIZE;
		//cout << "record_row_offset: " << std::dec << record_row_offset << "\n";
		//std::copy(data, data+length, record_buf+record_row_offset+RECORD_CONTENT_OFFSET);
		for (unsigned int i=0; i<length; ++i)
		{
			*(record_buf+record_row_offset+RECORD_CONTENT_OFFSET+i) = *(data + i);
			//cout << "|" << (unsigned int)(*(record_buf+record_row_offset+RECORD_CONTENT_OFFSET+i)) << "|";
			//cout << "|" << (unsigned int)(*(data+i)) << "|";
		}
		++current_record_row;
      
		*(unsigned int*)(record_buf + CURR_RECORD_ROW_POS) = current_record_row;
		*(unsigned int*)(record_buf + record_row_offset) = current_record_row;
		*(unsigned int*)(record_buf + record_row_offset + RECORD_LENGTH_OFFSET) = length;		
        *(unsigned int*)(record_buf + record_row_offset + COMM_TYPE_OFFSET) = comm_type;		
    }

	std::string Mbs_Analyzer::get_record_content_by_str(unsigned int row) const
	{
		Poco::Mutex::ScopedLock lock(*(this->nmutex_));
		char *record_buf = comm_data_buffer_->begin();
		//cout << "magic data: " << record_buf[0] << record_buf[1] << record_buf[2] << record_buf[3] << "\n";
		//cout << "data: " << (unsigned int)(record_buf[308]) << (unsigned int)(record_buf[309]) << "\n";
		//unsigned int current_record_count = *(unsigned int*)(record_buf + CURR_RECORD_ROW_POS);
		//cout << "current_record_count: " << std::dec << current_record_count << "\n";
		unsigned int record_row_content_offset = (row + 1) * MAX_RECORD_SIZE + RECORD_CONTENT_OFFSET;
		//cout << "record_row_content_offset: " << std::dec << record_row_content_offset << "\n";
		unsigned int current_record_length = *(unsigned int*)(record_buf + (row + 1) * MAX_RECORD_SIZE + RECORD_LENGTH_OFFSET);
		//cout << "current_record_length: " << std::dec << current_record_length << "\n";
		//unsigned int current_record_id = *(unsigned int*)(record_buf + record_row_content_offset);
		//cout << "current_record_id: " << std::dec << current_record_id << "\n";
		string str;
		for (unsigned int i=0; i < current_record_length; ++i)
		{
			str.append(BYTE_STR_TABLE[(unsigned char)(*(record_buf + record_row_content_offset + i))]);
			str.append(" ");
			//cout << "|" << (unsigned int)(*(record_buf + record_row_offset + i)) << "|";		
		}
		//cout << "\n";
		return str;
	}

    Mbs_Analyzer::~Mbs_Analyzer()
    {
    }
}
