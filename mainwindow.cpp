#include "mainwindow.h"
#include "myopenglwidget.h"

#include <QMenuBar>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("OpenGL Demo");
    setMinimumSize(800, 600);

    setStyleSheet("\
        QWidget {\
            background-color: rgb(68, 68, 68);\
            color: rgb(255, 255, 255);\
            font: 10pt \"幼圆\";\
        }\
        QMenuBar {\
            background-color: rgb(200, 200, 200);\
            color: rgb(60, 60, 60);\
        }");

    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *fileMenu = new QMenu("文件", this);
    menuBar->addMenu(fileMenu);
    QMenu *editMenu = new QMenu("编辑", this);
    menuBar->addMenu(editMenu);
    QMenu *settingsMenu = new QMenu("设置", this);
    menuBar->addMenu(settingsMenu);
    QMenu *helpMenu = new QMenu("帮助", this);
    menuBar->addMenu(helpMenu);
    setMenuBar(menuBar);

    QToolBar *toolBar = new QToolBar(this);
    QAction *darwRectAction = new QAction("绘制矩形", this);
    toolBar->addAction(darwRectAction);
    QAction *clearAction = new QAction("清除", this);
    toolBar->addAction(clearAction);
    addToolBar(toolBar);

    setCentralWidget(new MyOpenGLWidget(this));
}

MainWindow::~MainWindow()
{
}
