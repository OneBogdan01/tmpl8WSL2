#version 310 es

layout (location = 0) in vec3 pos;  // in local coords


out vec3 TexCoords;


uniform mat4 view;			// view matrix
uniform mat4 projection;	// projection matrix

void main()
{
    TexCoords = pos;
    highp vec4 p = projection * view * vec4(pos, 1.0);
    gl_Position = p.xyww;
}  
