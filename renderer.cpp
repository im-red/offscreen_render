#include "renderer.h"
#include "debug.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QtMath>
#include <QMatrix4x4>

#include <iostream>

Renderer::Renderer(QObject *parent)
    : QObject(parent)
{
    init();
}

Renderer::~Renderer()
{
    uninit();
}

void Renderer::render(int width, int height)
{
    RAIITimer t("Renderer::render");
    if (m_width != width || m_height != height)
    {
        m_width = width;
        m_height = height;
        adjustSize();
    }

    static float degree = 0.0f;
    degree += 1.0f;

    QMatrix4x4 rotate;
    rotate.setToIdentity();
    rotate.rotate(degree, 0, 0, 1);

    glViewport(m_viewportX, m_viewportY, m_viewportWidth, m_viewportHeight);
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();
    m_program->setUniformValue("rotate", rotate);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_sierpinski->vertexCount()));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_program->release();
    glFinish();
}

void Renderer::init()
{
    initializeOpenGLFunctions();

//    glEnable(GL_DEBUG_OUTPUT);
//    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
//    glDebugMessageCallback(glDebugOutput, nullptr);

    qDebug() << reinterpret_cast<const char *>(glGetString(GL_VERSION));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &m_rbo);

    adjustSize();

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    char vertexShaderSource[] =
            "#version 330 core\n"
            "layout (location = 0) in vec2 vPos;\n"
            "uniform mat4 rotate;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = rotate * vec4(vPos.x, vPos.y, 0.0, 1.0);\n"
            "}\n";
    char fragmentShaderSource[] =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
            "}\n";

    m_program.reset(new QOpenGLShaderProgram);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();

    m_sierpinski.reset(new Sierpinski(15));

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_sierpinski->size() * sizeof(float), m_sierpinski->data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), reinterpret_cast<void *>(0));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::uninit()
{
    glDeleteRenderbuffers(1, &m_rbo);
    glDeleteTextures(1, &m_texture);
    glDeleteFramebuffers(1, &m_fbo);
}

void Renderer::adjustSize()
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    if (m_width > m_height)
    {
        m_viewportX = (m_width - m_height) / 2;
        m_viewportY = 0;
        m_viewportWidth = m_height;
        m_viewportHeight = m_height;
    }
    else
    {
        m_viewportX = 0;
        m_viewportY = (m_height - m_width) / 2;
        m_viewportWidth = m_width;
        m_viewportHeight = m_width;
    }
}
