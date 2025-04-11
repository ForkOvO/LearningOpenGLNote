#version 330 core

// GLSL (OpenGL Shader Language)

// in 传入
// location 索引位置
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

// out 传出
out vec3 vertexColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexColor = aColor;
    TexCoord = aTexCoord;
}
