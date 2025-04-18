#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QOpenGLTexture>

/*
顶点着色器 - 处理顶点数据
形状（图元）装配 - 将顶点数据组合成图元
几何着色器 - 可选，处理图元数据
光栅化 - 将图元转换为片段数据
片段着色器 - 处理片段数据
测试与混合 - 处理片段数据，决定最终颜色

OpenGL是一个巨大的状态机
VAO会保存所有的状态信息，包括VBO和EBO的绑定状态、顶点属性指针等。
这样，在绘制时，只需要绑定VAO即可，无需再次设置所有的状态信息。
VBO和EBO是存储数据的对象，分别用于存储顶点数据和索引数据。
*/

class MyOpenGLWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    enum Shape {
        None,
        Rectangle,
        Triangle,
    };

public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);
    ~MyOpenGLWidget() override;

    void drawShape(Shape shape); // 绘制形状
    void setWireframeMode(bool enabled); // 设置线框模式

protected:
    void initializeGL() override; // 初始化OpenGL环境
    void resizeGL(int w, int h) override; // 调整窗口大小时调用
    void paintGL() override; // 绘制OpenGL场景

private:
    void onTimerTimeout(); // 定时器超时处理函数

private:
    // VAO (Vertex Array Object) - 存储顶点属性配置（结构、状态）
    // VBO (Vertex Buffer Object)- 存储顶点数据（数据）
    // EBO (Element Buffer Object / Index Buffer Object) - 存储顶点索引（索引）
    unsigned int VAO, VBO, EBO;

    Shape m_shape = None; // 当前绘制的形状
    QTimer *m_timer = nullptr; // 定时器
    QOpenGLShaderProgram *m_shaderProgram = nullptr; // 着色器程序
    QOpenGLTexture* m_texture1 = nullptr; // 纹理
    QOpenGLTexture* m_texture2 = nullptr; // 纹理
};

#endif // MYOPENGLWIDGET_H
