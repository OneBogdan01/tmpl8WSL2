#include "MathLibrary.h"
using namespace glm;

vec2 MathLibrary::CubicBezierCurve(const vec2& a, const vec2& b, const vec2& c, const vec2& d, float t)
{
	return a * (-(t * t * t) + 3 * t * t - 3 * t + 1)
		+ b * (3 * t * t * t - (6 * t * t) + (3 * t))
		+ c * (-3 * t * t * t + 3 * t * t)
		+ d * t * t * t;
}

//from https://math.stackexchange.com/questions/1360891/find-quadratic-bezier-curve-equation-based-on-its-control-points
vec2 MathLibrary::QuadraticBezierCurve(const vec2& a, const vec2& b, const vec2& c, float t)
{
	return (1.0f - t) * (1.0f - t) * a + 2 * t * (1.0f - t) * b + (t * t) * c;
}

float MathLibrary::InvLerp(const float a, const float b, const float v)
{
	return (v - a) / (b - a);
}

float MathLibrary::Sign(float value)
{
	if (value > 0)
		return 1.0f;

	return -1.0f;
}
