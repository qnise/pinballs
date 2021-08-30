#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneCollision : public SceneBase
{
public:
	SceneCollision();
	~SceneCollision();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();
	void ReturnGO(GameObject *go);
protected:

	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	float m_ballScale;
	float m_rotation;
	float m_rotationSpeed;
	float m_gravity;
	float m_restitutionCoeff;
	float m_maxRotSpeed;
	float m_RotSpeedAcceleration;
	float m_pWallHeight;

	bool bounced;

	GameObject *m_ghost;
	GameObject *m_ghost2;

	GameObject *m_cWall[2];
	GameObject *m_cPillar[2][2];
	
	int m_objectCount;
	int m_clickcount;
	
	

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2, w1, w2;

	bool CheckCollision(GameObject* go1, GameObject* go2, float dt);
	void CollisionResponse(GameObject* go1, GameObject* go2);
	void makeThinWall(float x, float y, float angleDeg, float thickness, float height);
	void makeThinWall(Vector3 pos, Vector3 normal, float thickness, float height);
	void makeThickWall(float x, float y, float angleDeg, float thickness, float height);

	void getPlayerPosition(int player);
	void movePlayer(int player);
};

#endif