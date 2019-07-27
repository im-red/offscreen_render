#ifndef TEXTUREBUFFER_H
#define TEXTUREBUFFER_H

#include <QMutex>
#include <QOpenGLContext>

class TextureBuffer
{
public:
    static TextureBuffer *instance();

    bool ready() const { return m_ready; }

    void createTexture(QOpenGLContext *context);
    void deleteTexture(QOpenGLContext *context);

    void updateTexture(QOpenGLContext *context, int width, int height);
    void drawTexture(QOpenGLContext *context, int vertextCount);

private:
    TextureBuffer();
    ~TextureBuffer();

private:
    TextureBuffer(const TextureBuffer &) = delete;
    TextureBuffer &operator =(const TextureBuffer &) = delete;
    TextureBuffer(const TextureBuffer &&) = delete;
    TextureBuffer &operator =(const TextureBuffer &&) = delete;

private:
    mutable QMutex m_mutex;

    bool m_ready;

    unsigned m_texture;
};

#endif // TEXTUREBUFFER_H
