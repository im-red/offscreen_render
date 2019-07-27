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
    int m_width;
    int m_height;
    bool m_sizeChanged;

    unsigned m_vao;
    unsigned m_vbo;
    unsigned m_fbo;
    unsigned m_rbo;
    unsigned m_texture;

    std::unique_ptr<QOpenGLShaderProgram> m_program;
    std::unique_ptr<Sierpinski> m_sierpinski;
};

#endif // RENDERER_H
