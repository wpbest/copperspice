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

#ifndef Patternist_XsdAnnotated_H
#define Patternist_XsdAnnotated_H

#include "qxsdannotation_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{

    class XsdAnnotated
    {
        public:
            /**
             * Adds a new @p annotation to the component.
             */
            void addAnnotation(const XsdAnnotation::Ptr &annotation);

            /**
             * Adds a list of new @p annotations to the component.
             */
            void addAnnotations(const XsdAnnotation::List &annotations);

            /**
             * Returns the list of all annotations of the component.
             */
            XsdAnnotation::List annotations() const;

        private:
            XsdAnnotation::List m_annotations;
    };
}

QT_END_NAMESPACE

#endif