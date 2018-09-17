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
#include "mxlistmodel.h"

#include <QIcon>
#include <QVariant>

MxListModel::MxListModel()
{
}

MxListModel::~MxListModel()
{
}

QModelIndex	MxListModel::index ( int row,
                                int column,
                                const QModelIndex & parent) const
{
    if (row >= 0 && row < rowCount() && column >= 0 && column < columnCount())
    {
        QStandardItem* item = m_data.at(row);
        item->setSizeHint(QSize(80,130));
        return createIndex (row, column, (void *)(item));
    }
    return QModelIndex();
}

int MxListModel::rowCount ( const QModelIndex & parent  ) const
{
    return m_data.count();
}

int MxListModel::columnCount ( const QModelIndex & parent  ) const
{
    return m_title.count();
}
bool MxListModel::insertRows ( int row, int count, const QModelIndex & parent)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
        QStandardItem *item = new QStandardItem();
        item->setSizeHint(QSize(80,130));
//        item->setFont(QFont("微软雅黑",10,1));
//        QPixmap pix("://res/box-hover.png");
//        QBrush brush(pix);
//        item->setBackground(brush);
        m_data.insert (row,item);
    }
    endInsertRows();
    return true;
}

bool MxListModel::insertColumns ( int column, int count, const QModelIndex & )
{
    return true;
}

bool MxListModel::removeRows ( int row, int count, const QModelIndex & parent  )
{
    return true;
}

bool MxListModel::removeColumns ( int column, int count, const QModelIndex & parent )
{
    return true;
}

QVariant MxListModel::data ( const QModelIndex & index, int role  ) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    if (role == Qt::DisplayRole)
    {
        QStandardItem* item = m_data.at(index.row());
        item->setSizeHint(QSize(80,130));
        return QVariant(item->text());

    }
    if (role == Qt::DecorationRole)
    {
        QStandardItem* item = m_data.at(index.row());
        item->setSizeHint(QSize(80,130));
        return QVariant(item->icon());
    }

    if (role == Qt::WhatsThisRole)
    {
        QStandardItem* item = m_data.at(index.row());
        item->setSizeHint(QSize(80,130));
        return item->toolTip();
    }
    return QVariant();
}

bool MxListModel::setData ( const QModelIndex & index,
                           const QVariant & value,
                           int role)
{

    if (index.isValid() && role == Qt::EditRole)
    {
        QStandardItem* item = m_data.at(index.row());
        item->setSizeHint(QSize(80,130));
        item->setText(value.toString());
        emit dataChanged(index, index);
        return true;
    }
    if (role == Qt::DecorationRole)
    {
        QStandardItem* item = m_data.at(index.row());
        item->setSizeHint(QSize(80,130));
        item->setIcon(value.value<QIcon>());
         return true;
    }
    if (role == Qt::WhatsThisRole)
    {
      QStandardItem* item = m_data.at(index.row());
      item->setSizeHint(QSize(80,130));
      item->setToolTip(value.toString());
      return true;
    }
    return false;
}

QVariant MxListModel::headerData(int section, Qt::Orientation  orientation ,int role) const
{

    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return m_title.at(0);
    }
    return QVariant();
}
