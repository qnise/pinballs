
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	normal(1.f, 0.f, 0.f),
	timer(2)
{
}

GameObject::~GameObject()
{
}