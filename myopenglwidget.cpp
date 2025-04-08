#include "myopenglwidget.h"

#include <QTime>

// 点位置
float vertices[] = { // 矩形
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // 右上角 红色
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 右下角 绿色
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // 左下角 蓝色
    -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f  // 左上角 灰色
};
unsigned int indices[] = { // 索引数组
    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_shaderProgram = new QOpenGLShaderProgram(this); // 创建着色器程序对象
    m_timer = new QTimer(this); // 创建定时器对象
    connect(m_timer, &QTimer::timeout, this, &MyOpenGLWidget::onTimerTimeout); // 连接定时器超时信号
    m_timer->start(100); // 启动定时器，100ms触发一次
}

MyOpenGLWidget::~MyOpenGLWidget()
{
    if (m_shaderProgram) m_shaderProgram->deleteLater(); // 删除着色器程序对象
    if (m_timer) m_timer->deleteLater(); // 删除定时器对象
    if (!isValid()) return; // 如果OpenGL上下文无效，直接返回

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

    // 创建VAO和VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 绑定VAO和VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 内存数据传入显卡
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 获取顶点属性位置
    m_shaderProgram->bind();
    GLint posLocation = m_shaderProgram->attributeLocation("aPos");
    qDebug() << "posLocation:" << posLocation;
    // 告知显卡如何解析数据
    // （位置，大小，类型，是否归一化，步长，偏移量）
    glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // 位置
    glEnableVertexAttribArray(posLocation); // 启用顶点属性
    glVertexAttribPointer(posLocation + 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // 颜色
    glEnableVertexAttribArray(posLocation + 1); // 启用顶点属性

    // 创建EBO
    glGenBuffers(1, &EBO);
    // 绑定EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // 内存数据传入显卡
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 解绑VAO和VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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

void MyOpenGLWidget::onTimerTimeout()
{
    if (!m_shaderProgram) return; // 如果着色器程序为空，直接返回

    makeCurrent(); // 确保当前上下文有效
    int timeValue = QTime::currentTime().second(); // 获取当前秒数
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // 计算绿色值
    m_shaderProgram->setUniformValue("ourColor", 0.0f, greenValue, 0.0f, 1.0f); // 设置uniform变量
    doneCurrent(); // 释放当前上下文
    update(); // 更新窗口，触发paintGL()函数
}
