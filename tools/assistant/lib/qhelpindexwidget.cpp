/***********************************************************************
*
* Copyright (c) 2012-2013 Barbara Geller
* Copyright (c) 2012-2013 Ansel Sermersheim
* Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "qhelpindexwidget.h"
#include "qhelpenginecore.h"
#include "qhelpengine_p.h"
#include "qhelpdbreader_p.h"

#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtGui/QListView>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class QHelpIndexProvider : public QThread
{
public:
    QHelpIndexProvider(QHelpEnginePrivate *helpEngine);
    ~QHelpIndexProvider();
    void collectIndices(const QString &customFilterName);
    void stopCollecting();
    QStringList indices() const;
    QList<QHelpDBReader*> activeReaders() const;
    QSet<int> indexIds(QHelpDBReader *reader) const;

private:
    void run();

    QHelpEnginePrivate *m_helpEngine;
    QStringList m_indices;
    QList<QHelpDBReader*> m_activeReaders;
    QMap<QHelpDBReader*, QSet<int> > m_indexIds;
    QStringList m_filterAttributes;
    mutable QMutex m_mutex;
    bool m_abort;
};

class QHelpIndexModelPrivate
{
public:
    QHelpIndexModelPrivate(QHelpEnginePrivate *hE)
    {
        helpEngine = hE;
        indexProvider = new QHelpIndexProvider(helpEngine);
        insertedRows = 0;
    }

    QHelpEnginePrivate *helpEngine;
    QHelpIndexProvider *indexProvider;
    QStringList indices;
    int insertedRows;
    QString currentFilter;
    QList<QHelpDBReader*> activeReaders;
};

static bool caseInsensitiveLessThan(const QString &as, const QString &bs)
{
    return QString::compare(as, bs, Qt::CaseInsensitive) < 0;
}

QHelpIndexProvider::QHelpIndexProvider(QHelpEnginePrivate *helpEngine)
    : QThread(helpEngine)
{
    m_helpEngine = helpEngine;
    m_abort = false;
}

QHelpIndexProvider::~QHelpIndexProvider()
{
    stopCollecting();
}

void QHelpIndexProvider::collectIndices(const QString &customFilterName)
{
    m_mutex.lock();
    m_filterAttributes = m_helpEngine->q->filterAttributes(customFilterName);
    m_mutex.unlock();
    if (!isRunning()) {
        start(LowPriority);
    } else {
        stopCollecting();
        start(LowPriority);
    }
}

void QHelpIndexProvider::stopCollecting()
{
    if (!isRunning())
        return;
    m_mutex.lock();
    m_abort = true;
    m_mutex.unlock();
    wait();
    m_abort = false;
}

QStringList QHelpIndexProvider::indices() const
{
    QMutexLocker lck(&m_mutex);
    return m_indices;
}

QList<QHelpDBReader*> QHelpIndexProvider::activeReaders() const
{
    QMutexLocker lck(&m_mutex);
    return m_activeReaders;
}

QSet<int> QHelpIndexProvider::indexIds(QHelpDBReader *reader) const
{
    QMutexLocker lck(&m_mutex);
    if (m_indexIds.contains(reader))
        return m_indexIds.value(reader);
    return QSet<int>();
}

void QHelpIndexProvider::run()
{
    m_mutex.lock();
    QStringList atts = m_filterAttributes;
    m_indices.clear();
    m_activeReaders.clear();
    QSet<QString> indicesSet;
    m_mutex.unlock();

    foreach (const QString &dbFileName, m_helpEngine->fileNameReaderMap.keys()) {
        m_mutex.lock();
        if (m_abort) {
            m_mutex.unlock();
            return;
        }
        m_mutex.unlock();
        QHelpDBReader reader(dbFileName,
            QHelpGlobal::uniquifyConnectionName(dbFileName +
            QLatin1String("FromIndexProvider"),
            QThread::currentThread()), 0);
        if (!reader.init())
            continue;
        QStringList lst = reader.indicesForFilter(atts);
        if (!lst.isEmpty()) {
            m_mutex.lock();
            foreach (const QString &s, lst)
                indicesSet.insert(s);
            if (m_abort) {
                m_mutex.unlock();
                return;
            }
            QHelpDBReader *orgReader = m_helpEngine->fileNameReaderMap.value(dbFileName);
            m_indexIds.insert(orgReader, reader.indexIds(atts));
            m_activeReaders.append(orgReader);
            m_mutex.unlock();
        }
    }
    m_mutex.lock();
    m_indices = indicesSet.values();
    qSort(m_indices.begin(), m_indices.end(), caseInsensitiveLessThan);
    m_mutex.unlock();
}



/*!
    \class QHelpIndexModel
    \since 4.4
    \inmodule QtHelp
    \brief The QHelpIndexModel class provides a model that
    supplies index keywords to views.


*/

/*!
    \fn void QHelpIndexModel::indexCreationStarted()

    This signal is emitted when the creation of a new index
    has started. The current index is invalid from this
    point on until the signal indexCreated() is emitted.

    \sa isCreatingIndex()
*/

/*!
    \fn void QHelpIndexModel::indexCreated()

    This signal is emitted when the index has been created.
*/

QHelpIndexModel::QHelpIndexModel(QHelpEnginePrivate *helpEngine)
    : QStringListModel(helpEngine)
{
    d = new QHelpIndexModelPrivate(helpEngine);

    connect(d->indexProvider, SIGNAL(finished()), this, SLOT(insertIndices()));
    connect(helpEngine->q, SIGNAL(setupStarted()), this, SLOT(invalidateIndex()));
}

QHelpIndexModel::~QHelpIndexModel()
{
    delete d;
}

void QHelpIndexModel::invalidateIndex(bool onShutDown)
{
    if (onShutDown)
        disconnect(this, SLOT(insertIndices()));
    d->indexProvider->stopCollecting();
    d->indices.clear();
    if (!onShutDown)
        filter(QString());
}

/*!
    Creates a new index by querying the help system for
    keywords for the specified \a customFilterName.
*/
void QHelpIndexModel::createIndex(const QString &customFilterName)
{
    d->currentFilter = customFilterName;
    d->indexProvider->collectIndices(customFilterName);
    emit indexCreationStarted();
}

void QHelpIndexModel::insertIndices()
{
    d->indices = d->indexProvider->indices();
    d->activeReaders = d->indexProvider->activeReaders();
    QStringList attributes = d->helpEngine->q->filterAttributes(d->currentFilter);
    if (attributes.count() > 1) {
        foreach (QHelpDBReader *r, d->activeReaders)
            r->createAttributesCache(attributes, d->indexProvider->indexIds(r));
    }
    filter(QString());
    emit indexCreated();
}

/*!
    Returns true if the index is currently built up, otherwise
    false.
*/
bool QHelpIndexModel::isCreatingIndex() const
{
    return d->indexProvider->isRunning();
}

/*!
    Returns all hits found for the \a keyword. A hit consists of
    the URL and the document title.
*/
QMap<QString, QUrl> QHelpIndexModel::linksForKeyword(const QString &keyword) const
{
    QMap<QString, QUrl> linkMap;
    QStringList filterAttributes = d->helpEngine->q->filterAttributes(d->currentFilter);
    foreach (QHelpDBReader *reader, d->activeReaders)
        reader->linksForKeyword(keyword, filterAttributes, linkMap);
    return linkMap;
}

/*!
    Filters the indices and returns the model index of the best
    matching keyword. In a first step, only the keywords containing
    \a filter are kept in the model's index list. Analogously, if
    \a wildcard is not empty, only the keywords matched are left
    in the index list. In a second step, the best match is
    determined and its index model returned. When specifying a
    wildcard expression, the \a filter string is used to
    search for the best match.
*/
QModelIndex QHelpIndexModel::filter(const QString &filter, const QString &wildcard)
{
    if (filter.isEmpty()) {
        setStringList(d->indices);
        return index(-1, 0, QModelIndex());
    }

    QStringList lst;
    int goodMatch = -1;
    int perfectMatch = -1;

    if (!wildcard.isEmpty()) {
        QRegExp regExp(wildcard, Qt::CaseInsensitive);
        regExp.setPatternSyntax(QRegExp::Wildcard);
        foreach (const QString &index, d->indices) {
            if (index.contains(regExp)) {
                lst.append(index);
                if (perfectMatch == -1 && index.startsWith(filter, Qt::CaseInsensitive)) {
                    if (goodMatch == -1)
                        goodMatch = lst.count()-1;
                    if (filter.length() == index.length()){
                        perfectMatch = lst.count()-1;
                    }
                } else if (perfectMatch > -1 && index == filter) {
                    perfectMatch = lst.count()-1;
                }
            }
        }
    } else {
        foreach (const QString &index, d->indices) {
            if (index.contains(filter, Qt::CaseInsensitive)) {
                lst.append(index);
                if (perfectMatch == -1 && index.startsWith(filter, Qt::CaseInsensitive)) {
                    if (goodMatch == -1)
                        goodMatch = lst.count()-1;
                    if (filter.length() == index.length()){
                        perfectMatch = lst.count()-1;
                    }
                } else if (perfectMatch > -1 && index == filter) {
                    perfectMatch = lst.count()-1;
                }
            }
        }

    }

    if (perfectMatch == -1)
        perfectMatch = qMax(0, goodMatch);

    setStringList(lst);
    return index(perfectMatch, 0, QModelIndex());
}



/*!
    \class QHelpIndexWidget
    \inmodule QtHelp
    \since 4.4
    \brief The QHelpIndexWidget class provides a list view
    displaying the QHelpIndexModel.
*/

/*!
    \fn void QHelpIndexWidget::linkActivated(const QUrl &link,
        const QString &keyword)

    This signal is emitted when an item is activated and its
    associated \a link should be shown. To know where the link
    belongs to, the \a keyword is given as a second paremeter.
*/

/*!
    \fn void QHelpIndexWidget::linksActivated(const QMap<QString, QUrl> &links,
        const QString &keyword)

    This signal is emitted when the item representing the \a keyword
    is activated and the item has more than one link associated.
    The \a links consist of the document title and their URL.
*/

QHelpIndexWidget::QHelpIndexWidget()
    : QListView(0)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setUniformItemSizes(true);
    connect(this, SIGNAL(activated(QModelIndex)),
        this, SLOT(showLink(QModelIndex)));
}

void QHelpIndexWidget::showLink(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    QHelpIndexModel *indexModel = qobject_cast<QHelpIndexModel*>(model());
    if (!indexModel)
        return;
    QVariant v = indexModel->data(index, Qt::DisplayRole);
    QString name;
    if (v.isValid())
        name = v.toString();

    QMap<QString, QUrl> links = indexModel->linksForKeyword(name);
    if (links.count() == 1) {
        emit linkActivated(links.constBegin().value(), name);
    } else if (links.count() > 1) {
        emit linksActivated(links, name);
    }
}

/*!
    Activates the current item which will result eventually in
    the emitting of a linkActivated() or linksActivated()
    signal.
*/
void QHelpIndexWidget::activateCurrentItem()
{
    showLink(currentIndex());
}

/*!
    Filters the indices according to \a filter or \a wildcard.
    The item with the best match is set as current item.

    \sa QHelpIndexModel::filter()
*/
void QHelpIndexWidget::filterIndices(const QString &filter, const QString &wildcard)
{
    QHelpIndexModel *indexModel = qobject_cast<QHelpIndexModel*>(model());
    if (!indexModel)
        return;
    QModelIndex idx = indexModel->filter(filter, wildcard);
    if (idx.isValid())
        setCurrentIndex(idx);
}

QT_END_NAMESPACE
