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

#include "qdeclarativepropertyvaluesource.h"

#include "qdeclarative.h"

QT_BEGIN_NAMESPACE

/*!
    \class QDeclarativePropertyValueSource
    \brief The QDeclarativePropertyValueSource class is an interface for property value sources such as animations and bindings.

    See \l{Property Value Sources} for information on writing custom property
    value sources.
 */

/*!
    Constructs a QDeclarativePropertyValueSource.
*/
QDeclarativePropertyValueSource::QDeclarativePropertyValueSource()
{
}

/*!
    Destroys the value source.
*/
QDeclarativePropertyValueSource::~QDeclarativePropertyValueSource()
{
}

/*!
    \fn void QDeclarativePropertyValueSource::setTarget(const QDeclarativeProperty &property)
    Set the target \a property for the value source.  This method will
    be called by the QML engine when assigning a value source.
*/

QT_END_NAMESPACE