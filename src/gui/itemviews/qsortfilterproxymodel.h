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

#ifndef QSORTFILTERPROXYMODEL_H
#define QSORTFILTERPROXYMODEL_H

#include <QtGui/qabstractproxymodel.h>

#ifndef QT_NO_SORTFILTERPROXYMODEL

#include <QtCore/qregexp.h>

QT_BEGIN_NAMESPACE

class QSortFilterProxyModelPrivate;
class QSortFilterProxyModelLessThan;
class QSortFilterProxyModelGreaterThan;

class Q_GUI_EXPORT QSortFilterProxyModel : public QAbstractProxyModel
{ 
    CS_OBJECT(QSortFilterProxyModel)

    GUI_CS_PROPERTY_READ(filterRegExp, filterRegExp)
    GUI_CS_PROPERTY_WRITE(filterRegExp, cs_setFilterRegExp)  

    GUI_CS_PROPERTY_READ(filterKeyColumn, filterKeyColumn)
    GUI_CS_PROPERTY_WRITE(filterKeyColumn, setFilterKeyColumn)

    GUI_CS_PROPERTY_READ(dynamicSortFilter, dynamicSortFilter)
    GUI_CS_PROPERTY_WRITE(dynamicSortFilter, setDynamicSortFilter)

    GUI_CS_PROPERTY_READ(filterCaseSensitivity, filterCaseSensitivity)
    GUI_CS_PROPERTY_WRITE(filterCaseSensitivity, setFilterCaseSensitivity)

    GUI_CS_PROPERTY_READ(sortCaseSensitivity, sortCaseSensitivity)
    GUI_CS_PROPERTY_WRITE(sortCaseSensitivity, setSortCaseSensitivity)

    GUI_CS_PROPERTY_READ(isSortLocaleAware, isSortLocaleAware)
    GUI_CS_PROPERTY_WRITE(isSortLocaleAware, setSortLocaleAware)

    GUI_CS_PROPERTY_READ(sortRole, sortRole)
    GUI_CS_PROPERTY_WRITE(sortRole, setSortRole)

    GUI_CS_PROPERTY_READ(filterRole, filterRole)
    GUI_CS_PROPERTY_WRITE(filterRole, setFilterRole)

    friend class QSortFilterProxyModelLessThan;
    friend class QSortFilterProxyModelGreaterThan;

public:
    QSortFilterProxyModel(QObject *parent = 0);
    ~QSortFilterProxyModel();

    void setSourceModel(QAbstractItemModel *sourceModel);

    QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;

    QItemSelection mapSelectionToSource(const QItemSelection &proxySelection) const;
    QItemSelection mapSelectionFromSource(const QItemSelection &sourceSelection) const;

    QRegExp filterRegExp() const;
    void setFilterRegExp(const QRegExp &regExp);
  
    // wrapper for overloaded method
    inline void cs_setFilterRegExp(const QRegExp &regExp); 
       
    int filterKeyColumn() const;
    void setFilterKeyColumn(int column);

    Qt::CaseSensitivity filterCaseSensitivity() const;
    void setFilterCaseSensitivity(Qt::CaseSensitivity cs);

    Qt::CaseSensitivity sortCaseSensitivity() const;
    void setSortCaseSensitivity(Qt::CaseSensitivity cs);

    bool isSortLocaleAware() const;
    void setSortLocaleAware(bool on);

    int sortColumn() const;
    Qt::SortOrder sortOrder() const;

    bool dynamicSortFilter() const;
    void setDynamicSortFilter(bool enable);

    int sortRole() const;
    void setSortRole(int role);

    int filterRole() const;
    void setFilterRole(int role);

    GUI_CS_SLOT_1(Public, void setFilterRegExp(const QString & pattern))
    GUI_CS_SLOT_OVERLOAD(setFilterRegExp,(const QString &))   

    GUI_CS_SLOT_1(Public, void setFilterWildcard(const QString & pattern))
    GUI_CS_SLOT_2(setFilterWildcard) 

    GUI_CS_SLOT_1(Public, void setFilterFixedString(const QString & pattern))
    GUI_CS_SLOT_2(setFilterFixedString) 

    GUI_CS_SLOT_1(Public, void clear())
    GUI_CS_SLOT_2(clear) 

    GUI_CS_SLOT_1(Public, void invalidate())
    GUI_CS_SLOT_2(invalidate) 

    using QObject::parent;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool setHeaderData(int section, Qt::Orientation orientation,
                  const QVariant &value, int role = Qt::EditRole);

    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                  int row, int column, const QModelIndex &parent);

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex());

    void fetchMore(const QModelIndex &parent);
    bool canFetchMore(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QModelIndex buddy(const QModelIndex &index) const;
    QModelIndexList match(const QModelIndex &start, int role,const QVariant &value, int hits = 1,
                  Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith|Qt::MatchWrap)) const;

    QSize span(const QModelIndex &index) const;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    QStringList mimeTypes() const;
    Qt::DropActions supportedDropActions() const;

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    virtual bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

    void filterChanged();
    void invalidateFilter();

private:
    Q_DECLARE_PRIVATE(QSortFilterProxyModel)
    Q_DISABLE_COPY(QSortFilterProxyModel)

    GUI_CS_SLOT_1(Private, void _q_sourceDataChanged(const QModelIndex & source_top_left,const QModelIndex & source_bottom_right))
    GUI_CS_SLOT_2(_q_sourceDataChanged)

    GUI_CS_SLOT_1(Private, void _q_sourceHeaderDataChanged(Qt::Orientation orientation,int start,int end))
    GUI_CS_SLOT_2(_q_sourceHeaderDataChanged)

    GUI_CS_SLOT_1(Private, void _q_sourceAboutToBeReset())
    GUI_CS_SLOT_2(_q_sourceAboutToBeReset)

    GUI_CS_SLOT_1(Private, void _q_sourceReset())
    GUI_CS_SLOT_2(_q_sourceReset)

    GUI_CS_SLOT_1(Private, void _q_sourceLayoutAboutToBeChanged())
    GUI_CS_SLOT_2(_q_sourceLayoutAboutToBeChanged)

    GUI_CS_SLOT_1(Private, void _q_sourceLayoutChanged())
    GUI_CS_SLOT_2(_q_sourceLayoutChanged)

    GUI_CS_SLOT_1(Private, void _q_sourceRowsAboutToBeInserted(const QModelIndex & source_parent,int start,int end))
    GUI_CS_SLOT_2(_q_sourceRowsAboutToBeInserted)

    GUI_CS_SLOT_1(Private, void _q_sourceRowsInserted(const QModelIndex & source_parent,int start,int end))
    GUI_CS_SLOT_2(_q_sourceRowsInserted)

    GUI_CS_SLOT_1(Private, void _q_sourceRowsAboutToBeRemoved(const QModelIndex & source_parent,int start,int end))
    GUI_CS_SLOT_2(_q_sourceRowsAboutToBeRemoved)

    GUI_CS_SLOT_1(Private, void _q_sourceRowsRemoved(const QModelIndex & source_parent,int start,int end))
    GUI_CS_SLOT_2(_q_sourceRowsRemoved)

    GUI_CS_SLOT_1(Private, void _q_sourceColumnsAboutToBeInserted(const QModelIndex & source_parent,int start,int end))
    GUI_CS_SLOT_2(_q_sourceColumnsAboutToBeInserted)

    GUI_CS_SLOT_1(Private, void _q_sourceColumnsInserted(const QModelIndex & source_parent,int start,int end))
    GUI_CS_SLOT_2(_q_sourceColumnsInserted)

    GUI_CS_SLOT_1(Private, void _q_sourceColumnsAboutToBeRemoved(const QModelIndex & source_parent,int start,int end))
    GUI_CS_SLOT_2(_q_sourceColumnsAboutToBeRemoved)

    GUI_CS_SLOT_1(Private, void _q_sourceColumnsRemoved(const QModelIndex & source_parent,int start,int end))
    GUI_CS_SLOT_2(_q_sourceColumnsRemoved)

    GUI_CS_SLOT_1(Private, void _q_clearMapping())
    GUI_CS_SLOT_2(_q_clearMapping)
   
};

void QSortFilterProxyModel::cs_setFilterRegExp(const QRegExp &regExp) 
{ 
   setFilterRegExp(regExp);
}

QT_END_NAMESPACE

#endif // QT_NO_SORTFILTERPROXYMODEL

#endif // QSORTFILTERPROXYMODEL_H