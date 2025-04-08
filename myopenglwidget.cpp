#include "myopenglwidget.h"

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

MyOpenGLWidget::~MyOpenGLWidget()
{
}

void MyOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions(); // 初始化OpenGL函数指针

    // 创建VAO和VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 绑定VAO和VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 内存数据传入显卡
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 告知显卡如何解析数据
    // （位置，大小，类型，是否归一化，步长，偏移量）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 启用顶点属性
    glEnableVertexAttribArray(0);

    // 解绑VAO和VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 顶点着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // 片段着色器
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // 链接着色器程序
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void MyOpenGLWidget::resizeGL(int w, int h)
{
}

void MyOpenGLWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清除后颜色
    glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲区

    glUseProgram(shaderProgram); // 使用着色器程序
    glBindVertexArray(VAO); // 绑定VAO
    glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制三角形
}
