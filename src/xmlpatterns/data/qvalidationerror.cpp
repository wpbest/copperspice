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


#include "qvalidationerror_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

ValidationError::ValidationError(const QString &msg,
                                 const ReportContext::ErrorCode code) : m_message(msg),
                                                                        m_code(code)
{
}

AtomicValue::Ptr ValidationError::createError(const QString &description,
                                              const ReportContext::ErrorCode code)
{
    return ValidationError::Ptr(new ValidationError(description, code));
}

bool ValidationError::hasError() const
{
    return true;
}

QString ValidationError::stringValue() const
{
    Q_ASSERT_X(false, Q_FUNC_INFO, "stringValue() asked for ValidationError -- it makes no sense.");
    return QString();
}

QString ValidationError::message() const
{
    return m_message;
}

ItemType::Ptr ValidationError::type() const
{
    Q_ASSERT_X(false, Q_FUNC_INFO,
               "This function should never be called, the caller "
               "didn't check whether the AtomicValue was an ValidationError.");
    return ItemType::Ptr();
}

ReportContext::ErrorCode ValidationError::errorCode() const
{
    return m_code;
}

QT_END_NAMESPACE
