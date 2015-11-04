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

#ifndef Patternist_XsdAttributeTerm_H
#define Patternist_XsdAttributeTerm_H

#include "qnamedschemacomponent_p.h"
#include "qxsdannotated_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
 
    class XsdAttributeTerm : public NamedSchemaComponent, public XsdAnnotated
    {
        public:
            typedef QExplicitlySharedDataPointer<XsdAttributeTerm> Ptr;

            /**
             * Returns @c true if the term is an attribute use, @c false otherwise.
             */
            virtual bool isAttributeUse() const;

            /**
             * Returns @c true if the term is an attribute use reference, @c false otherwise.
             *
             * @note The reference term is only used internally as helper during type resolving.
             */
            virtual bool isReference() const;
    };
}

QT_END_NAMESPACE

#endif