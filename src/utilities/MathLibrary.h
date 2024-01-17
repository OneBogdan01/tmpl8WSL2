#pragma once
#include "glm/vec2.hpp"

//from my previos project
class MathLibrary
{
public:
	static glm::vec2 CubicBezierCurve(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d,
	                                  float t);
	static glm::vec2 QuadraticBezierCurve(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c,
	                                      float t);
	static float InvLerp(float a, float b, float v);
	
	static float Lerp(float a, float b, float f);
	static float Sign(float value);
};
