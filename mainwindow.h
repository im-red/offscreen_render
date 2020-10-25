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
    ~MainWindow() override;

protected:
    void timerEvent(QTimerEvent *) override;

private:
    Ui::MainWindow *ui;
    RenderThread *m_thread;
};

#endif // MAINWINDOW_H
