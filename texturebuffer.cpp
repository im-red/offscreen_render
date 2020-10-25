#include "texturebuffer.h"
#include "debug.h"

#include <QElapsedTimer>
#include <QOpenGLFunctions>

TextureBuffer *TextureBuffer::instance()
{
    static TextureBuffer s_instance;
    return &s_instance;
}

TextureBuffer::TextureBuffer()
    : m_ready(false)
    , m_texture(0)
{
}

TextureBuffer::~TextureBuffer()
{
}

void TextureBuffer::createTexture(QOpenGLContext *context)
{
    context->functions()->glGenTextures(1, &m_texture);
    m_ready = true;
}

void TextureBuffer::deleteTexture(QOpenGLContext *context)
{
    context->functions()->glDeleteTextures(1, &m_texture);
}

// called in render thread
void TextureBuffer::updateTexture(QOpenGLContext *context, int width, int height)
{
    RAIITimer t("ImageBuffer::updateTexture");
    QMutexLocker lock(&m_mutex);

    auto f = context->functions();
    f->glActiveTexture(GL_TEXTURE0);
    f->glBindTexture(GL_TEXTURE_2D, m_texture);
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    f->glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width, height, 0);
    f->glBindTexture(GL_TEXTURE_2D, 0);
    f->glFinish();
}

// called in main thread
void TextureBuffer::drawTexture(QOpenGLContext *context, int vertextCount)
{
    RAIITimer t("ImageBuffer::drawTexture");
    QMutexLocker lock(&m_mutex);

    auto f = context->functions();

    f->glBindTexture(GL_TEXTURE_2D, m_texture);

    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    f->glActiveTexture(GL_TEXTURE0);

    f->glDrawArrays(GL_TRIANGLES, 0, vertextCount);
    f->glBindTexture(GL_TEXTURE_2D, 0);

    //f->glFinish();
}
