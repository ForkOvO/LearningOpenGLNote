#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

/*
顶点着色器 - 处理顶点数据
形状（图元）装配 - 将顶点数据组合成图元
几何着色器 - 可选，处理图元数据
光栅化 - 将图元转换为片段数据
片段着色器 - 处理片段数据
测试与混合 - 处理片段数据，决定最终颜色

标准化设备坐标（NDC）
在OpenGL中，所有的坐标都是在一个标准化的设备坐标系中进行处理的。这个坐标系的范围是[-1, 1]，
其中x轴和y轴分别表示水平和垂直方向，z轴表示深度。这个坐标系被称为标准化设备坐标（NDC）。
在NDC中，左下角的点是(-1, -1)，右上角的点是(1, 1)。所有的顶点坐标都需要经过一个变换过程，将其转换为NDC坐标。
*/

class MyOpenGLWidget : public QOpenGLWidget, QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit MyOpenGLWidget(QWidget *parent = nullptr);
    ~MyOpenGLWidget() override;

protected:
    void initializeGL() override; // 初始化OpenGL环境
    void resizeGL(int w, int h) override; // 调整窗口大小时调用
    void paintGL() override; // 绘制OpenGL场景

private:
    // VAO (Vertex Array Object) - 存储顶点属性配置（结构）
    // VBO (Vertex Buffer Object)- 存储顶点数据（数据）
    unsigned int VAO, VBO;
    unsigned int shaderProgram;
    float vertices[9] = { // 位置
        -0.5f, -0.5f, 0.0f, // 左下角
        0.5f, -0.5f, 0.0f, // 右下角
        0.0f,  0.5f, 0.0f  // 顶部
    };

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";
};

#endif // MYOPENGLWIDGET_H
