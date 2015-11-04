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

#include "qmeegopixmapdata.h"
#include "qmeegoextensions.h"
#include "qmeegorasterpixmapdata.h"
#include <private/qimage_p.h>
#include <private/qwindowsurface_gl_p.h>
#include <private/qeglcontext_p.h>
#include <private/qapplication_p.h>
#include <private/qgraphicssystem_runtime_p.h>

// from dithering.cpp
extern unsigned short* convertRGB32_to_RGB565(const unsigned char *in, int width, int height, int stride);
extern unsigned short* convertARGB32_to_RGBA4444(const unsigned char *in, int width, int height, int stride);
extern unsigned char* convertBGRA32_to_RGBA32(const unsigned char *in, int width, int height, int stride);

static EGLint preserved_image_attribs[] = { EGL_IMAGE_PRESERVED_KHR, EGL_TRUE, EGL_NONE };

QHash <void*, QMeeGoImageInfo*> QMeeGoPixmapData::sharedImagesMap;

/* Public */

QMeeGoPixmapData::QMeeGoPixmapData() : QGLPixmapData(QPixmapData::PixmapType)
{
}

void QMeeGoPixmapData::fromTexture(GLuint textureId, int w, int h, bool alpha)
{
    resize(w, h); 
    texture()->id = textureId;
    m_hasAlpha = alpha;
    softImage = QImage();
}

QImage QMeeGoPixmapData::toImage() const
{
    return softImage;
}

void QMeeGoPixmapData::fromImage(const QImage &image,
                                 Qt::ImageConversionFlags flags)
{
    void *rawResource = static_cast <void *> (((QImage &) image).data_ptr()->data);

    if (sharedImagesMap.contains(rawResource)) {
        QMeeGoImageInfo *info = sharedImagesMap.value(rawResource);
        fromEGLSharedImage(info->handle, image);
    } else {
        // This should *never* happen since the graphics system should never
        // create a QMeeGoPixmapData for an origin that doesn't contain a raster
        // image we know about. But...
        qWarning("QMeeGoPixmapData::fromImage called on non-know resource. Falling back...");
        QGLPixmapData::fromImage(image, flags);
    }
}

void QMeeGoPixmapData::fromEGLSharedImage(Qt::HANDLE handle, const QImage &si)
{
    if (si.isNull())
        qFatal("Trying to build pixmap with an empty/null softimage!");
        
    QGLShareContextScope ctx(qt_gl_share_widget()->context());
   
    QMeeGoExtensions::ensureInitialized();
 
    bool textureIsBound = false;
    GLuint newTextureId;
    GLint newWidth, newHeight;

    glGenTextures(1, &newTextureId);
    glBindTexture(GL_TEXTURE_2D, newTextureId);
    
    EGLImageKHR image = QEgl::eglCreateImageKHR(QEgl::display(), EGL_NO_CONTEXT, EGL_SHARED_IMAGE_NOK,
                                                (EGLClientBuffer)handle, preserved_image_attribs);

    if (image != EGL_NO_IMAGE_KHR) {
        glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, image);
        GLint err = glGetError();
        if (err == GL_NO_ERROR)
            textureIsBound = true;
        
        QMeeGoExtensions::eglQueryImageNOK(QEgl::display(), image, EGL_WIDTH, &newWidth);
        QMeeGoExtensions::eglQueryImageNOK(QEgl::display(), image, EGL_HEIGHT, &newHeight);
          
        QEgl::eglDestroyImageKHR(QEgl::display(), image);
    }
        
    if (textureIsBound) {
        fromTexture(newTextureId, newWidth, newHeight, 
                    (si.hasAlphaChannel() && const_cast<QImage &>(si).data_ptr()->checkForAlphaPixels()));
        texture()->options &= ~QGLContext::InvertedYBindOption;
        softImage = si;
        QMeeGoPixmapData::registerSharedImage(handle, softImage);
    } else {
        qWarning("Failed to create a texture from a shared image!");
        glDeleteTextures(1, &newTextureId);
    }
}

Qt::HANDLE QMeeGoPixmapData::imageToEGLSharedImage(const QImage &image)
{
    QGLShareContextScope ctx(qt_gl_share_widget()->context());

    QMeeGoExtensions::ensureInitialized();

    GLuint textureId;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    if (image.hasAlphaChannel() && const_cast<QImage &>(image).data_ptr()->checkForAlphaPixels()) {
        void *converted = convertBGRA32_to_RGBA32(image.bits(), image.width(), image.height(), image.bytesPerLine());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, converted);
        free(converted);
    } else {
        void *converted = convertRGB32_to_RGB565(image.bits(), image.width(), image.height(), image.bytesPerLine());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, converted);
        free(converted);
    }

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    EGLImageKHR eglimage = QEgl::eglCreateImageKHR(QEgl::display(), QEglContext::currentContext(QEgl::OpenGL)->context(),
                                                                                                EGL_GL_TEXTURE_2D_KHR,
                                                                                                (EGLClientBuffer) textureId,
                                                                                                preserved_image_attribs);
    glDeleteTextures(1, &textureId);
    if (eglimage) {
        EGLNativeSharedImageTypeNOK handle = QMeeGoExtensions::eglCreateSharedImageNOK(QEgl::display(), eglimage, NULL);
        QEgl::eglDestroyImageKHR(QEgl::display(), eglimage);
        return (Qt::HANDLE) handle;
    } else {
        qWarning("Failed to create shared image from pixmap/texture!");
        return 0;
    }
}

void QMeeGoPixmapData::updateFromSoftImage()
{
    // FIXME That's broken with recent 16bit textures changes.
    m_dirty = true;
    m_source = softImage;
    ensureCreated();
    
    if (softImage.width() != w || softImage.height() != h)
        qWarning("Ooops, looks like softImage changed dimensions since last updated! Corruption ahead?!");
}

bool QMeeGoPixmapData::destroyEGLSharedImage(Qt::HANDLE h)
{
    QGLShareContextScope ctx(qt_gl_share_widget()->context());   
    QMeeGoExtensions::ensureInitialized();

    QMutableHashIterator <void*, QMeeGoImageInfo*> i(sharedImagesMap);
    while (i.hasNext()) {
        i.next();
        if (i.value()->handle == h)
            i.remove();
    }

    return QMeeGoExtensions::eglDestroySharedImageNOK(QEgl::display(), (EGLNativeSharedImageTypeNOK) h);
}

void QMeeGoPixmapData::registerSharedImage(Qt::HANDLE handle, const QImage &si)
{
    void *raw = static_cast <void *> (((QImage) si).data_ptr()->data);
    QMeeGoImageInfo *info;
    
    if (! sharedImagesMap.contains(raw)) {
        info = new QMeeGoImageInfo;
        info->handle = handle;
        info->rawFormat = si.format();
        sharedImagesMap.insert(raw, info);
    } else {
        info = sharedImagesMap.value(raw);
        if (info->handle != handle || info->rawFormat != si.format())
            qWarning("Inconsistency detected: overwriting entry in sharedImagesMap but handle/format different");
    }
}

QPixmapData *QMeeGoPixmapData::createCompatiblePixmapData() const
{
    return new QMeeGoRasterPixmapData(pixelType());
}
