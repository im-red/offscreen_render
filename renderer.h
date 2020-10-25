#ifndef RENDERER_H
#define RENDERER_H

#include "sierpinski.h"

#include <QObject>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QImage>
#include <memory>

class Renderer : public QObject, public QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    explicit Renderer(QObject *parent = nullptr);
    ~Renderer();

    void render(int width, int height);

private:
    void init();
    void uninit();
    void adjustSize();

private:
    Renderer(const Renderer &) = delete;
    Renderer &operator =(const Renderer &) = delete;
    Renderer(const Renderer &&) = delete;
    Renderer &operator =(const Renderer &&) = delete;

private:
    int m_width = 0;
    int m_height = 0;
    int m_viewportX = 0;
    int m_viewportY = 0;
    int m_viewportWidth = 0;
    int m_viewportHeight = 0;
    bool m_sizeChanged = true;

    unsigned m_vao = 0;
    unsigned m_vbo = 0;
    unsigned m_fbo = 0;
    unsigned m_rbo = 0;
    unsigned m_texture = 0;

    std::unique_ptr<QOpenGLShaderProgram> m_program;
    std::unique_ptr<Sierpinski> m_sierpinski;
};

#endif // RENDERER_H
