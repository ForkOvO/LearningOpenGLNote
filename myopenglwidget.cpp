#include "myopenglwidget.h"

#include <QTime>

/*
标准化设备坐标（NDC）
在OpenGL中，所有的坐标都是在一个标准化的设备坐标系中进行处理的。这个坐标系的范围是[-1, 1]，
其中x轴和y轴分别表示水平和垂直方向，z轴表示深度。这个坐标系被称为标准化设备坐标（NDC）。
在NDC中，左下角的点是(-1, -1)，右上角的点是(1, 1)。所有的顶点坐标都需要经过一个变换过程，将其转换为NDC坐标。

纹理坐标（UV坐标）
纹理坐标是一个二维坐标，用于指定纹理图像中的位置。纹理坐标的范围是[0, 1]，其中(0, 0)表示纹理图像的左下角，
(1, 1)表示纹理图像的右上角。在片段着色器中，可以使用纹理坐标来采样纹理图像，从而实现纹理映射。
*/

// 点位置
float vertices[] = { // 矩形
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f,  // 右上角 红色 纹理坐标
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, -1.0f, // 右下角 绿色
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, // 左下角 蓝色
    -0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, -1.0f, 2.0f  // 左上角 灰色
};
unsigned int indices[] = { // 索引数组
    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

MyOpenGLWidget::~MyOpenGLWidget()
{
    if (m_shaderProgram) m_shaderProgram->deleteLater(); // 删除着色器程序对象
    if (m_timer) m_timer->deleteLater(); // 删除定时器对象
    if (m_texture1) delete m_texture1; // 删除纹理对象
    if (m_texture2) delete m_texture2; // 删除纹理对象
    if (!isValid()) return; // 如果OpenGL上下文无效，直接返回

    makeCurrent(); // 确保当前上下文有效
    glDeleteVertexArrays(1, &VAO); // 删除VAO
    glDeleteBuffers(1, &VBO); // 删除VBO
    glDeleteBuffers(1, &EBO); // 删除EBO
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

    m_shaderProgram = new QOpenGLShaderProgram(this); // 创建着色器程序对象
    m_timer = new QTimer(this); // 创建定时器对象
    connect(m_timer, &QTimer::timeout, this, &MyOpenGLWidget::onTimerTimeout); // 连接定时器超时信号
    m_timer->start(100); // 启动定时器，100ms触发一次

    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shapes.vert"); // 添加顶点着色器
    m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shapes.frag"); // 添加片段着色器
    m_shaderProgram->link(); // 链接着色器程序

    // 内存数据传入显卡
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 获取顶点属性位置
    m_shaderProgram->bind();
    GLint posLocation = m_shaderProgram->attributeLocation("aPos");
    qDebug() << "posLocation:" << posLocation;
    // 告知显卡如何解析数据
    // （位置，大小，类型，是否归一化，步长，偏移量）
    glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // 位置
    glEnableVertexAttribArray(posLocation); // 启用顶点属性
    glVertexAttribPointer(posLocation + 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // 颜色
    glEnableVertexAttribArray(posLocation + 1); // 启用顶点属性
    glVertexAttribPointer(posLocation + 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // 纹理坐标
    glEnableVertexAttribArray(posLocation + 2); // 启用顶点属性

    m_texture1 = new QOpenGLTexture(QImage(":/res/wall.jpg").mirrored()); // 创建纹理对象
    m_shaderProgram->setUniformValue("texture1", 0); // 设置纹理单元
    m_texture2 = new QOpenGLTexture(QImage(":/res/awesomeface.png").mirrored()); // 创建纹理对象
    m_shaderProgram->setUniformValue("texture2", 1); // 设置纹理单元

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
        m_texture1->bind(0); // 绑定纹理单元
        m_texture2->bind(1); // 绑定纹理单元
        // 设置纹理环绕方式
        m_texture1->setWrapMode(QOpenGLTexture::Repeat); // 重复纹理
        m_texture2->setWrapMode(QOpenGLTexture::MirroredRepeat); // 镜像重复纹理
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
