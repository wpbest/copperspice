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

#ifndef QOBJECT_H
#define QOBJECT_H

#include "csobject_macro.h"

// include before qmetaobject.h
#include "csregister1.h"

#include "qmetaobject.h"
#include "csregister2.h"

#include <QList>
#include <QRegExp>
#include <QPointer>
#include <QSharedPointer>
#include <QString>

#include <atomic>
#include <mutex>
#include <type_traits>

QT_BEGIN_NAMESPACE

typedef QList<QObject*>    QObjectList;

#if defined(QT_NO_KEYWORDS)
# define QT_NO_EMIT
#endif

#ifndef QT_NO_EMIT
#  define emit
#endif

#ifndef QT_NO_REGEXP
   class QRegExp;
#endif

class CSAbstractDeclarativeData;
class CSInternalChildren;
class CSInternalDeclarativeData;
class CSInternalEvents;
class CSInternalRefCount;
class CSInternalSender;
class CSInternalThreadData;

class QEvent;
class QChildEvent;
class QTimerEvent;
class QThread;
class QThreadData;

class Q_CORE_EXPORT QObject
{
   protected:
      // used in the CS_OBJECT macro to define cs_parent (ex:QObject) and cs_class (ex:MyClass)
      typedef QObject cs_class;

   private:
      CS_OBJECT_INTERNAL(QObject)

      CORE_CS_PROPERTY_READ(objectName,  objectName)
      CORE_CS_PROPERTY_WRITE(objectName, setObjectName)

      Q_DISABLE_COPY(QObject)

   public:
      CORE_CS_INVOKABLE_CONSTRUCTOR_1(Public, explicit QObject(QObject *parent = 0) )
      CORE_CS_INVOKABLE_CONSTRUCTOR_2(QObject, QObject *)

      virtual ~QObject();

      bool blockSignals(bool block);
      const QList<QObject *> &children() const;

      bool connect(const QObject *sender, const char *signalMethod, const char *location,
                     const char *slotMethod, Qt::ConnectionType type = Qt::AutoConnection);

      static bool connect(const QObject *sender, const char *signalMethod, const char *location,
                     const QObject *receiver, const char *slotMethod, Qt::ConnectionType type = Qt::AutoConnection);

      static bool connect(const QObject *sender, const char *signalMethod,
                     const QObject *receiver, const char *slotMethod, Qt::ConnectionType type = Qt::AutoConnection, 
                     const char *location = 0);

      static bool connect(const QObject *sender, const QMetaMethod &signalMethod,
                     const QObject *receiver, const QMetaMethod &slotMethod, Qt::ConnectionType type = Qt::AutoConnection);

      bool connect(const QObject *sender, const char *signalMethod, const char *slotMethod,
                     Qt::ConnectionType type = Qt::AutoConnection);

      static bool disconnect(const QObject *sender, const char *signalMethod,
                     const QObject *receiver, const char *slotMethod );

      static bool disconnect(const QObject *sender, const QMetaMethod &signalMethod,
                     const QObject *receiver, const QMetaMethod &slotMethod );

      static bool disconnect(const QObject *sender, const char *signalMethod, const char *location,
                     const QObject *receiver, const char *slotMethod );

      bool disconnect(const char *signalMethod = 0, const QObject *receiver = 0, const char *slotMethod = 0) const;
      bool disconnect(const char *signalMethod, const char *lineNumber, const QObject *receiver = 0, const char *slotMethod = 0) const;
      bool disconnect(const QObject *receiver, const char *slotMethod = 0) const;

      void dumpObjectTree();
      void dumpObjectInfo();

      QList<QByteArray> dynamicPropertyNames() const;

      virtual bool event(QEvent *);
      virtual bool eventFilter(QObject *watched, QEvent *event);
      void installEventFilter(QObject *obj);

      bool inherits(const char *classname) const;
      bool isWidgetType() const;
      void killTimer(int id);

      void moveToThread(QThread *targetThread);
      QString objectName() const;
      QObject *parent() const;
      QVariant property(const char *name) const;

      void removeEventFilter(QObject *obj);

      void setObjectName(const QString &name);
      void setParent(QObject *parent);
      bool setProperty(const char *name, const QVariant &value);     
      bool signalsBlocked() const;
      int startTimer(int interval);

		bool cs_InstanceOf(const char *iid);
      virtual bool cs_interface_query(const char *data) const;

      QThread *thread() const;

      // method ptr
      template<class Sender, class SignalClass, class ...SignalArgs, class Receiver, class SlotClass, class ...SlotArgs, class SlotReturn>
      static bool connect(const Sender *sender, void (SignalClass::*signalMethod)(SignalArgs...),
                        const Receiver *receiver, SlotReturn (SlotClass::*slotMethod)(SlotArgs...),
                        Qt::ConnectionType type = Qt::AutoConnection);

      // bento signal, method ptr slot
      template<class Receiver, class SlotClass, class ...SlotArgs, class SlotReturn>
      static bool connect(const QObject *sender, BentoAbstract *signalBento,
                        const Receiver *receiver, SlotReturn (SlotClass::*slotMethod)(SlotArgs...),
                        Qt::ConnectionType type = Qt::AutoConnection);

      // function ptr or lamda
      template<class Sender, class SignalClass, class ...SignalArgs, class Receiver, class T>
      static bool connect(const Sender *sender, void (SignalClass::*signalMethod)(SignalArgs...),
                        const Receiver *receiver, T slot, Qt::ConnectionType type = Qt::AutoConnection);

      // method ptr
      template<class Sender, class SignalClass, class ...SignalArgs, class Receiver, class SlotClass, class ...SlotArgs, class SlotReturn>
      static bool disconnect(const Sender *sender, void (SignalClass::*signalMethod)(SignalArgs...),
                        const Receiver *receiver, SlotReturn (SlotClass::*slotMethod)(SlotArgs...));

      // bento signal, method ptr slot
      template<class Receiver, class SlotClass, class ...SlotArgs, class SlotReturn>
      static bool disconnect(const QObject *sender, BentoAbstract *signalBento,
                        const Receiver *receiver, SlotReturn (SlotClass::*slotMethod)(SlotArgs...));
                       
      // function ptr only
      template<class Sender, class SignalClass, class ...SignalArgs, class Receiver, class T>
      static bool disconnect(const Sender *sender, void (SignalClass::*signalMethod)(SignalArgs...),
                        const Receiver *receiver, T slot);

      template<typename T>
      T findChild(const QString &aName = QString()) const;

      template<class T>
      QList<T> findChildren(const QString &objName = QString()) const;

      template<class T>
      QList<T> findChildren(const QRegExp &regExp) const;

      template<class T>
      T property(const char *name) const;

      CORE_CS_SIGNAL_1(Public, void destroyed(QObject *obj = 0))
      CORE_CS_SIGNAL_2(destroyed, obj)

      CORE_CS_SIGNAL_1(Public, void objectNameChanged(const QString &objectName))
      CORE_CS_SIGNAL_2(objectNameChanged, objectName)

      CORE_CS_SLOT_1(Public, void deleteLater())
      CORE_CS_SLOT_2(deleteLater)

   protected:
      virtual bool cs_isWidgetType() const;
      void cs_forceRemoveChild();

      virtual void childEvent(QChildEvent *event);

      virtual void connectNotify(const char *signalMethod) const;
      virtual void connectNotify(const QMetaMethod &signalMethod) const;

      virtual void customEvent(QEvent *event);

      virtual void disconnectNotify(const char *signal) const;
      virtual void disconnectNotify(const QMetaMethod &signal) const;

      virtual void timerEvent(QTimerEvent *event);

      bool isSignalConnected(const QMetaMethod &signalMethod) const;
      int receivers(const char *signal) const;
      QObject * sender() const;
      int senderSignalIndex() const;
       
		static QMap<std::type_index, QMetaObject *> &m_metaObjectsAll();      

   private:
      struct ConnectStruct {
         const QObject *sender;
         const BentoAbstract *signalMethod;
         const QObject *receiver;
         const BentoAbstract *slotMethod;
         Qt::ConnectionType type;      
      };

      struct SenderStruct
      {
          QObject *sender;
          int signal_index;
          int ref;                  
      };

      enum DisconnectType { DisconnectAll, DisconnectOne };

      //
      QObject *m_parent;
      QList<QObject *> m_children;

      SenderStruct *m_currentSender;   // object currently sending a signal

      // list of connections from my Signal to some Receiver
      mutable QList<ConnectStruct> m_connectList_ToReceiver;
      
      // list of connections from some Sender to my Slots
      mutable QList<ConnectStruct> m_connectList_FromSender;
            
      mutable std::mutex m_mutex_ToReceiver;
      mutable std::mutex m_mutex_FromSender;

      mutable int m_activateBusy;
      mutable int m_raceCount;

      QObject *m_currentChildBeingDeleted;
      CSAbstractDeclarativeData *m_declarativeData;

      QList< QPointer<QObject> > m_eventFilters;
   
      static std::atomic<int> m_objectCount;
      QString m_objectName;
      int m_postedEvents;

      mutable std::atomic<QtSharedPointer::ExternalRefCountData *> m_sharedRefCount;
      
      uint m_pendTimer           : 1;
      uint m_blockSig            : 1;
      uint m_wasDeleted          : 1;      

      uint m_sentChildRemoved    : 1;
      uint m_sendChildEvents     : 1;
      uint m_receiveChildEvents  : 1;

      std::atomic<bool> m_inThreadChangeEvent;

      // id of the thread which owns the object
      std::atomic<QThreadData *> m_threadData;

      QList<QByteArray> m_extra_propertyNames;
      QList<QVariant> m_extra_propertyValues;

      //
      void addConnection(const BentoAbstract *signalMethod, const QObject *receiver,
                         const BentoAbstract *slotMethod, Qt::ConnectionType type) const;

      void addObject();

      static bool check_parent_thread(QObject *parent, QThreadData *parentThreadData, QThreadData *currentThreadData);      

      void deleteChildren();

      static bool internal_disconnect(const QObject *sender,  const BentoAbstract *signalBento,
                        const QObject *receiver, const BentoAbstract *slotBento);

      bool isSender(const QObject *receiver, const char *signal) const;
      bool isSignalConnected(const BentoAbstract &signalMethod_Bento) const;

      void moveToThread_helper();

      QList<QObject *> receiverList(const char *signal) const;

      static void resetCurrentSender(QObject *receiver, SenderStruct *currentSender, SenderStruct *previousSender);
      void removeObject();

      static SenderStruct *setCurrentSender(QObject *receiver, SenderStruct *sender);
      void setThreadData_helper(QThreadData *currentData, QThreadData *targetData);
      QList<QObject *> senderList() const;

      static int *queuedConnectionTypes(const QList<QByteArray> &typeNames);

      //
      template<class T>
      void findChildren_helper(const QString &name, const QRegExp *regExp, QList<T> &list) const;

      CORE_CS_SLOT_1(Private, void internal_reregisterTimers(QList< std::pair<int, int> > timerList))
      CORE_CS_SLOT_2(internal_reregisterTimers)

      friend QMetaObject;
      friend CSInternalChildren;
      friend CSInternalDeclarativeData;
		friend CSInternalThreadData;
      friend CSInternalRefCount;
      friend CSInternalSender;
		friend CSInternalEvents;
};

template<class T>
T QObject::findChild(const QString &childName) const
{
   T retval = nullptr;
   QObject *temp;

   for (int i = 0; i < m_children.size(); ++i) {
      temp    = m_children.at(i);
      T child = dynamic_cast<T>(temp);

      if (child) {

         if (childName.isNull() || child->objectName() == childName) {
            retval = child;
            break;
         }

      }

      retval = temp->findChild<T>(childName);

      if (retval) {
         break;
      }
   }

   return retval;
}

template<class T>
QList<T> QObject::findChildren(const QString &objName) const
{
   QList<T> list;   
   this->findChildren_helper<T>(objName, nullptr, list);

   return list;
}

template<typename T>
QList<T> QObject::findChildren(const QRegExp &regExp) const
{
   QList<T> list;
   this->findChildren_helper<T>(QString(), &regExp, list);

   return list;
}

template<class T>
void QObject::findChildren_helper(const QString &name, const QRegExp *regExp, QList<T> &list) const
{
   QObject *temp;

   for (int i = 0; i < m_children.size(); ++i) {
      temp    = m_children.at(i);
      T child = dynamic_cast<T>(temp);

      if (child) {

         if (regExp) {

            if (regExp->indexIn(child->objectName()) != -1) {
               list.append(child);
            }

         } else {

            if (name.isNull() || child->objectName() == name) {
               list.append(child);
            }
         }
      }

      temp->findChildren_helper<T>(name, regExp, list);
   }
}

template<class T>
T QObject::property(const char *name) const
{
   const QMetaObject* meta = metaObject();

   if (! name || ! meta) {
      throw std::logic_error("QObject::property() Invalid name or meta object");
   }

   int id = meta->indexOfProperty(name);

   if (id < 0) {
      // dynamic property or does not exist
      const int k = m_extra_propertyNames.indexOf(name);

      if (k < 0) {
         std::string msg = std::string(meta->className()) + "::property() Property " + name + " is invalid or does not exist";
         throw std::invalid_argument(msg);
      }

      QVariant data = m_extra_propertyValues[k];

      if (data.canConvert<T>()) {
         return data.value<T>();

      } else {
         std::string msg = std::string(meta->className()) + "::property() Property " + name + " is not the correct type";
         throw std::invalid_argument(msg);
      }
   }

   QMetaProperty p = meta->property(id);

   if (! p.isReadable()) {
      qWarning("%s::property() Property \"%s\" is invalid or does not exist", meta->className(), name);
   }

   return p.read<T>(this);
}


template <class T>
inline T qobject_cast(QObject *object)
{
   return dynamic_cast<T>(object);
}

template <class T>
inline T qobject_cast(const QObject *object)
{
   return dynamic_cast<T>(object);
}

template <class T>
inline const char * qobject_interface_iid()
{
   return 0;
}

#ifndef QT_NO_DEBUG_STREAM
   Q_CORE_EXPORT QDebug operator<<(QDebug, const QObject *);
#endif


// **
class Q_CORE_EXPORT CSAbstractDeclarativeData
{
   public:
      static void (*destroyed)(CSAbstractDeclarativeData *, QObject *);
      static void (*parentChanged)(CSAbstractDeclarativeData *, QObject *, QObject *);
      static void (*signalEmitted)(CSAbstractDeclarativeData *, QObject *, int, void **);
      static int  (*receivers)(CSAbstractDeclarativeData *, const QObject *, int);
};


// **
class Q_CORE_EXPORT CSGadget_Fake_Parent
{
   public:
      static const QMetaObject & staticMetaObject();      
		static QMap<std::type_index, QMetaObject *> &m_metaObjectsAll();
};


// **
class Q_CORE_EXPORT CSInternalChildren
{
	private:      
      static void deleteChildren(QObject *object);
      static void moveChildren(QObject *object, int from, int to);
      static void removeOne(QObject *object, QObject *value);
      static void set_mParent(QObject *object, QObject *value);

   friend class QWidget;
   friend class QWidgetPrivate;
};


class Q_CORE_EXPORT CSInternalDeclarativeData
{
	private:
		static CSAbstractDeclarativeData * get_m_declarativeData(const QObject *object);
      static void set_m_declarativeData(QObject *object, CSAbstractDeclarativeData *value);

   friend class QWidget;
   friend class QGraphicsItem;
};


class Q_CORE_EXPORT CSInternalEvents
{
	private:
		static int get_m_PostedEvents(const QObject *object);
		static void incr_PostedEvents(QObject *object);
		static void decr_PostedEvents(QObject *object);

      static bool get_m_sendChildEvents(const QObject *object);
      static bool get_m_receiveChildEvents(const QObject *object);
      static void set_m_sendChildEvents(QObject *object, bool data);
      static void set_m_receiveChildEvents(QObject *object, bool data);

      static QList<QPointer<QObject> > & get_m_EventFilters(QObject *object);
		static std::atomic<bool> & get_m_inThreadChangeEvent(QObject *object);
			
   friend class QApplication;
	friend class QCoreApplication;
	friend class QCoreApplicationPrivate;
   friend class QEventDispatcherMac;
   friend class QEventDispatcherMacPrivate;
	friend class QEventDispatcherWin32Private;
   friend class QFocusFramePrivate;
   friend class QStateMachinePrivate;
   friend class QTimerInfoList;
   friend class QThreadData;
   friend class QWidget;
   friend class QWidgetPrivate;

   friend struct QDeclarativeGraphics_DerivedObject;
   
};

class Q_CORE_EXPORT CSInternalRefCount
{
	private:
		static bool get_m_wasDeleted(const QObject *object);
		static std::atomic<QtSharedPointer::ExternalRefCountData *> & get_m_SharedRefCount(const QObject *object);   

      static void set_m_wasDeleted(QObject *object, bool data);
 
   friend class QGraphicsItem;
   friend class QtFriendlyLayoutWidget;
	friend class QtSharedPointer::ExternalRefCountData;

};

class Q_CORE_EXPORT CSInternalSender
{
	private:
      static bool isSender(const QObject *object, const QObject *receiver, const char *signal);    
      static QObjectList receiverList(const QObject *object, const char *signal);
      static QObjectList senderList(const QObject *object);
        
   friend class QACConnectionObject;
};

#ifdef QT_BUILD_GUI_LIB
static QThreadData * internal_get_ThreadData(QObject *object);
#endif

class Q_CORE_EXPORT CSInternalThreadData
{
	private:
		static QThreadData * get_m_ThreadData(const QObject *object);		
      static std::atomic<QThreadData *> & get_AtomicThreadData(QObject *object);

   friend class QApplication;
	friend class QAbstractEventDispatcher;
   friend class QAbstractEventDispatcherPrivate;
   friend class QAbstractSocket;
   friend class QAbstractSocketPrivate;
   friend class QCoreApplication;
	friend class QCoreApplicationPrivate;
	friend class QEventLoop;
   friend class QEventLoopPrivate;
   friend class QEventDispatcherMac;
   friend class QEventDispatcherMacPrivate;
   friend class QEventDispatcherUNIX;
   friend class QEventDispatcherUNIXPrivate;
   friend class QEventDispatcherWin32;
   friend class QEventDispatcherWin32Private;
   friend class QNativeSocketEngine;
   friend class QProcessPrivate;
	friend class QSocketNotifier;
   friend class QTimerInfoList;	
	friend class QWinEventNotifier;

#ifdef QT_BUILD_GUI_LIB
   friend QThreadData * internal_get_ThreadData(QObject *object);
#endif

};


QT_END_NAMESPACE

// best way to handle declarations
#include "csobject_internal.h"

#endif
