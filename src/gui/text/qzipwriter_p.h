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

#ifndef QZIPWRITER_P_H
#define QZIPWRITER_P_H

#ifndef QT_NO_TEXTODFWRITER

#include <QtCore/qstring.h>
#include <QtCore/qfile.h>

QT_BEGIN_NAMESPACE

class QZipWriterPrivate;

class Q_GUI_EXPORT QZipWriter
{
public:
    QZipWriter(const QString &fileName, QIODevice::OpenMode mode = (QIODevice::WriteOnly | QIODevice::Truncate) );

    explicit QZipWriter(QIODevice *device);
    ~QZipWriter();

    QIODevice* device() const;

    bool isWritable() const;
    bool exists() const;

    enum Status {
        NoError,
        FileWriteError,
        FileOpenError,
        FilePermissionsError,
        FileError
    };

    Status status() const;

    enum CompressionPolicy {
        AlwaysCompress,
        NeverCompress,
        AutoCompress
    };

    void setCompressionPolicy(CompressionPolicy policy);
    CompressionPolicy compressionPolicy() const;

    void setCreationPermissions(QFile::Permissions permissions);
    QFile::Permissions creationPermissions() const;

    void addFile(const QString &fileName, const QByteArray &data);

    void addFile(const QString &fileName, QIODevice *device);

    void addDirectory(const QString &dirName);

    void addSymLink(const QString &fileName, const QString &destination);

    void close();
private:
    QZipWriterPrivate *d;
    Q_DISABLE_COPY(QZipWriter)
};

QT_END_NAMESPACE

#endif // QT_NO_TEXTODFWRITER
#endif // QZIPWRITER_H