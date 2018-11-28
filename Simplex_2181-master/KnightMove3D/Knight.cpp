#include "Knight.h"
using namespace Simplex;

Knight::Knight(Simplex::String fileName, Simplex::String uniqueID, vector3* gridPositions[4][8]) : Entity(fileName, uniqueID)
{
	entityMngr->AddEntity(fileName, uniqueID);

	//Copy grid
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			grid[i][j] = gridPositions[i][j];
		}
	}

	vector3 v3Position = *gridPositions[0][0];
	gridIndices = new vector2(0, 0);
	matrix4 m4Matrix = glm::translate(v3Position) * glm::scale(vector3(0.25f, 0.25f, 0.25f));
	entityMngr->SetModelMatrix(m4Matrix, -1);
}


Knight::~Knight()
{
	//Delete heap vars
	SafeDelete(gridIndices);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			SafeDelete(grid[i][j]);
		}
	}
}

//moves the knight in a given direction
void Knight::MoveKnight(int dir)
{
	if (dir < 0 || dir > 4)
		return;

	Entity* knight = entityMngr->GetEntity(entityMngr->GetEntityIndex("Knight"));

	//REMEMBER: the knightGridPos vector2 is reversed
	//knightGridPos.x is the row of the grid (max 4)
	//knightGridPos.y is the column of the grid (max 8)
	switch (dir)
	{
		//Up
	case 1:
		if (gridIndices->x > 0)
			gridIndices->x--;
		break;
		//Left
	case 2:
		if (gridIndices->y > 0)
			gridIndices->y--;
		break;
		//Down
	case 3:
		if (gridIndices->x < 3)
			gridIndices->x++;
		break;
		//Right
	case 4:
		if (gridIndices->y < 7)
			gridIndices->y++;
		break;
	}

	matrix4 matrix = glm::translate(*grid[(int)gridIndices->x][(int)gridIndices->y])
		* glm::scale(vector3(0.25f, 0.25f, 0.25f));
	knight->SetModelMatrix(matrix); //"Knight");
}

void Simplex::Knight::Jump()
{
	////get the percentage
	//float fTimeBetweenStops = 2.0;//in seconds
	////map the value to be between 0.0 and 1.0
	//float fPercentage = MapValue(fTimer, 0.0f, fTimeBetweenStops, 0.0f, 1.0f);

	////calculate the current position
	//vector3 v3CurrentPos = glm::lerp(v3Start, v3End, fPercentage);
	//float arc = sin(fPercentage * MATH_PI);
	//v3CurrentPos.y += arc * maxHeight;
	//matrix4 m4Model = glm::translate(IDENTITY_M4, v3CurrentPos);

	////if we are done with this route
	//if (fPercentage >= 1.0f)
	//{
	//	fTimer = m_pSystem->GetDeltaTime(uClock);//restart the clock
	//	Land(destination); 
	//}
}
