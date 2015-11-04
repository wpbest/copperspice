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

#ifndef QWAYLANDXCOMPOSITEGLXCONTEXT_H
#define QWAYLANDXCOMPOSITEGLXCONTEXT_H

#include <QtGui/QPlatformGLContext>

#include <QtCore/QWaitCondition>

#include "qwaylandbuffer.h"
#include "qwaylandxcompositeglxintegration.h"

#include "qglxconvenience.h"

class QWaylandXCompositeGLXWindow;
class QWaylandShmBuffer;

class QWaylandXCompositeGLXContext : public QPlatformGLContext
{
public:
    QWaylandXCompositeGLXContext(QWaylandXCompositeGLXIntegration *glxIntegration, QWaylandXCompositeGLXWindow *window);

    void makeCurrent();
    void doneCurrent();
    void swapBuffers();
    void* getProcAddress(const QString& procName);

    QPlatformWindowFormat platformWindowFormat() const;

    void geometryChanged();

private:
    QWaylandXCompositeGLXIntegration *mGlxIntegration;
    QWaylandXCompositeGLXWindow *mWindow;
    QWaylandBuffer *mBuffer;

    Window mXWindow;
    GLXFBConfig mConfig;
    GLXContext mContext;

    static void sync_function(void *data);
    void waitForSync();
    bool mWaitingForSyncCallback;
};

#endif // QWAYLANDXCOMPOSITEGLXCONTEXT_H
