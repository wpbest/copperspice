/***********************************************************************
*
* Copyright (c) 2012-2014 Barbara Geller
* Copyright (c) 2012-2014 Ansel Sermersheim
* Copyright (c) 2012-2014 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This file is part of CopperSpice.
*
* CopperSpice is free software: you can redistribute it and/or 
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with CopperSpice.  If not, see 
* <http://www.gnu.org/licenses/>.
*
***********************************************************************/

#ifndef QLISTWIDGET_P_H
#define QLISTWIDGET_P_H

#include <QtCore/qabstractitemmodel.h>
#include <QtGui/qabstractitemview.h>
#include <QtGui/qlistwidget.h>
#include <qitemdelegate.h>
#include <qlistview_p.h>
#include <qwidgetitemdata_p.h>

#ifndef QT_NO_LISTWIDGET

QT_BEGIN_NAMESPACE

class QListModelLessThan
{
public:
    inline bool operator()(QListWidgetItem *i1, QListWidgetItem *i2) const
        { return *i1 < *i2; }
};

class QListModelGreaterThan
{
public:
    inline bool operator()(QListWidgetItem *i1, QListWidgetItem *i2) const
        { return *i2 < *i1; }
};

class QListModel : public QAbstractListModel
{
    CS_OBJECT(QListModel)
public:
    QListModel(QListWidget *parent);
    ~QListModel();

    void clear();
    QListWidgetItem *at(int row) const;
    void insert(int row, QListWidgetItem *item);
    void insert(int row, const QStringList &items);
    void remove(QListWidgetItem *item);
    QListWidgetItem *take(int row);
    void move(int srcRow, int dstRow);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(QListWidgetItem *item) const;
    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    QMap<int, QVariant> itemData(const QModelIndex &index) const;

    bool insertRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count = 1, const QModelIndex &parent = QModelIndex());

    Qt::ItemFlags flags(const QModelIndex &index) const;

    void sort(int column, Qt::SortOrder order);
    void ensureSorted(int column, Qt::SortOrder order, int start, int end);
    static bool itemLessThan(const QPair<QListWidgetItem*,int> &left,
                             const QPair<QListWidgetItem*,int> &right);
    static bool itemGreaterThan(const QPair<QListWidgetItem*,int> &left,
                                const QPair<QListWidgetItem*,int> &right);
    static QList<QListWidgetItem*>::iterator sortedInsertionIterator(
        const QList<QListWidgetItem*>::iterator &begin,
        const QList<QListWidgetItem*>::iterator &end,
        Qt::SortOrder order, QListWidgetItem *item);

    void itemChanged(QListWidgetItem *item);

    // dnd
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
#ifndef QT_NO_DRAGANDDROP
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent);
    Qt::DropActions supportedDropActions() const;
#endif

    QMimeData *internalMimeData()  const;
private:
    QList<QListWidgetItem*> items;

    // A cache must be mutable if get-functions should have const modifiers
    mutable QModelIndexList cachedIndexes;
};



class QListWidgetPrivate : public QListViewPrivate
{
    Q_DECLARE_PUBLIC(QListWidget)
public:
    QListWidgetPrivate() : QListViewPrivate(), sortOrder(Qt::AscendingOrder), sortingEnabled(false) {}
    inline QListModel *listModel() const { return qobject_cast<QListModel*>(model); }
    void setup();
    void _q_emitItemPressed(const QModelIndex &index);
    void _q_emitItemClicked(const QModelIndex &index);
    void _q_emitItemDoubleClicked(const QModelIndex &index);
    void _q_emitItemActivated(const QModelIndex &index);
    void _q_emitItemEntered(const QModelIndex &index);
    void _q_emitItemChanged(const QModelIndex &index);
    void _q_emitCurrentItemChanged(const QModelIndex &current, const QModelIndex &previous);
    void _q_sort();
    void _q_dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    Qt::SortOrder sortOrder;
    bool sortingEnabled;
};

class QListWidgetItemPrivate
{
public:
    QListWidgetItemPrivate(QListWidgetItem *item) : q(item), theid(-1) {}
    QListWidgetItem *q;
    QVector<QWidgetItemData> values;
    int theid;
};

QT_END_NAMESPACE

#endif // QT_NO_LISTWIDGET

#endif // QLISTWIDGET_P_H