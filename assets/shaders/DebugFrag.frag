#version 310 es
precision highp float;
out vec4 FragColor;

in vec3 ColorVertex;
void main()
	{ 
		FragColor = vec4(ColorVertex, 1.0f);
	}