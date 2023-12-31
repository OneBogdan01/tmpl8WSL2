#include "PointLight.h"

#include <string>


#include "template.h"

void PointLight::SetLightProperties(Shader& lightingShader, size_t index)
{
	std::string str = "pointLights[" + std::to_string(index) + "].";
	lightingShader.SetFloat((str + "constant").c_str(), constant);
	lightingShader.SetFloat((str + "linear").c_str(), linear);
	lightingShader.SetFloat((str + "quadratic").c_str(), quadratic);

	lightingShader.SetVec3((str + "ambient").c_str(), ambient);
	lightingShader.SetVec3((str + "diffuse").c_str(), diffuse*glm::vec3(5)); // darken diffuse light a bit
	lightingShader.SetVec3((str + "specular").c_str(), specular);
	lightingShader.SetVec3((str + "position").c_str(), position);
}


void PointLight::SetConstant(float newConstant)
{
	constant = newConstant;
}

void PointLight::SetLinear(float newLinear)
{
	linear = newLinear;
}

void PointLight::SetQuadratic(float newQuadratic)
{
	quadratic = newQuadratic;
}
