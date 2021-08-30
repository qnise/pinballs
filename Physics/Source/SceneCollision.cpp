#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneCollision::SceneCollision()
{
}

SceneCollision::~SceneCollision()
{
}

void SceneCollision::Init()
{
	SceneBase::Init();

	//calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here //i beg you
	m_speed = 1.f;
	m_gravity = 10;
	m_restitutionCoeff = 0.8f;
	m_rotation = 0;
	m_rotationSpeed = 0;

	m_maxRotSpeed = 4;
	m_RotSpeedAcceleration = 1;
	
	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_objectCount = 0;

	m_ghost = new GameObject(GameObject::GO_BALL);
	m_ghost2 = new GameObject(GameObject::GO_BALL);
	bounced = false;

	m_pWallHeight = 10.f;
	const float offset = 44.5;
	const float wallThickness = 2.f;
	const float angleInterval = Math::QUARTER_PI;

	//for (int i = 0; i < 8; i++)
	//{
	//	GameObject* go = FetchGO();
	//	go->type = GameObject::GO_WALL;
	//	go->scale.Set(wallThickness, wallHeight, 0);
	//	go->normal.Set(cosf(angleInterval * i), sinf(angleInterval * i), 0);
	//	//go->scale.Set(2, 2, 2);
	//	//go->mass = go->scale.x;
	//	go->pos.Set(m_worldWidth / 2.f + offset * cosf(angleInterval * i),
	//				m_worldHeight / 2.f + offset * sinf(angleInterval * i), 0);
	//}

	m_clickcount = 0;

	m_cWall[0] = FetchGO();
	m_cWall[0]->active = true;
	m_cWall[0]->type = GameObject::GO_PWALL;
	m_cWall[0]->scale.Set(wallThickness, m_pWallHeight, 0);
	m_cWall[0]->pos.Set(m_worldWidth * 0.5f, 20, 0);
	float positions[2] = { m_pWallHeight, -m_pWallHeight };

	for (int i = 0; i < 2; i++)
	{
		m_cPillar[i][0] = FetchGO();
		m_cPillar[i][0]->active = true;
		m_cPillar[i][0]->type = GameObject::GO_PPILLAR;
		m_cPillar[i][0]->scale.Set(wallThickness, wallThickness, wallThickness);
		m_cPillar[i][0]->pos.Set(m_worldWidth / 2.f, positions[i], 0);
	}
}

GameObject* SceneCollision::FetchGO()
{
	//Exercise 2a: implement FetchGO() 
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); it++)
	{
		GameObject* go = (GameObject*)*it;
		if (go->active == false)
		{
			go->active = true;
			//Exercise 2b: increase object count every time an object is set to active
			++m_objectCount;
			return go;
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		m_goList.push_back(new GameObject(GameObject::GO_BALL));
	}

	//Exercise 2b: increase object count every time an object is set to active
	m_goList.back()->active = true;
	++m_objectCount;
	return m_goList.back();
}

void SceneCollision::ReturnGO(GameObject *go)
{
	//Exercise 3: implement ReturnGO()
	if (go->active)
	{
		go->active = false;
		--m_objectCount;
	}
}

bool SceneCollision::CheckCollision(GameObject* go, GameObject* go2, float dt)
{
	if (go->type == GameObject::GO_BALL && go2->type == GameObject::GO_BALL || go2->type == GameObject::GO_PILLAR || go2->type == GameObject::GO_PPILLAR)
	{
		Vector3 relVel = go->vel - go2->vel;
		Vector3 collisionNormal = go2->pos - go->pos;

		if (relVel.Dot(collisionNormal) <= 0)
		{
			return false;
		}

		return (go->pos - go2->pos).LengthSquared() <= (go->scale.x + go2->scale.x) * (go->scale.x + go2->scale.x);
	}
	else if (go->type == GameObject::GO_BALL && go2->type == GameObject::GO_WALL || go2->type == GameObject::GO_PWALL)
	{
		Vector3 N = go2->normal;
		if ((go2->pos - go->pos).Dot(N) < 0)
		{
			N = -N;
		}

		if (N.Dot(go->vel) < 0)
		{
			return false;
		}

		Vector3 NP = Vector3(-N.y, N.x, 0);
		Vector3 w0_b1 = go2->pos - go->pos;
		Vector3 directionVec = go2->pos - go->pos;
		float r = go->scale.x;
		float h_2 = go2->scale.x;
		float l_2 = go2->scale.y;

		if (directionVec.Dot(go2->normal) < 0)
		{
			N = -N;
		}

		if (abs((w0_b1).Dot(N)) < r + h_2 &&
			abs((w0_b1).Dot(NP)) < r + l_2)
		{
			std::cout << "HIT\n";
			return true;
		}

	}
	return false;
}

void SceneCollision::Update(double dt)
{
	SceneBase::Update(dt);
	
	//keybort
	if(Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	static bool bAWasPressed = false;
	if (Application::IsKeyPressed('A'))
	{
		bAWasPressed = true;
		m_rotationSpeed += m_RotSpeedAcceleration;
		if (m_rotationSpeed >= m_maxRotSpeed)
		{
			m_rotationSpeed = m_maxRotSpeed;
		}
	}

	else if (m_rotationSpeed > 0 && bAWasPressed == true)
	{
		m_rotationSpeed -= m_RotSpeedAcceleration * 3;
		if (m_rotationSpeed <= 0)
		{
			m_rotationSpeed = 0;
			bAWasPressed = false;
		}
	}

	static bool bDWasPressed = false;
	if (Application::IsKeyPressed('D'))
	{
		bDWasPressed = true;
		m_rotationSpeed -= m_RotSpeedAcceleration;
		if (m_rotationSpeed <= -m_maxRotSpeed)
		{
			m_rotationSpeed = -m_maxRotSpeed;
		}
	}

	else if (m_rotationSpeed < 0 && bDWasPressed == true)
	{
		m_rotationSpeed += m_RotSpeedAcceleration * 3;
		if (m_rotationSpeed >= 0)
		{
			m_rotationSpeed = 0;
			bDWasPressed = false;
		}
	}

	m_rotation += m_rotationSpeed * m_speed;

	m_cWall[0]->normal.Set(cosf(Math::DegreeToRadian(m_rotation)), sinf(Math::DegreeToRadian(m_rotation)));
	Vector3 np(m_cWall[0]->normal.y, -m_cWall[0]->normal.x, 0);
	Vector3 positions[2] = { m_cWall[0]->pos + np * m_pWallHeight, m_cWall[0]->pos - np * m_pWallHeight };

	for (int i = 0; i < 2; ++i)
	{
		m_cPillar[i][0]->pos = positions[i];
	}

	//mouse section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();

		if (m_clickcount == 0)
		{
			m_ghost->active = true;
			m_ghost->pos.Set((x / w) * m_worldWidth, m_worldHeight - (y / h) * m_worldHeight);
			++m_clickcount;
		}

		else if (m_clickcount == 1 && m_ghost->pos != Vector3((x / w) * m_worldWidth, m_worldHeight - (y / h) * m_worldHeight))
		{
			m_ghost2->active = true;
			m_ghost2->pos.Set((x / w) * m_worldWidth, m_worldHeight - (y / h) * m_worldHeight);
			++m_clickcount;
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		if (m_clickcount == 2)
		{
			//double x, y;
			//Application::GetCursorPos(&x, &y);
			//int w = Application::GetWindowWidth();
			//int h = Application::GetWindowHeight();

			////Exercise 6: spawn small GO_BALL
			//GameObject* smallBall = FetchGO();
			//smallBall->pos = m_ghost->pos;
			//smallBall->vel = m_ghost->pos - Vector3(m_worldWidth * (x / w), m_worldHeight - m_worldHeight * (y / h), 0.f);
			//smallBall->scale.Set(2, 2, 2);
			//smallBall->mass = 8;
			//m_ghost->active = false;

			makeThinWall((m_ghost2->pos + m_ghost->pos) * 0.5f, (m_ghost2->pos - m_ghost->pos).Normalize(), 2, (m_ghost2->pos - m_ghost->pos).Length() * 0.5f);
			m_ghost->active = false;
			m_ghost2->active = false;
			m_clickcount = 0;

		}
	}
	static bool bRButtonState = false;
	if (Application::IsMousePressed(1))
	{
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();

		if (!bRButtonState)
		{
			bRButtonState = true;
			std::cout << "RBUTTON DOWN" << std::endl;

			//Exercise 10: spawn ghost ball
			m_ghost->active = true;
			m_ghost->pos.Set(m_worldWidth * (x / w), m_worldHeight - m_worldHeight * (y / h));
			m_ghost->vel.SetZero();
		}
		//m_ballScale = (m_ghost->pos - Vector3(m_worldWidth * (x / w), m_worldHeight - m_worldHeight * (y / h))).Length();
		//m_ballScale = Math::Clamp<float>(m_ballScale, 2, 10);
		m_ballScale = (2, 2, 2);
		m_ghost->scale.Set(m_ballScale, m_ballScale, m_ballScale);

	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();

		m_ghost->active = false;

		//SPAWN BIG BALL
		GameObject* bigBall = FetchGO();
		bigBall->pos = m_ghost->pos;
		bigBall->vel = m_ghost->pos - Vector3(m_worldWidth * (x / w), m_worldHeight - m_worldHeight * (y / h), 0.f);
		bigBall->scale = m_ghost->scale;
		bigBall->mass = bigBall->scale.x * bigBall->scale.x * bigBall->scale.x;
	}

	//Physics Simulation Section

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			//Vector3 relativeMiddle = Vector3(m_worldWidth / 2.f, m_worldHeight / 2.f, 0) - go->pos;
			Vector3 relativeMiddle = Vector3(0, -1);
			if (go->type == GameObject::GO_BALL && go->timer < 5)
			{
				go->vel += (relativeMiddle * (1.f / relativeMiddle.Length() * m_gravity)) * dt * m_speed; //gravity
			}

			go->pos += go->vel * dt * m_speed;
			//Exercise 7: handle out of bound game objects // Original by Rayn
			if (go->pos.x >= m_worldWidth + go->scale.x || go->pos.x <= 0 - go->scale.x || go->pos.y >= m_worldHeight + go->scale.y || go->pos.y <= 0 - go->scale.y)
			{
				ReturnGO(go);
				std::cout << "Unspawned ball" << std::endl;
				continue;
			}

			//// bounceback
			//if (go->pos.x <= 0 + go->scale.x && go->vel.x < 0 || go->pos.x >= m_worldWidth - go->scale.x && go->vel.x > 0)
			//{
			//	go->vel.x = -go->vel.x * m_restitutionCoeff;
			//	bounced = false;
			//}

			//if ((go->pos.y < go->scale.y && go->vel.y < 0) || go->pos.y > m_worldHeight - go->scale.y && go->vel.y > 0)
			//	go->vel.y = -go->vel.y;

			//Exercise 8a: handle collision between GO_BALL and GO_BALL using velocity swap
			for (std::vector<GameObject*>::iterator it2 = it + 1; it2 != m_goList.end(); it2++)
			{
				GameObject* go2 = (GameObject*)*it2;

				if (go2->active)
				{
					GameObject* ball = go;
					GameObject* other = go2;
					if (ball->type != GameObject::GO_BALL)
					{
						//swap
						ball = go2;
						other = go;
					}

					if (CheckCollision(ball, other, 0.f))
					{
						CollisionResponse(ball, other);

						if (ball->vel.Length() > 0.5f)
						{
							ball->timer = 0;
						}
						else
						{
							ball->vel.SetZero();
						}
					}
				}
			}



			//Exercise 8b: store values in auditing variables

			//Exercise 10: handle collision using momentum swap instead

			//Exercise 12: improve inner loop to prevent double collision

			//Exercise 13: improve collision detection algorithm [solution to be given later] 
		}
	}
}

void SceneCollision::CollisionResponse(GameObject* go, GameObject* go2)
{
	if (go->type == GameObject::GO_BALL && go2->type == GameObject::GO_BALL)
	{
		m1 = go->mass;
		m2 = go2->mass;
		u1 = go->vel;
		u2 = go2->vel;

		//COLLISION RESPONSE
		v1 = (u1 * (m1 - m2) + 2 * m2 * u2) * (1 / (m1 + m2));
		v2 = (u2 * (m2 - m1) + 2 * m1 * u1) * (1 / (m1 + m2));

		//Vector3 collisionNormal = go->pos = go2->pos;
		//float distSq = collisionNormal.LengthSquared();
		//float dp = (u1 - u2).Dot(go->pos - go2->pos);
		//v1 = u1 - ((2 * m2) / (m1 + m2)) * (dp / distSq) * collisionNormal;
		//v2 = u2 - ((2 * m1) / (m1 + m2)) * (dp / distSq) * -collisionNormal;

		go->vel = v1 * m_restitutionCoeff;
		go2->vel = v2 * m_restitutionCoeff;
	}
	if (go->type == GameObject::GO_BALL && go2->type == GameObject::GO_WALL)
	{
		go->vel -= (2 * go->vel.Dot(go2->normal) * go2->normal) * m_restitutionCoeff;
	}

	else if (go->type == GameObject::GO_BALL && go2->type == GameObject::GO_PILLAR)
	{
		Vector3 normal = (go->pos - go2->pos).Normalized();
		go->vel -= (2 * go->vel.Dot(normal) * normal) * m_restitutionCoeff;
	}

	if (go->type == GameObject::GO_BALL && go2->type == GameObject::GO_PWALL)
	{
			go->vel -= (2 * go->vel.Dot(go2->normal) * go2->normal) + (2 * Math::PI * ((go->pos - go2->pos).LengthSquared()) * Math::DegreeToRadian(m_rotationSpeed));
	}

	else if (go->type == GameObject::GO_BALL && go2->type == GameObject::GO_PPILLAR)
	{
			Vector3 normal = (go->pos - go2->pos).Normalized();
			go->vel -= (2 * go->vel.Dot(normal) * normal) + (2 * Math::PI * ((go->pos - m_cWall[0]->pos).LengthSquared()) * Math::DegreeToRadian(m_rotationSpeed));
	}
}

void SceneCollision::makeThinWall(float x, float y, float angleDeg, float thickness, float height)
{
	GameObject* wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	//wall->ambience.Set(Math::RandFloatMinMax(0.f, 1.f), Math::RandFloatMinMax(0.f, 1.f), Math::RandFloatMinMax(0.f, 1.f));
	wall->pos.Set(x, y, 0);
	wall->normal.Set(cosf(Math::DegreeToRadian(angleDeg)), sinf(Math::DegreeToRadian(angleDeg)));
	wall->scale.Set(thickness, height, 1.f);
	wall->vel.Set(0, 0, 0);

	Vector3 np(wall->normal.y, -wall->normal.x, 0);
	Vector3 positions[2] = { wall->pos + np * height, wall->pos - np * height };

	for (int i = 0; i < 2; ++i)
	{
		GameObject* pillar = FetchGO();
		pillar->type = GameObject::GO_PILLAR;
		//pillar->ambience.Set(Math::RandFloatMinMax(0.f, 1.f), Math::RandFloatMinMax(0.f, 1.f), Math::RandFloatMinMax(0.f, 1.f));
		pillar->pos = positions[i];
		pillar->vel.Set(0, 0, 0);
		pillar->scale.Set(thickness, thickness, thickness);
	}
}

void SceneCollision::makeThinWall(Vector3 pos, Vector3 vNormal, float thickness, float height)
{
		GameObject* wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	//wall->ambience.Set(Math::RandFloatMinMax(0.f, 1.f), Math::RandFloatMinMax(0.f, 1.f), Math::RandFloatMinMax(0.f, 1.f));
	wall->pos.Set(pos.x, pos.y, 0);
	wall->normal = Vector3(vNormal.y, -vNormal.x, 0);
	wall->scale.Set(thickness, height, 1.f);
	wall->vel.Set(0, 0, 0);

	Vector3 np(wall->normal.y, -wall->normal.x, 0);
	Vector3 positions[2] = { wall->pos + np * height, wall->pos - np * height };

	for (int i = 0; i < 2; ++i)
	{
		GameObject* pillar = FetchGO();
		pillar->type = GameObject::GO_PILLAR;
		//pillar->ambience.Set(Math::RandFloatMinMax(0.f, 1.f), Math::RandFloatMinMax(0.f, 1.f), Math::RandFloatMinMax(0.f, 1.f));
		pillar->pos = positions[i];
		pillar->vel.Set(0, 0, 0);
		pillar->scale.Set(thickness, thickness, thickness);
	}
}

//void SceneCollision::getPlayerPosition(int player)
//{
//	static bool jumpstate = false;
//	if (!jumpstate && Application::IsKeyPressed('W'))
//	{
//		jumpstate = true;
//		if (jump == false)
//		{
//			jump = true;
//			jumptarget = m_player[0]->pos.y + jumpheight;
//		}
//	}
//	else if (jumpstate && !Application::IsKeyPressed('W'))
//	{
//		jumpstate = false;
//	}
//	if (Application::IsKeyPressed('A'))
//	{
//		m_player[0]->dir = Vector3(-1, 0);
//	}
//
//	else if (Application::IsKeyPressed('D'))
//	{
//		m_player[0]->dir = Vector3(1, 0);
//	}
//
//	if (m_player[1]->dir == Vector3(0, 0))
//	{
//		m_player[1]->dir = Vector3(1, 0);
//	}
//}


void SceneCollision::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_BALL:
	{
		//Exercise 4: render a sphere using scale and pos
		modelStack.PushMatrix();
		{
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_BALL], false);
		}
		modelStack.PopMatrix();
		//Exercise 11: think of a way to give balls different colors
		break;
	}

	case GameObject::GO_WALL:
	{
		modelStack.PushMatrix();
		{
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Rotate(Math::RadianToDegree(atan2f(go->normal.y, go->normal.x)), 0, 0, 1);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_CUBE], false);
		}
		modelStack.PopMatrix();
		break;
	}

	case GameObject::GO_PILLAR:
	{
		modelStack.PushMatrix();
		{
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			modelStack.Rotate(90, 1, 0, 0);
			RenderMesh(meshList[GEO_CYLINDER], false);
		}
		modelStack.PopMatrix();
		break;
	}

	case GameObject::GO_PWALL:
	{
		modelStack.PushMatrix();
		{
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Rotate(Math::RadianToDegree(atan2f(go->normal.y, go->normal.x)), 0, 0, 1);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			RenderMesh(meshList[GEO_CUBE], false);
		}
		modelStack.PopMatrix();
		break;
	}
	case GameObject::GO_PPILLAR:
	{
		modelStack.PushMatrix();
		{
			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
			modelStack.Rotate(90, 1, 0, 0);
			RenderMesh(meshList[GEO_CYLINDER], false);
		}
		modelStack.PopMatrix();
		break;
	}	
	}
}

void SceneCollision::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
	
	RenderMesh(meshList[GEO_AXES], false);

	if (m_ghost->active)
	{
		RenderGO(m_ghost);
	}

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			RenderGO(go);
		}
	}

	//On screen text
	std::ostringstream Imomentum;
Imomentum.precision(3);
Imomentum << "P1: " << (m1 * u1 + m2 * u2).Length() << "kg(m/s)";
RenderTextOnScreen(meshList[GEO_TEXT], Imomentum.str(), Color(1, 1, 1), 3, 0, 12);

std::ostringstream Fmomentum;
Fmomentum.precision(3);
Fmomentum << "P2: " << (m1 * v1 + m2 * v2).Length() << "kg(m/s)";
RenderTextOnScreen(meshList[GEO_TEXT], Fmomentum.str(), Color(1, 1, 1), 3, 0, 15);

std::ostringstream ss6;
ss6.precision(5);
ss6 << "KE1: " << 0.5 * (m1 * u1.LengthSquared() + m2 * u2.LengthSquared()) << "J";
RenderTextOnScreen(meshList[GEO_TEXT], ss6.str(), Color(0, 1, 0), 3, 0, 21);

ss6.str("");
ss6.precision(5);
ss6 << "KE2: " << 0.5 * (m1 * v1.LengthSquared() + m2 * v2.LengthSquared()) << "J";
RenderTextOnScreen(meshList[GEO_TEXT], ss6.str(), Color(0, 1, 0), 3, 0, 18);


	//Exercise 5: Render m_objectCount

	std::ostringstream ss;
	ss.precision(3);
	ss << "objCount: " << m_objectCount;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);

	//Exercise 8c: Render initial and final momentum

	std::ostringstream ss2;
	ss2.precision(3);
	ss2 << "Speed: " << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss2.str(), Color(0, 1, 0), 3, 0, 6);
	
	std::ostringstream ss3;
	ss3.precision(5);
	ss3 << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 3, 0, 3);
	
	RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);
}

void SceneCollision::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
