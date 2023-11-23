#version 310 es
out highp vec4 FragColor;

uniform highp vec3 color;
void main()
	{ 
		FragColor = vec4(color, 1.0f);
	}