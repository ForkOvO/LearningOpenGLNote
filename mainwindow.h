#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MyOpenGLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    MyOpenGLWidget *m_openglWidget = nullptr; // OpenGL窗口部件
    QAction *m_drawRectAction = nullptr; // 绘制矩形动作
    QAction *m_clearAction = nullptr; // 清除动作
    QAction *m_wireframeAction = nullptr; // 线框模式动作
};

#endif // MAINWINDOW_H
