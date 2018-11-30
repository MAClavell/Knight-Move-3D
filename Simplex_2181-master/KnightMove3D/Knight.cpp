#include "Knight.h"

using namespace Simplex;

//Constructor
Knight::Knight(Simplex::String fileName, Simplex::String uniqueID, Board* brd, SystemSingleton* a_system) : Entity(fileName, uniqueID)
{
	//Initialize entity manager and entity
	entityMngr = EntityManager::GetInstance();
	entityMngr->AddEntity(fileName, uniqueID);

	system = a_system;

	//Add board and set initial position
	board = brd;
	gridIndex = vector2(0, 0);
	this->SetPosition(board->GetKnightPositionOnTile((int)gridIndex.x, (int)gridIndex.y));

	//Initialize lerp values
	origin = brd->GetTile(vector2(0, 0));
	destination = brd->GetTile(vector2(1, 2));

}

//Descructor
Knight::~Knight()
{ }

//Set the position of the knight
void Knight::SetPosition(vector3 newPos)
{
	matrix4 matrix = glm::translate(newPos) * glm::scale(vector3(0.25f, 0.25f, 0.25f));
	entityMngr->GetEntity(entityMngr->GetEntityIndex("Knight"))->SetModelMatrix(matrix);
}

//moves the knight in a given direction
void Knight::MoveKnight(int dir)
{
	if (dir < 0 || dir > 4)
		return;

	//REMEMBER: the knightGridPos vector2 is reversed
	//knightGridPos.x is the row of the grid (max 4)
	//knightGridPos.y is the column of the grid (max 8)
	switch (dir)
	{
		//Up
	case 1:
		if (gridIndex.x > 0)
			gridIndex.x--;
		break;
		//Left
	case 2:
		if (gridIndex.y > 0)
			gridIndex.y--;
		break;
		//Down
	case 3:
		if (gridIndex.x < board->GetBoardDimensions().x - 1)
			gridIndex.x++;
		break;
		//Right
	case 4:
		if (gridIndex.y < board->GetBoardDimensions().y - 1)
			gridIndex.y++;
		break;
	}

	this->SetPosition(board->GetKnightPositionOnTile((int)gridIndex.x, (int)gridIndex.y));
}

void Knight::Jump()
{

	static float fTimer = 0;//store the new timer
	static uint uClock =system->GenClock(); //generate a new clock for that timer
	fTimer += system->GetDeltaTime(uClock); //get the delta time for that timer

	//map the percentage to be between 0.0 and 1.0
	fPercentage = MapValue(fTimer, 0.0f, fTimeBetweenStops, 0.0f, 1.0f);

	//calculate the current position
	vector3 v3CurrentPos = glm::lerp(origin.GetKnightPosition(), destination.GetKnightPosition(), fPercentage);
	float arc = sin(fPercentage * 3.14f);
	v3CurrentPos.y += arc * maxHeight;
	matrix4 m4Model = glm::translate(IDENTITY_M4, v3CurrentPos);

	//if we are done with this route
	if (fPercentage >= 1.0f)
	{
		fTimer = m_pSystem->GetDeltaTime(uClock);//restart the clock
		Land(destination); 
	}
}

void Knight::Land(Tile target)
{
	//TO DO: Check if knight dies

	//TO DO: Check if heart is here (or do this in Step()?)

	//Decrement target's health
	target.Step();

	//TO DO: Reset LERP variables (time, origin)
	origin = target;

	//Replace validMoves
	validMoves.clear();

	//Iterate through tile's array of moves
	for (int i = 0; i < 8; i++)
	{
		//If the coordinate is on the board, add it to the list of valid moves
		vector2 move = target.GetMoves()[i];
		if (move.x >= 0 && move.x < 8 && move.y >= 0 && move.y < 4)
		{
			validMoves.push_back(board->GetTile(move));
		}
	}

	//Set new default destination
	//destination = validMoves[0];
}
