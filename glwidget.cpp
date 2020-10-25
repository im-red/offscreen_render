#include "glwidget.h"
#include "texturebuffer.h"
#include "renderthread.h"
#include "debug.h"
#include "fpscounter.h"

#include <QOpenGLContext>
#include <QDebug>
#include <QWindow>
#include <QSurface>

#include <iostream>

namespace
{
    float vertices[] =
    {
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
    };
}

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
    initRenderThread();

    qDebug() << initializeOpenGLFunctions();

//    glEnable(GL_DEBUG_OUTPUT);
//    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//    glDebugMessageCallback(glDebugOutput, nullptr);

    char vertexShaderSource[] =
            "#version 450 core\n"
            "layout (location = 0) in vec2 vPos;\n"
            "layout (location = 1) in vec2 texCoord;\n"
            "out vec2 TexCoord;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(vPos, 0.0, 1.0);\n"
            "   TexCoord = texCoord;\n"
            "}\n";
    char fragmentShaderSource[] =
            "#version 450 core\n"
            "out vec4 FragColor;\n"
            "in vec2 TexCoord;\n"
            "uniform sampler2D ourTexture;\n"
            "void main()\n"
            "{\n"
            "   FragColor = texture(ourTexture, TexCoord);\n"
            "}\n";

    m_program.reset(new QOpenGLShaderProgram);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void *>(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void *>(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glCheckError();
}

void GLWidget::paintGL()
{
    RAIITimer t("GLWidget::paintGL");

    glViewport(0, 0, width(), height());

    m_program->bind();

    glBindVertexArray(m_vao);
    if (TextureBuffer::instance()->ready())
    {
        TextureBuffer::instance()->drawTexture(QOpenGLContext::currentContext(), sizeof(vertices) / sizeof(float) / 4);
    }
    glBindVertexArray(0);

    m_program->release();

    FpsCounter::instance()->frame(FpsCounter::Display);
}

void GLWidget::resizeGL(int w, int h)
{
    m_thread->setNewSize(w, h);
}

void GLWidget::initRenderThread()
{
    auto context = QOpenGLContext::currentContext();
    auto mainSurface = context->surface();

    auto renderSurface = new QOffscreenSurface(nullptr, this);
    renderSurface->setFormat(context->format());
    renderSurface->create();

    context->doneCurrent();
    m_thread = new RenderThread(renderSurface, context, this);
    context->makeCurrent(mainSurface);

    connect(m_thread, &RenderThread::imageReady, this, [this](){
        update();
    }, Qt::QueuedConnection);
    m_thread->start();
}
