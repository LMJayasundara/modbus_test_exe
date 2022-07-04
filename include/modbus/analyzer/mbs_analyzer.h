/* Mbs_Analyzer header.
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


#ifndef CMS_MBS_ANALYZER_H
#define CMS_MBS_ANALYZER_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "modbus/config/mbs_def.h"

namespace Poco
{
    class SharedMemory;
	class NamedMutex;
	class Mutex;
}

namespace cms
{
    class Mbs_Analyzer
    {
    public:
        explicit Mbs_Analyzer(const std::string name);
        ~Mbs_Analyzer();

        void init();

        // commonly use once after init() to write initialize data
        void init_data();

        void print_buffer();

        void append_record(unsigned char *data, unsigned int length, MBS_DIAGRAM_TYPE comm_type);

        unsigned int get_record_id(unsigned int row) const;
        unsigned int get_record_content_length(unsigned int row) const;
        unsigned int get_comm_type(unsigned int row) const;
        unsigned int get_max_record_content() const;
        std::string get_record_content_by_str(unsigned int row) const;

    private:
        static const unsigned int MAX_RECORD_SIZE = 312;
        static const unsigned int MAX_RECORD_COUNT = 1024;

        static const unsigned int CURR_RECORD_ROW_POS = 4;

        static const unsigned int RECORD_LENGTH_OFFSET = 4;
        static const unsigned int COMM_TYPE_OFFSET = 8;
        static const unsigned int RECORD_CONTENT_OFFSET = 12;
        static const unsigned int MAX_RECORD_CONTENT_SIZE = 300; // MAX_RECORD_CONTENT_SIZE = MAX_RECORD_SIZE - RECORD_CONTENT_OFFSET

        std::string name_;
        boost::shared_ptr<Poco::SharedMemory> comm_data_buffer_;
		boost::shared_ptr<Poco::Mutex> nmutex_;
    };
}

#endif