#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 10.0f, 10.0f), //Position
		vector3(0.0f, 0.0f, 0.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

#ifdef DEBUG
	uint uInstances = 900;
#else
	uint uInstances = 1849;
#endif
	int nSquare = static_cast<int>(std::sqrt(uInstances));
	m_uObjects = nSquare * nSquare;
	uint uIndex = -1;
	//for (int i = 0; i < nSquare; i++)
	//{
	//	for (int j = 0; j < nSquare; j++)
	//	{
	//		uIndex++;
	//		m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
	//		vector3 v3Position = vector3(glm::sphericalRand(34.0f));
	//		matrix4 m4Position = glm::translate(v3Position);
	//		m_pEntityMngr->SetModelMatrix(m4Position);
	//		m_pEntityMngr->UsePhysicsSolver(m_bUsingPhysics); //Apply physics to the objects
	//	}
	//}
	m_uOctantLevels = 1;
	m_pRoot = new Octant(m_uOctantLevels,5);

	//Creates Board
	float squareSize = 1.0f;
	float xOffset = 4 * squareSize;
	float zOffset = 2 * squareSize;

	for (uint i = 0; i < 4; i++)
	{
		for (uint j = 0; j < 8; j++)
		{
			m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", "Grid(" + std::to_string(i) + "," + std::to_string(j) + ")");
			vector3* v3Position = new vector3(j * squareSize - xOffset, 0, i * squareSize - zOffset);
			matrix4 m4Position = glm::translate(*v3Position);
			m_pEntityMngr->SetModelMatrix(m4Position, -1);

			//Use modified position for knight movement
			v3Position->x += 0.5f;
			v3Position->y += 1;
			v3Position->z += 0.5f;
			gridPositions[i][j] = v3Position;
		}
	}

	//Adds Player Model
	m_pEntityMngr->AddEntity("KnightMove3D\\knight.obj", "Knight");
	vector3 v3Position = *gridPositions[0][0];
	knightGridPos = new vector2(0, 0);
	matrix4 m4Matrix = glm::translate(v3Position) * glm::scale(vector3(0.25f, 0.25f, 0.25f));
	m_pEntityMngr->SetModelMatrix(m4Matrix, -1);
	
	//m_pMeshMngr->AddCubeToRenderList(m4Position, vector3(1.0f, 0.0f, 0.0f));

	m_pEntityMngr->Update();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Reconstructing the Octree each half a second
	if (m_bUsingPhysics)
	{
		static uint nClock = m_pSystem->GenClock();
		static bool bStarted = false;
		if (m_pSystem->IsTimerDone(nClock) || !bStarted)
		{
			bStarted = true;
			m_pSystem->StartTimerOnClock(0.5, nClock);
			SafeDelete(m_pRoot);
			m_pRoot = new Octant(m_uOctantLevels, 5);
		}
	}
	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//display octree
	if (m_uOctantID == -1)
		m_pRoot->Display();
	else
		m_pRoot->Display(m_uOctantID);
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui,
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Release the octree
	SafeDelete(m_pRoot);
	SafeDelete(knightGridPos);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			SafeDelete(gridPositions[i][j]);
		}
	}

	//release GUI
	ShutdownGUI();
}

void Application::MoveKnight(int dir)
{
	if (dir < 0 || dir > 4)
		return;
	
	Entity* knight = m_pEntityMngr->GetEntity(m_pEntityMngr->GetEntityIndex("Knight"));

	//REMEMBER: the knightGridPos vector2 is reversed
	//knightGridPos.x is the row of the grid (max 4)
	//knightGridPos.y is the column of the grid (max 8)
	switch (dir)
	{
		//Up
		case 1:
			if (knightGridPos->x > 0)
				knightGridPos->x--;
			break;
		//Left
		case 2:
			if (knightGridPos->y > 0)
				knightGridPos->y--;
			break;
		//Down
		case 3:
			if (knightGridPos->x < 3)
				knightGridPos->x++;
			break;
		//Right
		case 4:
			if (knightGridPos->y < 7)
				knightGridPos->y++;
			break;
	}

	matrix4 matrix = glm::translate(*gridPositions[(int)knightGridPos->x][(int)knightGridPos->y])
		* glm::scale(vector3(0.25f, 0.25f, 0.25f));
	knight->SetModelMatrix(matrix); //"Knight");
}