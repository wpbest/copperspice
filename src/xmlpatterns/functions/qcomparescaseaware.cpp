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

#include "qvaluecomparison_p.h"

#include "qcomparescaseaware_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

ComparesCaseAware::ComparesCaseAware() : m_caseSensitivity(Qt::CaseSensitive)
{
}

Expression::Ptr ComparesCaseAware::compress(const StaticContext::Ptr &context)
{
    Q_ASSERT(m_operands.size() >= 2);

    if(ValueComparison::isCaseInsensitiveCompare(m_operands.first(), m_operands[1]))
        m_caseSensitivity = Qt::CaseInsensitive;
    else
    {
        /* Yes, we could probably skip this since m_caseSensitivity is initialized to this value,
         * but perhaps subsequent calls to compress() can make isCaseInsensitiveCompare() return
         * a different value. */
        m_caseSensitivity = Qt::CaseSensitive;
    }

    return FunctionCall::compress(context);
}

QT_END_NAMESPACE