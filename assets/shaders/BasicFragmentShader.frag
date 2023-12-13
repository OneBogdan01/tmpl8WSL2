#version 310 es
out highp vec4 FragColor;

in highp vec2 TexCoord;

uniform sampler2D wallTexture;
uniform sampler2D faceTexture;
uniform highp float mixing;
void main()
	{ 
		FragColor = mix(texture(wallTexture, TexCoord), texture(faceTexture, TexCoord), mixing);
	}