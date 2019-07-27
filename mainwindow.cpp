#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "renderthread.h"
#include "texturebuffer.h"
#include "glwidget.h"
#include "fpscounter.h"

#include <QDateTime>
#include <QStatusBar>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_glWidget = new GLWidget(this);
    ui->centralWidget->layout()->addWidget(m_glWidget);

    startTimer(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *)
{
    m_glWidget->update();
    static int i = 0;
    i++;

    const int INTERVAL = 30;
    if (i % INTERVAL == 0)    // update fps message every INTERVAL frame
    {
        QString fps = QString::fromStdString(FpsCounter::instance()->toString());
        qDebug() << fps;
        statusBar()->showMessage(fps);
    }
}

void MainWindow::resizeEvent(QResizeEvent *)
{

}
