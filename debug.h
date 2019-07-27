#ifndef DEBUG_H
#define DEBUG_H

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QDebug>
#include <QElapsedTimer>

class Timer
{
public:
    Timer(const std::string &tag)
        : m_tag(tag)
        , m_printed(false)
    {
        m_timer.start();
        qDebug() << m_tag.c_str() << "begin";
    }
    ~Timer()
    {
        stop();
    }
    void stop()
    {
        if (m_printed)
        {
            return;
        }
        m_printed = true;
        auto ns = m_timer.nsecsElapsed();
        float ms = ns / 1000000.0;
        qDebug() << m_tag.c_str() << "cost" << ms << "ms";
    }

private:
    std::string m_tag;
    bool m_printed;
    QElapsedTimer m_timer;
};

// glCheckError_ && glDebugOutput is from https://learnopengl.com/In-Practice/Debugging

inline GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = QOpenGLContext::currentContext()->functions()->glGetError()) != GL_NO_ERROR)
    {
        const char *error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        qDebug() << error << " | " << file << " (" << line << ")";
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

inline void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei,
                            const GLchar *message,
                            const void *)
{
    // ignore non-significant error/warning codes
    // if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    qDebug() << "---------------";
    qDebug() << "Debug message (" << id << "):" <<  message;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             qDebug() << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   qDebug() << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: qDebug() << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     qDebug() << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     qDebug() << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           qDebug() << "Source: Other"; break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               qDebug() << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: qDebug() << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  qDebug() << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         qDebug() << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         qDebug() << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              qDebug() << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          qDebug() << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           qDebug() << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               qDebug() << "Type: Other"; break;
    }

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         qDebug() << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       qDebug() << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          qDebug() << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: qDebug() << "Severity: notification"; break;
    }
}

#endif // DEBUG_H
