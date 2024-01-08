﻿#include "Rope.h"

#include "game.h"
#include "utilities/MathLibrary.h"
using namespace glm;

Rope::Rope()
{
	t = new Timer();
}

Rope::~Rope()
{
	delete t;
}

void Rope::Render()
{
	//const vec2 camPos = Game::GetCameraPosition();
	const vec3 a = points[0];
	const vec3 b = points[1];
	const vec3 c = points[2];
	const vec3 d = points[3];
	//make line
	//screen->BezierCurve(GREEN, p0, p1, p2, p3, resolution);

	vec3 startPoint = a;
	for (uint i = 1; i <= resolution; i++)
	{
		const vec2 point = MathLibrary::CubicBezierCurve(a, b, c, d,
		                                                 static_cast<float>(i) / static_cast<float>(resolution));

		//Line(startPoint.x, startPoint.y, point.x, point.y);
		line.StoreLine(btVector3(a.x, startPoint.y, startPoint.x), btVector3(a.x, point.y, point.x),
		               btVector3(0, 1, 0));
		startPoint = glm::vec3(point.x, point.y, 0);
	}
	line.DrawLine();
#ifdef _DEBUG
	//const int x1 = static_cast<int>(points[0].x - camPos.x + coll.min.x);
	//const int y1 = static_cast<int>(points[0].y - camPos.y + coll.min.y);

	//const int x2 = static_cast<int>(points[0].x - camPos.x + coll.max.x);
	//const int y2 = static_cast<int>(points[0].y - camPos.y + coll.max.y);

#endif
}

vec3 Rope::GetMovingPartAtTime(vec3 startPoint, float timeElapsed, const float leng)
{
	const float x = sinf(timeElapsed) * leng;
	const float y = -cosf(timeElapsed) * leng;
	return startPoint + vec3{x, y, 0};
}

vec3 Rope::GetMovingPart() const
{
	return points[3];
}

vec3* Rope::pGetMovingPart()
{
	return &points[3];
}

void Rope::Update(float deltaTime)
{
	//got helped for this formula from Lynn 230137
	for (int i = 1; i < 4; i++)
	{
		const float timeElapsed = cosf(timeOffset + t->elapsed() * frq) * amp * deltaTime;

		points[i] = GetMovingPartAtTime(points[i - 1], timeElapsed * multipler[i - 1], len[i - 1] * lenMultiplier);
	}
}

void Rope::Init(vec3 _fixedPoint)
{
	//offset the function using the fixed point
	if (static_cast<int>(_fixedPoint.x) % 2 == 0)
	{
		timeOffset = PI;
	}
	else
		timeOffset = 0;

	points[0] = _fixedPoint;
	totalLen = 0;
	for (int i = 0; i < 3; i++)
		totalLen += len[i];
	//coll = Box{-vec2{totalLen * lenMultiplier, 0}, vec2{totalLen * lenMultiplier, totalLen * lenMultiplier}};
}
