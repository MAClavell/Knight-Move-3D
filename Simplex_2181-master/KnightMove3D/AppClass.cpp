#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Seed the random number generator
	srand(time(NULL));

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 6.0f, 5.0f), //Position
		vector3(0.0f, 0.0f, 0.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 10.0f, 0.0f), 1); //set the position of first light (0 is reserved for ambient light)
	m_pLightMngr->SetIntensity(2, 1);

	m_uOctantLevels = 2;
	m_pRoot = new Octant(m_uOctantLevels, 5);
	
	//Create the board
	board = new Board(m_pSystem);

	//Adds Player Model
	knight = new Knight("KnightMove3D\\knight.obj", "Knight", board, m_pSystem);

	//Adds Enemy Model
	rook = new Rook("KnightMove3D\\rook.obj", "Rook", board, m_pSystem);
	
	//Set control variables
	gameOver = false;

	m_pEntityMngr->Update();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Update Entities
	knight->Jump();
	rook->Jump();
	board->Update();

	//Display screen
	if (!knight->IsAlive() && !gameOver)
	{
		gameOver = true;
	}

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
	
	//Display board
	board->Display();

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
void Application::ResetGame()
{
	board->Reset();
	knight->Reset();
	gameOver = false;
}
void Application::Release(void)
{
	//Release the octree
	SafeDelete(m_pRoot);
	SafeDelete(knight);
	SafeDelete(rook);
	SafeDelete(board);

	//release GUI
	ShutdownGUI();
}