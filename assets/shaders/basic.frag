#version 300 es

in highp vec2 TexCoord;
out highp vec4 frag_color;

uniform sampler2D texSampler1;

void main()
{
	frag_color = texture(texSampler1, TexCoord);
}
