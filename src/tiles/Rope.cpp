#include "Rope.h"

#include "game.h"


#include "utilities/MathLibrary.h"


void Rope::UpdatePhysicsPosition(glm::vec3 chunkPos)
{
	btTransform newTransform = ghostObject->getWorldTransform();

	newTransform.setOrigin(btVector3(GlmToBtVector3(chunkPos)));
	ghostObject->setWorldTransform(newTransform);
}

void Rope::ResetPosition()
{
	btTransform newTransform = ghostObject->getWorldTransform();

	newTransform.setOrigin(GetTileInitPosition());
	ghostObject->setWorldTransform(newTransform);
}

RopeTrigger& Rope::GetCallback()
{
	return *callback;
}

Rope::Rope() :
	Tile()
{
	timeToDisable = new Timer();
	t = new Timer();
}

Rope::~Rope()
{
	delete t;
	delete timeToDisable;
}

glm::vec3 Rope::DrawRope(const glm::vec3 a, float offsetX, btVector3 color)
{
	const glm::vec3 b = points[1];
	const glm::vec3 c = points[2];
	const glm::vec3 d = points[3];
	//make line
	//screen->BezierCurve(GREEN, p0, p1, p2, p3, resolution);
	glm::vec3 lastPoint{};
	glm::vec3 startPoint = a;
	for (uint i = 1; i <= resolution; i++)
	{
		const glm::vec2 point = MathLibrary::CubicBezierCurve(a, b, c, d,
		                                                      static_cast<float>(i) / static_cast<float>(resolution));

		//Line(startPoint.x, startPoint.y, point.x, point.y);
		line.StoreLine(btVector3(a.x + offsetX, startPoint.y, a.z + startPoint.x),
		               btVector3(a.x + offsetX, point.y, a.z + point.x),
		               color);
		if (i == resolution)
			lastPoint = glm::vec3(point.x, point.y, 0);
		startPoint = glm::vec3(point.x, point.y, 0);
	}
	return lastPoint;
}

void Rope::Render(glm::vec3 position)
{
	//const vec2 camPos = Game::GetCameraPosition();


	glm::vec3 a = initialPosition + position;

	const glm::vec3 endOfRope = DrawRope(a, -offset / 2, btVector3(1, 1, 0));;

	line.DrawLine();
	DrawRope(a, offset, btVector3(0, 1, 0));

	line.DrawLine();
	//draw the line in between
	line.StoreLine(btVector3(a.x - offset / 2, endOfRope.y, a.z + endOfRope.x),
	               btVector3(a.x + offset, endOfRope.y, a.z + endOfRope.x),
	               btVector3(0, 1, 1));


	line.DrawLine();

	UpdatePhysicsPosition(glm::vec3(a.x, endOfRope.y, a.z + endOfRope.x));



}

glm::vec3 Rope::GetMovingPartAtTime(glm::vec3 startPoint, float timeElapsed, const float leng)
{
	const float x = sinf(timeElapsed) * leng;
	const float y = -cosf(timeElapsed) * leng;

	return startPoint + glm::vec3{x, y, 0};
}

glm::vec3 Rope::GetMovingPart() const
{
	return points[3];
}

glm::vec3* Rope::pGetMovingPart()
{
	return &points[3];
}

void Rope::ActivateMovement()
{
	if (shouldMove)
		return;
	shouldMove = true;
	t->reset();
}

void Rope::SimulateRope(float deltaTime)
{
	points[0] = initialPosition;
	for (int i = 1; i < 4; i++)
	{
		float timeElapsed = cosf(timeOffset + t->elapsed() * frq) * amp * deltaTime;
		//TODO remove this when player is on it

		points[i] = GetMovingPartAtTime(points[i - 1], timeElapsed * multipler[i - 1], len[i - 1] * lenMultiplier);
	}
}

void Rope::Update(float deltaTime)
{
	if (!shouldMove)
		return;
	if (t->elapsed() > 3.0f)
		DeactivateMovement();
	SimulateRope(deltaTime);
}

void Rope::ChangePosition(const glm::vec3& pos)
{
	initialPosition = pos;
	SimulateRope(0.f);
	DeactivateMovement();
}

void Rope::DeactivateMovement()
{
	shouldMove = false;
}

void Rope::Init(const char* path, const glm::vec3 pos, size_t _index)
{
	path;
	startDisableing = false;

	this->index = _index;
	callback = new RopeTrigger(GameObject::Rope, _index);
	callback->GetEvent().connect(&Rope::ActivateMovement, this);
	//.GetEvent().connect(&Chunk::DisableRope, this);
	timeOffset = PI / 2;
	initialPosition = pos;
	AddATriggerBox();
	points[0] = pos;
	totalLen = 0;
	for (int i = 0; i < 3; i++)
		totalLen += len[i];
	DeactivateMovement();
	callback->ropeEnd = &points[3];
	//coll = Box{-vec2{totalLen * lenMultiplier, 0}, vec2{totalLen * lenMultiplier, totalLen * lenMultiplier}};
}

void Rope::AddATriggerBox()
{
	//make a collision shape
	// Create a collision shape (e.g., a box shape for a rectangular tile)

	btBoxShape* tileShape = new btBoxShape(btVector3(1 + offset / 2, 1, 1));

	// Create a motion state
	btTransform tileTransform;
	tileTransform.setIdentity();
	//drawOffset = glm::vec3(0.0f, tileShape->getHalfExtentsWithMargin().y(), 0.0f);
	tileTransform.setOrigin(btVector3(initialPosition.x, initialPosition.y,
	                                  initialPosition.z));


	ghostObject = new btGhostObject();
	ghostObject->setWorldTransform(tileTransform);
	ghostObject->setCollisionShape(tileShape);
	ghostObject->setCollisionFlags(ghostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	ghostObject->setUserPointer(callback);

	Game::world.AddTrigger(ghostObject);
}
