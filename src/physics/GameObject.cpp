#include "GameObject.h"

#include <iostream>
using namespace std;

GameObject::GameObject(ColliderType type)
{
	m_type = type;
}

void GameObject::CollidedWith(GameObject* p_obj1)
{
	//cout << "This is a " << m_type << endl;
	//cout << "Collided with " << p_obj1->m_type << endl;
}

void GameObject::SeparatedFrom(GameObject* p_obj1)
{
	/*cout << "This is a " << m_type << endl;

	cout << "Finish colliding with " << p_obj1->m_type << endl;*/
}
