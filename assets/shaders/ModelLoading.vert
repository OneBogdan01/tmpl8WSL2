#version 310 es
//from https://learnopengl.com/Lighting/Basic-Lighting
precision highp float;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aColor;
layout (location = 4) in mat4 aInstanceMatrix;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos; 
out vec2 TexCoords;
out vec3 Color;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0);
    FragPos = vec3(aInstanceMatrix * vec4(aPos, 1.0));
    Normal = aNormal;
    Color = aColor;
}