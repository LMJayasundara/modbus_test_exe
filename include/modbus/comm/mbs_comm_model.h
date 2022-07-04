/* Mbs_Comm_Model Header.
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

#ifndef CMS_MBS_COMM_MODEL_H
#define CMS_MBS_COMM_MODEL_H

#include <QStandardItemModel>
#include <QString>

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

namespace cms
{
    class Mbs_Analyzer;

    class Mbs_Comm_Model : public QStandardItemModel
    {
        Q_OBJECT
    public:
        static const int ID_INDEX = 0;
        static const int LENGTH_INDEX = 1;
        static const int TYPE_INDEX = 2;
        static const int CONTENT_INDEX = 3;
        static const int COLUMN_CNT = 4;

        unsigned int MAX_ROW_CNT;

        Mbs_Comm_Model(std::string mbs_analyzer_name, QObject *parent = 0);
        ~Mbs_Comm_Model();

        void init();

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
        virtual Qt::ItemFlags flags(const QModelIndex & index) const;

    public:

    private:
        std::string get_comm_type_str(unsigned int comm_type) const;

        std::vector<std::string> comm_types;

        boost::shared_ptr<Mbs_Analyzer> mbs_analyzer_;
    };
}

#endif