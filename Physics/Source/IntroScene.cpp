//#include "IntroScene.h"
//#include "GL\glew.h"
//#include "Application.h"
//#include <sstream>
//
//IntroScene::IntroScene()
//{
//}
//
//IntroScene::~IntroScene()
//{
//}
//
//void IntroScene::Init()
//{
//	SceneBase::Init();
//
//	//calculating aspect ratio
//	m_worldHeight = 100.f;
//	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
//
//	//Physics code here //i beg you
//	m_speed = 1.f;
//	m_objectCount = 0;
//}
//
//GameObject* IntroScene::FetchGO()
//{
//	//Exercise 2a: implement FetchGO() 
//	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); it++)
//	{
//		GameObject* go = (GameObject*)*it;
//		if (go->active == false)
//		{
//			go->active = true;
//			//Exercise 2b: increase object count every time an object is set to active
//			++m_objectCount;
//			return go;
//		}
//	}
//
//	for (int i = 0; i < 10; ++i)
//	{
//		m_goList.push_back(new GameObject(GameObject::GO_BALL));
//	}
//
//	//Exercise 2b: increase object count every time an object is set to active
//	m_goList.back()->active = true;
//	++m_objectCount;
//	return m_goList.back();
//}
//
//void IntroScene::ReturnGO(GameObject* go)
//{
//	//Exercise 3: implement ReturnGO()
//	if (go->active)
//	{
//		go->active = false;
//		--m_objectCount;
//	}
//}
//
//void IntroScene::Update(double dt)
//{
//	SceneBase::Update(dt);
//}
//
//void IntroScene::RenderGO(GameObject* go)
//{
//	switch (go->type)
//	{
//	case GameObject::GO_INTRO:
//	{
//		//Exercise 4: render a sphere using scale and pos
//		modelStack.PushMatrix();
//		{
//			modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
//			modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
//			RenderMesh(meshList[GEO_INTRO], false);
//		}
//		modelStack.PopMatrix();
//		//Exercise 11: think of a way to give balls different colors
//		break;
//	}
//	}
//}
//
//void IntroScene::Render()
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	//Calculating aspect ratio
//	m_worldHeight = 100.f;
//	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
//
//	// Projection matrix : Orthographic Projection
//	Mtx44 projection;
//	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
//	projectionStack.LoadMatrix(projection);
//
//	// Camera matrix
//	viewStack.LoadIdentity();
//	viewStack.LookAt(
//		camera.position.x, camera.position.y, camera.position.z,
//		camera.target.x, camera.target.y, camera.target.z,
//		camera.up.x, camera.up.y, camera.up.z
//	);
//	// Model matrix : an identity matrix (model will be at the origin)
//	modelStack.LoadIdentity();
//
//	RenderMesh(meshList[GEO_AXES], false);
//
//	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
//	{
//		GameObject* go = (GameObject*)*it;
//		if (go->active)
//		{
//			RenderGO(go);
//		}
//	}
//
//	std::ostringstream ss3;
//	ss3.precision(5);
//	ss3 << "FPS: " << fps;
//	RenderTextOnScreen(meshList[GEO_TEXT], ss3.str(), Color(0, 1, 0), 3, 0, 3);
//
//	RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);
//}
//
//void IntroScene::Exit()
//{
//	SceneBase::Exit();
//	//Cleanup GameObjects
//	while (m_goList.size() > 0)
//	{
//		GameObject* go = m_goList.back();
//		delete go;
//		m_goList.pop_back();
//	}
//}
