#version 330 core

// uniform 全局变量
uniform vec4 ourColor;
in vec4 vertexColor;
in vec2 TexCoord;
uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

void main()
{
    // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    // FragColor = ourColor; // 定时器颜色
    // FragColor = vertexColor; // 渐变颜色
    // FragColor = texture(texture1, TexCoord); // 纹理贴图
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5); // 纹理贴图混合
}
