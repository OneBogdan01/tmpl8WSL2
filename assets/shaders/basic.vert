#version 300 es

layout (location = 0) in vec3 pos;  // in local coords
layout (location = 1) in vec3 nextPos;  // in local coords
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec3 nextNormal;

out vec2 TexCoords;

uniform mat4 model;			// model matrix
uniform mat4 view;			// view matrix
uniform mat4 projection;	// projection matrix

uniform mat4 modelView;
uniform float interpolation;
out vec3 Normal;

out highp vec3 FragPos; 
void main()
{
	float InterpolatedDeltaX = (nextPos.x - pos.x) * interpolation;
	float InterpolatedDeltaY = (nextPos.y - pos.y) * interpolation;
	float InterpolatedDeltaZ = (nextPos.z - pos.z) * interpolation;
	vec3 interpolatedPos = vec3(pos.x + InterpolatedDeltaX, pos.y + InterpolatedDeltaY, pos.z + InterpolatedDeltaZ);
	gl_Position = projection * modelView * vec4(interpolatedPos, 1.0f);

	float nX = (nextNormal.x - aNormal.x) * interpolation;
	float nY = (nextNormal.y - aNormal.y) * interpolation;
	float nZ = (nextNormal.z - aNormal.z) * interpolation;
	vec3 interNormal = vec3(aNormal.x + nX, aNormal.y + nY, aNormal.z + nZ);

	Normal = interNormal;





	TexCoords = texCoord;
	FragPos = vec3(model * vec4(pos, 1.0));
}
