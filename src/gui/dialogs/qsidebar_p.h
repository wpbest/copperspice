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

#ifndef QSIDEBAR_P_H
#define QSIDEBAR_P_H

#include <qlistwidget.h>
#include <qstandarditemmodel.h>
#include <qstyleditemdelegate.h>
#include <qurl.h>

#ifndef QT_NO_FILEDIALOG

QT_BEGIN_NAMESPACE

class QFileSystemModel;

class QSideBarDelegate : public QStyledItemDelegate
{
 public:
     QSideBarDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}
     void initStyleOption(QStyleOptionViewItem *option,
                          const QModelIndex &index) const;
};

class QUrlModel : public QStandardItemModel
{
    CS_OBJECT(QUrlModel)

public:
    enum Roles {
        UrlRole = Qt::UserRole + 1,
        EnabledRole = Qt::UserRole + 2
    };

    QUrlModel(QObject *parent = 0);

    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;

#ifndef QT_NO_DRAGANDDROP
    bool canDrop(QDragEnterEvent *event);
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
#endif

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);

    void setUrls(const QList<QUrl> &list);
    void addUrls(const QList<QUrl> &urls, int row = -1, bool move = true);
    QList<QUrl> urls() const;
    void setFileSystemModel(QFileSystemModel *model);
    bool showFullPath;

private :
    GUI_CS_SLOT_1(Private, void dataChanged(const QModelIndex & topLeft,const QModelIndex & bottomRight))
    GUI_CS_SLOT_2(dataChanged) 
    GUI_CS_SLOT_1(Private, void layoutChanged())
    GUI_CS_SLOT_2(layoutChanged) 

    void setUrl(const QModelIndex &index, const QUrl &url, const QModelIndex &dirIndex);
    void changed(const QString &path);
    void addIndexToWatch(const QString &path, const QModelIndex &index);
    QFileSystemModel *fileSystemModel;
    QList<QPair<QModelIndex, QString> > watching;
    QList<QUrl> invalidUrls;
};

class QSidebar : public QListView
{
    CS_OBJECT(QSidebar)

public:
    GUI_CS_SIGNAL_1(Public, void goToUrl(const QUrl & url))
    GUI_CS_SIGNAL_2(goToUrl,url) 

    QSidebar(QWidget *parent = 0);
    void setModelAndUrls(QFileSystemModel *model, const QList<QUrl> &newUrls);
    ~QSidebar();

    QSize sizeHint() const;

    void setUrls(const QList<QUrl> &list) { urlModel->setUrls(list); }
    void addUrls(const QList<QUrl> &list, int row) { urlModel->addUrls(list, row); }
    QList<QUrl> urls() const { return urlModel->urls(); }

    void selectUrl(const QUrl &url);

protected:
    bool event(QEvent * e);
    void focusInEvent(QFocusEvent *event);

#ifndef QT_NO_DRAGANDDROP
    void dragEnterEvent(QDragEnterEvent *event);
#endif

private:
    GUI_CS_SLOT_1(Private, void clicked(const QModelIndex & index))
    GUI_CS_SLOT_2(clicked) 

#ifndef QT_NO_MENU
    GUI_CS_SLOT_1(Private, void showContextMenu(const QPoint & position))
    GUI_CS_SLOT_2(showContextMenu) 
#endif

    GUI_CS_SLOT_1(Private, void removeEntry())
    GUI_CS_SLOT_2(removeEntry) 

    QUrlModel *urlModel;
    int m_selectUrl_processing;
};

QT_END_NAMESPACE

#endif // QT_NO_FILEDIALOG

#endif // QSIDEBAR_H
