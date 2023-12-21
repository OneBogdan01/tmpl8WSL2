#include "GameObject.h"

#include <iostream>
using namespace std;

GameObject::GameObject(ColliderType _type)
{
	type = _type;
}

GameObject::ColliderType GameObject::GetType() const
{
	return type;
}
