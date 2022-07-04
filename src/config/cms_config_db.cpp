/* CMS_Config_DB Body.
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

#include "config/cms_config_db.h"

#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/Connector.h>
#include <Poco/Data/SQLite/Connector.h>

#include <exception>
#include <iostream>

using namespace std;
using namespace Poco::Data;

namespace cms
{
    CMS_Config_DB::CMS_Config_DB()
    {
    }

    std::string CMS_Config_DB::get_app_setting(const std::string& setting_name)
    {
        string setting_value;
        try
        {	
            Poco::Data::SQLite::Connector::registerConnector();
            Session session("SQLite", "cms.db");
            Statement select(session);

            select << "SELECT setting_value FROM app_setting_tbl where setting_name = ?",
                use(setting_name), into(setting_value);
            select.execute();
        }
        catch (const std::exception& exp)
        {
            cerr << "select failed: " << exp.what() << "\n";
            cerr << exp.what();
        }

        return setting_value;
    }

    void CMS_Config_DB::set_app_setting(const std::string& setting_name, const std::string& setting_value)
    {
        try
        {
            Poco::Data::SQLite::Connector::registerConnector();
            Session session("SQLite", "cms.db");
            Statement update(session);

            update << "UPDATE app_setting_tbl set setting_value = ? where setting_name = ?",
                use(setting_value), use(setting_name);
            if (update.execute() < 1) // if the setting isn't existed, insert a new 
            {
                Poco::Data::SQLite::Connector::registerConnector();
                Session session("SQLite", "cms.db");
                Statement insert(session);

                insert << "INSERT into app_setting_tbl (setting_name, setting_value) values (?, ?)",
                    use(setting_name), use(setting_value);
                insert.execute();
            }
        }
        catch (const std::exception& exp)
        {
            cerr << "update failed: " << exp.what() << "\n";
        }

    }

    CMS_Config_DB::~CMS_Config_DB()
    {
    }
} 