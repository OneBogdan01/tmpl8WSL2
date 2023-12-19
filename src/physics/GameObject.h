#pragma once

class GameObject
{
public:
	enum ColliderType
	{
		Ground,
		Sphere,
		Capsule,
		Player
	};

	GameObject(ColliderType type);
	void CollidedWith(GameObject* p_obj1);
	void SeparatedFrom(GameObject* p_obj1);
	bool onGround = false;
private:
	
	ColliderType m_type;
};
