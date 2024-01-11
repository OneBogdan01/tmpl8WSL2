#pragma once

class GameObject
{
public:
	virtual ~GameObject() = default;

	enum ColliderType
	{
		Ground,
		Coin,
		Obstacle,
		Player,
		Whip,
		Rope
	};

	GameObject(ColliderType type);
	virtual void CollidedWith(GameObject* p_obj1) = 0;
	virtual void SeparatedFrom(GameObject* p_obj1) = 0;
	ColliderType GetType() const;

protected:
	ColliderType type;
};
