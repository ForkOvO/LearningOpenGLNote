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
        QWidget:checked {\
            background-color: rgb(150, 150, 150);\
        }\
        QMenuBar {\
            background-color: rgb(200, 200, 200);\
            color: rgb(60, 60, 60);\
        }");

    m_openglWidget = new MyOpenGLWidget(this);
    setCentralWidget(m_openglWidget);

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
    m_drawRectAction = new QAction("绘制矩形", this);
    connect(m_drawRectAction, &QAction::triggered, [this] {
        m_openglWidget->drawShape(MyOpenGLWidget::Rectangle);
    });
    toolBar->addAction(m_drawRectAction);
    m_clearAction = new QAction("清除", this);
    connect(m_clearAction, &QAction::triggered, [this] {
        m_openglWidget->drawShape(MyOpenGLWidget::None);
    });
    toolBar->addAction(m_clearAction);
    m_wireframeAction = new QAction("线框模式", this);
    m_wireframeAction->setCheckable(true);
    connect(m_wireframeAction, &QAction::triggered, [this](bool checked) {
        m_openglWidget->setWireframeMode(m_wireframeAction->isChecked());
    });
    toolBar->addAction(m_wireframeAction);
    addToolBar(toolBar);
}

MainWindow::~MainWindow()
{
}
