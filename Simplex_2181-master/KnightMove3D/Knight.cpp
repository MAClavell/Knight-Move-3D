#include "Knight.h"

using namespace Simplex;

//Constructor
Knight::Knight(String fileName, String uniqueID, Board* brd, SystemSingleton* a_system)
{
	//Initialize entity manager and entity
	entityMngr = EntityManager::GetInstance();
	entityMngr->AddEntity(fileName, uniqueID);
	this->uniqueID = uniqueID;

	system = a_system;

	//Add board and set initial position
	board = brd;
	gridIndex = vector2(0, 0);
	this->SetPosition(board->GetKnightPositionOnTile(gridIndex));

	//Initialize lerp values
	origin = brd->GetTile(vector2(0, 0));
	destination = brd->GetTile(vector2(1, 2));

	falling = false;
	Land(origin);
}

//Descructor
Knight::~Knight()
{ }

//Set the position of the knight
void Knight::SetPosition(vector3 newPos)
{
	matrix4 matrix = glm::translate(newPos) * glm::scale(vector3(0.25f, 0.25f, 0.25f));
	entityMngr->GetEntity(entityMngr->GetEntityIndex(uniqueID))->SetModelMatrix(matrix);
}

void Knight::Jump()
{
	static float fTimer = 0;//store the new timer
	static uint uClock = system->GenClock(); //generate a new clock for that timer
	float delta = system->GetDeltaTime(uClock); //get the delta time for that timer

	//If the heart is being placed, don't jump
	//Has to be placed after timer calculation
	if (board->IsPlacingHeart() || falling)
		return;

	//map the percentage to be between 0.0 and 1.0
	fTimer += delta; //add that delta to the timer
	fPercentage = MapValue(fTimer, 0.0f, fTimeBetweenStops, 0.0f, 1.0f);

	//calculate the current position
	vector3 v3CurrentPos = glm::lerp(origin.GetKnightPosition(), destination.GetKnightPosition(), fPercentage);
	float arc = sin(fPercentage * 3.14f);
	v3CurrentPos.y += arc * maxHeight;
	matrix4 m4Model = glm::translate(IDENTITY_M4, v3CurrentPos) * rotation * glm::scale(vector3(0.25f, 0.25f, 0.25f));
	entityMngr->GetEntity(entityMngr->GetEntityIndex(uniqueID))->SetModelMatrix(m4Model);

	//if we are done with this route
	if (fPercentage >= 1.0f)
	{
		fTimer = system->GetDeltaTime(uClock);//restart the clock
		Land(destination); 
	}
}

void Knight::Land(Tile target)
{
	//TO DO: Check if knight dies
	if (!target.IsAlive())
	{
		falling = true;
	}

	//Check if the heart is on this tile
	board->HandleIfOnHeart(target.coordinate);

	//Decrement target's health
	target.Step();

	//Set this tile as the new origin
	origin = target;

	//Replace validMoves
	validMoves.clear();

	//Iterate through tile's array of moves
	for (int i = 0; i < 8; i++)
	{
		//If the coordinate is on the board, add it to the list of valid moves
		vector2 move = target.GetMoves()[i];
		if (move.x >= 0 && move.x < 4 && move.y >= 0 && move.y < 8)
		{
			validMoves.push_back(board->GetTile(move));
		}
	}

	//Set new default destination
	destination = validMoves[0];
	destinationIndex = 0;

	SetRotation(origin, destination);
}

void Simplex::Knight::SetSpeed(float newTime)
{
	fTimeBetweenStops = newTime;
}

void Simplex::Knight::ChangeMove(bool clockwise)
{
	if (clockwise)
	{
		destinationIndex++;
		if (destinationIndex >= validMoves.size())
			destinationIndex = 0;
		
	}
	else
	{
		destinationIndex--;
		if (destinationIndex < 0)
			destinationIndex = validMoves.size() - 1;

	}

	destination = validMoves[destinationIndex];

	SetRotation(origin, destination);
}

matrix4 Simplex::Knight::SetRotation(Tile start, Tile end)
{
	vector3 distance = end.GetKnightPosition() - start.GetKnightPosition();
	float angle = glm::degrees(atan2f(distance.x, distance.y));
	matrix4 newRotation = ToMatrix4(glm::angleAxis(angle, vector3(0.0f, 1.0f, 0.0f)));
	rotation = newRotation;
	return newRotation;
}
