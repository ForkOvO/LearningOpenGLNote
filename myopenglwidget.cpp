#include "myopenglwidget.h"

// 点位置
#if 0
float vertices[] = { // 三角形
    -0.5f, -0.5f, 0.0f, // 左下角
    0.5f, -0.5f, 0.0f, // 右下角
    0.0f,  0.5f, 0.0f  // 顶部
};
#elif 0
float vertices[] = { // 矩形
    // 第一个三角形
    0.5f,  0.5f, 0.0f,  // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, 0.5f, 0.0f, // // 左上角
    // 第二个三角形
    0.5f, -0.5f, 0.0f, // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f,  0.5f, 0.0f  // 左上角
};
#else
float vertices[] = { // 矩形
    0.5f, 0.5f, 0.0f,  // 右上角
    0.5f, -0.5f, 0.0f, // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f  // 左上角
};
unsigned int indices[] = { // 索引数组
    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};
#endif

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

MyOpenGLWidget::~MyOpenGLWidget()
{
    makeCurrent(); // 确保当前上下文有效
    glDeleteVertexArrays(1, &VAO); // 删除VAO
    glDeleteBuffers(1, &VBO); // 删除VBO
    glDeleteBuffers(1, &EBO); // 删除EBO
    // glDeleteProgram(shaderProgram); // 删除着色器程序
    doneCurrent(); // 释放当前上下文
}

void MyOpenGLWidget::drawShape(Shape shape)
{
    m_shape = shape; // 设置当前绘制的形状
    update(); // 更新窗口，触发paintGL()函数
}

void MyOpenGLWidget::setWireframeMode(bool enabled)
{
    makeCurrent(); // 确保当前上下文有效
    // 绘制模式 默认是填充模式 GL_FILL
    if (enabled) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 线框模式
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // 填充模式
    doneCurrent(); // 释放当前上下文
    update(); // 更新窗口，触发paintGL()函数
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

    // 创建EBO
    glGenBuffers(1, &EBO);
    // 绑定EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // 内存数据传入显卡
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 解绑VAO和VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_shaderProgram = new QOpenGLShaderProgram(this); // 创建着色器程序对象
#if 0
    // 顶点着色器
    // unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // glCompileShader(vertexShader);
    m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource); // 添加顶点着色器

    // 片段着色器
    // unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // glCompileShader(fragmentShader);
    m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource); // 添加片段着色器

    // 链接着色器程序
    // shaderProgram = glCreateProgram();
    // glAttachShader(shaderProgram, vertexShader);
    // glAttachShader(shaderProgram, fragmentShader);
    // glLinkProgram(shaderProgram);
    m_shaderProgram->link(); // 链接着色器程序

    // 删除着色器对象
    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader);
#else
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shapes.vert"); // 添加顶点着色器
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shapes.frag"); // 添加片段着色器
    m_shaderProgram->link(); // 链接着色器程序
#endif
}

void MyOpenGLWidget::resizeGL(int w, int h)
{
}

void MyOpenGLWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清除后颜色
    glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲区

    // glUseProgram(shaderProgram); // 使用着色器程序
    m_shaderProgram->bind(); // 使用着色器程序
    glBindVertexArray(VAO); // 绑定VAO

    switch (m_shape)
    {
    case Shape::None:
        break;
    case Shape::Rectangle:
    {
        // glDrawArrays(GL_TRIANGLES, 0, 6); // 绘制矩形
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // 绘制矩形(使用索引)
        break;
    }
    case Shape::Triangle:
        glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制三角形
        break;
    }
}
