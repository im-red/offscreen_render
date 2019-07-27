#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>

#include <memory>

class RenderThread;

class GLWidget : public QOpenGLWidget, public QOpenGLFunctions_4_5_Core
{
public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    void initRenderThread();

private:
    unsigned m_vbo;
    unsigned m_vao;
    std::unique_ptr<QOpenGLShaderProgram> m_program;
    RenderThread *m_thread;
};

#endif // GLWIDGET_H
