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

#ifndef Patternist_IntersectIterator_H
#define Patternist_IntersectIterator_H

#include "qitem_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    class IntersectIterator : public Item::Iterator
    {
    public:     
        IntersectIterator(const Item::Iterator::Ptr &it1,                           const Item::Iterator::Ptr &it2);

        virtual Item next();
        virtual Item current() const;
        virtual xsInteger position() const;
        virtual Item::Iterator::Ptr copy() const;

    private:
        inline Item closedExit()
        {
            m_position = -1;
            m_current = Item();
            return Item();
        }

        const Item::Iterator::Ptr m_it1;
        const Item::Iterator::Ptr m_it2;
        Item m_current;
        xsInteger m_position;
        Item m_node1;
        Item m_node2;
    };
}

QT_END_NAMESPACE


#endif
