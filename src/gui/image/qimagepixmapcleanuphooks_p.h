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

#ifndef QIMAGEPIXMAPCLEANUPHOOKS_P_H
#define QIMAGEPIXMAPCLEANUPHOOKS_P_H

#include <QtGui/qpixmap.h>

QT_BEGIN_NAMESPACE

typedef void (*_qt_image_cleanup_hook_64)(qint64);
typedef void (*_qt_pixmap_cleanup_hook_pmd)(QPixmapData*);


class QImagePixmapCleanupHooks;

class Q_GUI_EXPORT QImagePixmapCleanupHooks
{
public:
    static QImagePixmapCleanupHooks *instance();

    static void enableCleanupHooks(const QImage &image);
    static void enableCleanupHooks(const QPixmap &pixmap);
    static void enableCleanupHooks(QPixmapData *pixmapData);

    static bool isImageCached(const QImage &image);
    static bool isPixmapCached(const QPixmap &pixmap);

    // Gets called when a pixmap data is about to be modified:
    void addPixmapDataModificationHook(_qt_pixmap_cleanup_hook_pmd);

    // Gets called when a pixmap data is about to be destroyed:
    void addPixmapDataDestructionHook(_qt_pixmap_cleanup_hook_pmd);

    // Gets called when an image is about to be modified or destroyed:
    void addImageHook(_qt_image_cleanup_hook_64);

    void removePixmapDataModificationHook(_qt_pixmap_cleanup_hook_pmd);
    void removePixmapDataDestructionHook(_qt_pixmap_cleanup_hook_pmd);
    void removeImageHook(_qt_image_cleanup_hook_64);

    static void executePixmapDataModificationHooks(QPixmapData*);
    static void executePixmapDataDestructionHooks(QPixmapData*);
    static void executeImageHooks(qint64 key);

private:
    QList<_qt_image_cleanup_hook_64> imageHooks;
    QList<_qt_pixmap_cleanup_hook_pmd> pixmapModificationHooks;
    QList<_qt_pixmap_cleanup_hook_pmd> pixmapDestructionHooks;
};

QT_END_NAMESPACE

#endif // QIMAGEPIXMAP_CLEANUPHOOKS_P_H
