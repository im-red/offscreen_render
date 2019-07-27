#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class RenderThread;
class GLWidget;

class QTimerEvent;
class QResizeEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void timerEvent(QTimerEvent *) override;
    void resizeEvent(QResizeEvent *) override;

private:
    Ui::MainWindow *ui;
    RenderThread *m_thread;
    GLWidget *m_glWidget;
};

#endif // MAINWINDOW_H
