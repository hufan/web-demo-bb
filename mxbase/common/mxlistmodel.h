/******************************************************************************
* Copyright (C), 2016-2017, Sunny.Guo
* FileName:
* Author: Sunny.Guo
* Version: 1.0
* Date:
* Description:
*
* History:
*  <author>  	<time>   	<version >   	<desc>
*  Sunny.Guo   	19/01/2017      1.0     	create this moudle
*
* Licensed under GPLv2 or later, see file LICENSE in this source tree.
*******************************************************************************/
#ifndef MXLISTMODEL_H
#define MXLISTMODEL_H

#pragma once

#include <QAbstractListModel>
#include <QStandardItem>
#include <QList>

class MxListModel : public QAbstractListModel
{
public:
    MxListModel();
    ~MxListModel();
    virtual QModelIndex	index (int row, int column = 0, const QModelIndex & parent = QModelIndex()) const;
    QVariant data (const QModelIndex & index, int role = Qt::DisplayRole ) const;
    virtual bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    int rowCount (const QModelIndex & parent = QModelIndex() ) const;
    int columnCount (const QModelIndex & parent = QModelIndex() ) const;
    bool insertRows (int row, int count, const QModelIndex & parent = QModelIndex());
    bool insertColumns (int column, int count, const QModelIndex & parent = QModelIndex());
    bool removeRows (int row, int count, const QModelIndex & parent = QModelIndex() );
    bool removeColumns (int column, int count, const QModelIndex & parent = QModelIndex());
    QVariant headerData(int section,Qt::Orientation,int role) const;
    void setTitle(QStringList title) { m_title = title;}
    void setGuid(QString guid) { m_guid_list.append(guid);}
    QString getGuid(int index) { return m_guid_list.at(index);}

private:
    QList<QStandardItem*> m_data;
    QStringList  m_title;
    QStringList  m_guid_list;
};


#endif // MXLISTMODEL_H
