#version 330 core

// uniform 全局变量
uniform vec4 ourColor;
in vec4 vertexColor;
out vec4 FragColor;

void main()
{
    // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    // FragColor = ourColor; // 定时器颜色
    FragColor = vertexColor; // 渐变颜色
}
