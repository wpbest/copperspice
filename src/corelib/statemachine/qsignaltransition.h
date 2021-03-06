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

#ifndef QSIGNALTRANSITION_H
#define QSIGNALTRANSITION_H

#include <QtCore/qabstracttransition.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_STATEMACHINE

class Q_CORE_EXPORT QSignalTransition : public QAbstractTransition
{
    CS_OBJECT(QSignalTransition)

    CORE_CS_PROPERTY_READ(senderObject, senderObject)
    CORE_CS_PROPERTY_WRITE(senderObject, setSenderObject)

public:
    QSignalTransition(QState *sourceState = 0);

    template<class SignalClass, class ...SignalArgs>
    QSignalTransition(QObject *sender, void (SignalClass::*signal)(SignalArgs...), QState *sourceState = 0);

    ~QSignalTransition();

    QObject *senderObject() const;
    void setSenderObject(QObject *sender);

    BentoAbstract *get_signalBento() const;
    
    void unregister();
    void maybeRegister();

    virtual void callOnTransition(QEvent *e);
  
protected:
    bool eventTest(QEvent *event);
    void onTransition(QEvent *event);
    bool event(QEvent *e);

private:
    Q_DISABLE_COPY(QSignalTransition)
   
    QObject *m_sender;
    QScopedPointer<BentoAbstract> m_signalBento;
};


template<class SignalClass, class ...SignalArgs>
QSignalTransition::QSignalTransition(QObject *sender, void (SignalClass::*signal)(SignalArgs...), QState *sourceState)
    : QAbstractTransition(sourceState)
{
    m_sender = sender;

    // store the signal method pointer in a CS Bento object 
    m_signalBento.reset(new Bento<void (SignalClass::*)(SignalArgs...)> {signal});
}

#endif //QT_NO_STATEMACHINE

QT_END_NAMESPACE

#endif
