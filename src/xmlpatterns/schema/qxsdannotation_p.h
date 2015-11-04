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

#ifndef Patternist_XsdAnnotation_H
#define Patternist_XsdAnnotation_H

#include "qderivedstring_p.h"
#include "qxsdapplicationinformation_p.h"
#include "qxsddocumentation_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
 
    class XsdAnnotation : public NamedSchemaComponent
    {
        public:
            typedef QExplicitlySharedDataPointer<XsdAnnotation> Ptr;
            typedef QList<XsdAnnotation::Ptr> List;

            /**
             * Sets the @p id of the annotation.
             */
            void setId(const DerivedString<TypeID>::Ptr &id);

            /**
             * Returns the @p id of the annotation.
             */
            DerivedString<TypeID>::Ptr id() const;

            /**
             * Adds an application @p information to the annotation.
             *
             * The application information is meant to be interpreted by
             * a software system, e.g. other parts of the XML processor pipeline.
             */
            void addApplicationInformation(const XsdApplicationInformation::Ptr &information);

            /**
             * Returns the list of all application information of the annotation.
             */
            XsdApplicationInformation::List applicationInformation() const;

            /**
             * Adds a @p documentation to the annotation.
             *
             * The documentation is meant to be read by human being, e.g. additional
             * constraints or information about schema components.
             */
            void addDocumentation(const XsdDocumentation::Ptr &documentation);

            /**
             * Returns the list of all documentations of the annotation.
             */
            XsdDocumentation::List documentation() const;

        private:
            DerivedString<TypeID>::Ptr      m_id;
            XsdApplicationInformation::List m_applicationInformation;
            XsdDocumentation::List          m_documentations;
    };
}

QT_END_NAMESPACE

#endif
