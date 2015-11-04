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

#ifndef QFILESYSTEMMODEL_P_H
#define QFILESYSTEMMODEL_P_H

#include "qfilesystemmodel.h"

#ifndef QT_NO_FILESYSTEMMODEL

#include <qabstractitemmodel_p.h>
#include <qabstractitemmodel.h>
#include "qfileinfogatherer_p.h"
#include <qpair.h>
#include <qdir.h>
#include <qicon.h>
#include <qdir.h>
#include <qicon.h>
#include <qfileinfo.h>
#include <qtimer.h>
#include <qhash.h>

QT_BEGIN_NAMESPACE

class ExtendedInformation;
class QFileSystemModelPrivate;
class QFileIconProvider;

class QFileSystemModelPrivate : public QAbstractItemModelPrivate
{
    Q_DECLARE_PUBLIC(QFileSystemModel)

public:
    class QFileSystemNode
    {
    public:
        QFileSystemNode(const QString &filename = QString(), QFileSystemNode *p = 0)
            : fileName(filename), populatedChildren(false), isVisible(false), dirtyChildrenIndex(-1), parent(p), info(0) {}
        ~QFileSystemNode() {
            QHash<QString, QFileSystemNode*>::const_iterator i = children.constBegin();
            while (i != children.constEnd()) {
                    delete i.value();
                    ++i;
            }
            delete info;
            info = 0;
            parent = 0;
        }

        QString fileName;
#if defined(Q_OS_WIN)
        QString volumeName;
#endif

        inline qint64 size() const { if (info && !info->isDir()) return info->size(); return 0; }
        inline QString type() const { if (info) return info->displayType; return QLatin1String(""); }
        inline QDateTime lastModified() const { if (info) return info->lastModified(); return QDateTime(); }
        inline QFile::Permissions permissions() const { if (info) return info->permissions(); return 0; }
        inline bool isReadable() const { return ((permissions() & QFile::ReadUser) != 0); }
        inline bool isWritable() const { return ((permissions() & QFile::WriteUser) != 0); }
        inline bool isExecutable() const { return ((permissions() & QFile::ExeUser) != 0); }
        inline bool isDir() const {
            if (info)
                return info->isDir();
            if (children.count() > 0)
                return true;
            return false;
        }
        inline bool isFile() const { if (info) return info->isFile(); return true; }
        inline bool isSystem() const { if (info) return info->isSystem(); return true; }
        inline bool isHidden() const { if (info) return info->isHidden(); return false; }
        inline bool isSymLink(bool ignoreNtfsSymLinks = false) const { return info && info->isSymLink(ignoreNtfsSymLinks); }
        inline bool caseSensitive() const { if (info) return info->isCaseSensitive(); return false; }
        inline QIcon icon() const { if (info) return info->icon; return QIcon(); }

        inline bool operator <(const QFileSystemNode &node) const {
            if (caseSensitive() || node.caseSensitive())
                return fileName < node.fileName;
            return QString::compare(fileName, node.fileName, Qt::CaseInsensitive) < 0;
        }
        inline bool operator >(const QString &name) const {
            if (caseSensitive())
                return fileName > name;
            return QString::compare(fileName, name, Qt::CaseInsensitive) > 0;
        }
        inline bool operator <(const QString &name) const {
            if (caseSensitive())
                return fileName < name;
            return QString::compare(fileName, name, Qt::CaseInsensitive) < 0;
        }
        inline bool operator !=(const QExtendedInformation &fileInfo) const {
            return !operator==(fileInfo);
        }
        bool operator ==(const QString &name) const {
            if (caseSensitive())
                return fileName == name;
            return QString::compare(fileName, name, Qt::CaseInsensitive) == 0;
        }
        bool operator ==(const QExtendedInformation &fileInfo) const {
            return info && (*info == fileInfo);
        }

        inline bool hasInformation() const { return info != 0; }

        void populate(const QExtendedInformation &fileInfo) {
            if (!info)
                info = new QExtendedInformation(fileInfo.fileInfo());
            (*info) = fileInfo;
        }

        // children shouldn't normally be accessed directly, use node()
        inline int visibleLocation(QString childName) {
            return visibleChildren.indexOf(childName);
        }
        void updateIcon(QFileIconProvider *iconProvider, const QString &path) {
            if (info)
                info->icon = iconProvider->icon(QFileInfo(path));
            QHash<QString, QFileSystemNode *>::const_iterator iterator;
            for(iterator = children.constBegin() ; iterator != children.constEnd() ; ++iterator) {
                //On windows the root (My computer) has no path so we don't want to add a / for nothing (e.g. /C:/)
                if (!path.isEmpty()) {
                    if (path.endsWith(QLatin1Char('/')))
                        iterator.value()->updateIcon(iconProvider, path + iterator.value()->fileName);
                    else
                        iterator.value()->updateIcon(iconProvider, path + QLatin1Char('/') + iterator.value()->fileName);
                } else
                    iterator.value()->updateIcon(iconProvider, iterator.value()->fileName);
            }
        }

        void retranslateStrings(QFileIconProvider *iconProvider, const QString &path) {
            if (info)
                info->displayType = iconProvider->type(QFileInfo(path));
            QHash<QString, QFileSystemNode *>::const_iterator iterator;
            for(iterator = children.constBegin() ; iterator != children.constEnd() ; ++iterator) {
                //On windows the root (My computer) has no path so we don't want to add a / for nothing (e.g. /C:/)
                if (!path.isEmpty()) {
                    if (path.endsWith(QLatin1Char('/')))
                        iterator.value()->retranslateStrings(iconProvider, path + iterator.value()->fileName);
                    else
                        iterator.value()->retranslateStrings(iconProvider, path + QLatin1Char('/') + iterator.value()->fileName);
                } else
                    iterator.value()->retranslateStrings(iconProvider, iterator.value()->fileName);
            }
        }

        bool populatedChildren;
        bool isVisible;
        QHash<QString,QFileSystemNode *> children;
        QList<QString> visibleChildren;
        int dirtyChildrenIndex;
        QFileSystemNode *parent;


        QExtendedInformation *info;

    };

    QFileSystemModelPrivate() :
            forceSort(true),
            sortColumn(0),
            sortOrder(Qt::AscendingOrder),
            readOnly(true),
            setRootPath(false),
            filters(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::AllDirs),
            nameFilterDisables(true), // false on windows, true on mac and unix
            disableRecursiveSort(false)
    {
        delayedSortTimer.setSingleShot(true);
    }

    void init();
    /*
      \internal

      Return true if index which is owned by node is hidden by the filter.
    */
    inline bool isHiddenByFilter(QFileSystemNode *indexNode, const QModelIndex &index) const
    {
       return (indexNode != &root && !index.isValid());
    }
    QFileSystemNode *node(const QModelIndex &index) const;
    QFileSystemNode *node(const QString &path, bool fetch = true) const;
    inline QModelIndex index(const QString &path) { return index(node(path)); }
    QModelIndex index(const QFileSystemNode *node) const;
    bool filtersAcceptsNode(const QFileSystemNode *node) const;
    bool passNameFilters(const QFileSystemNode *node) const;
    void removeNode(QFileSystemNode *parentNode, const QString &name);
    QFileSystemNode* addNode(QFileSystemNode *parentNode, const QString &fileName, const QFileInfo &info);
    void addVisibleFiles(QFileSystemNode *parentNode, const QStringList &newFiles);
    void removeVisibleFile(QFileSystemNode *parentNode, int visibleLocation);
    void sortChildren(int column, const QModelIndex &parent);

    inline int translateVisibleLocation(QFileSystemNode *parent, int row) const {
        if (sortOrder != Qt::AscendingOrder) {
            if (parent->dirtyChildrenIndex == -1)
                return parent->visibleChildren.count() - row - 1;

            if (row < parent->dirtyChildrenIndex)
                return parent->dirtyChildrenIndex - row - 1;
        }

        return row;
    }

    inline static QString myComputer() {
        // ### TODO We should query the system to find out what the string should be
        // XP == "My Computer",
        // Vista == "Computer",
        // OS X == "Computer" (sometime user generated) "Benjamin's PowerBook G4"
#ifdef Q_OS_WIN
        return QFileSystemModel::tr("My Computer");
#else
        return QFileSystemModel::tr("Computer");
#endif
    }

    inline void delayedSort() {
        if (!delayedSortTimer.isActive())
            delayedSortTimer.start(0);
    }

    static bool caseInsensitiveLessThan(const QString &s1, const QString &s2)
    {
       return QString::compare(s1, s2, Qt::CaseInsensitive) < 0;
    }

    static bool nodeCaseInsensitiveLessThan(const QFileSystemModelPrivate::QFileSystemNode &s1, const QFileSystemModelPrivate::QFileSystemNode &s2)
    {
       return QString::compare(s1.fileName, s2.fileName, Qt::CaseInsensitive) < 0;
    }

    QIcon icon(const QModelIndex &index) const;
    QString name(const QModelIndex &index) const;
    QString displayName(const QModelIndex &index) const;
    QString filePath(const QModelIndex &index) const;
    QString size(const QModelIndex &index) const;
    static QString size(qint64 bytes);
    QString type(const QModelIndex &index) const;
    QString time(const QModelIndex &index) const;

    void _q_directoryChanged(const QString &directory, const QStringList &list);
    void _q_performDelayedSort();
    void _q_fileSystemChanged(const QString &path, const QList<QPair<QString, QFileInfo> > &);
    void _q_resolvedName(const QString &fileName, const QString &resolvedName);

    static int naturalCompare(const QString &s1, const QString &s2, Qt::CaseSensitivity cs);

    QDir rootDir;
#ifndef QT_NO_FILESYSTEMWATCHER
    QFileInfoGatherer fileInfoGatherer;
#endif
    QTimer delayedSortTimer;
    bool forceSort;
    int sortColumn;
    Qt::SortOrder sortOrder;
    bool readOnly;
    bool setRootPath;
    QDir::Filters filters;
    QHash<const QFileSystemNode*, bool> bypassFilters;
    bool nameFilterDisables;
    //This flag is an optimization for the QFileDialog
    //It enable a sort which is not recursive, it means
    //we sort only what we see.
    bool disableRecursiveSort;
#ifndef QT_NO_REGEXP
    QList<QRegExp> nameFilters;
#endif
    // ### Qt5/resolvedSymLinks goes away
    QHash<QString, QString> resolvedSymLinks;

    QFileSystemNode root;

    QBasicTimer fetchingTimer;
    struct Fetching {
        QString dir;
        QString file;
        const QFileSystemNode *node;
    };
    QList<Fetching> toFetch;

};
#endif // QT_NO_FILESYSTEMMODEL

QT_END_NAMESPACE

#endif
