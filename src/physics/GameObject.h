#pragma once

class GameObject
{
public:
	enum ColliderType
	{
		Box,
		Sphere,
		Capsule,
		Mesh
	};

	GameObject(ColliderType type);
	void CollidedWith(GameObject* p_obj1);
	void SeparatedFrom(GameObject* p_obj1);

private:
	ColliderType m_type;
};
