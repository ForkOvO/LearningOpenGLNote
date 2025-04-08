#version 330 core

// GLSL (OpenGL Shader Language)

// in 传入
// location 索引位置
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

// out 传出
out vec3 vertexColor;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexColor = aColor;
}
