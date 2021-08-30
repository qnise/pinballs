#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_WALL,
		GO_CUBE,
		GO_PILLAR,
		GO_PWALL,
		GO_PPILLAR,
		GO_NORMAL,
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 color;
	Vector3 normal;
	Vector3 dir;
	int timer;
	bool active;
	float mass;
	float rotation;
	float rotationspeed;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif