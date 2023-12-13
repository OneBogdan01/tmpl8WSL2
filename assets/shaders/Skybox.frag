#version 310 es
//from https://learnopengl.com/Advanced-OpenGL/Cubemaps
out highp vec4 FragColor;
in highp vec3 TexCoords;
uniform samplerCube skybox;
void main()
	{ 
		 FragColor = texture(skybox, TexCoords);
	}