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

#ifndef QMACINPUTCONTEXT_P_H
#define QMACINPUTCONTEXT_P_H
#include <qconfig.h>

#ifndef QT_NO_IM

#include "QtGui/qinputcontext.h"
#include "qt_mac_p.h"

QT_BEGIN_NAMESPACE

class Q_GUI_EXPORT QMacInputContext : public QInputContext
{
    CS_OBJECT(QMacInputContext)
    //Q_DECLARE_PRIVATE(QMacInputContext)
    void createTextDocument();
public:
    explicit QMacInputContext(QObject* parent = 0);
    virtual ~QMacInputContext();

    virtual void setFocusWidget(QWidget *w);
    virtual QString identifierName() { return QLatin1String("mac"); }
    virtual QString language();

    virtual void reset();

    virtual bool isComposing() const;

    static OSStatus globalEventProcessor(EventHandlerCallRef, EventRef, void *);
    static void initialize();
    static void cleanup();

    EventRef lastKeydownEvent() { return keydownEvent; }
    void setLastKeydownEvent(EventRef);
    QWidget *lastFocusWidget() const { return lastFocusWid; }
protected:
    void mouseHandler(int pos, QMouseEvent *);
private:
    bool composing;
    bool recursionGuard;
    TSMDocumentID textDocument;
    QString currentText;
    EventRef keydownEvent;
    QWidget *lastFocusWid;
};

QT_END_NAMESPACE
#endif // QT_NO_IM
#endif // QMACINPUTCONTEXT_P_H
