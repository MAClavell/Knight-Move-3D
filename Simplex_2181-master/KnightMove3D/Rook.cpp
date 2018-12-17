#include "Rook.h"

using namespace Simplex;

#define FALL_TARGET -30
#define GRAVITY -9.8

//Constructor
Rook::Rook(String fileName, String uniqueID, Board* brd, SystemSingleton* a_system)
{
	//Initialize entity manager and entity
	entityMngr = EntityManager::GetInstance();
	entityMngr->AddEntity(fileName, uniqueID);
	this->uniqueID = uniqueID;
	entityMngr->GetRigidBody(uniqueID)->SetVisibleOBB(true);
	system = a_system;

	//Add board and set initial position
	board = brd;
	gridIndex = vector2(3, 7);
	this->SetPosition(board->GetKnightPositionOnTile(gridIndex));

	//Initialize lerp values
	origin = brd->GetTile(vector2(3, 7));
	destination = brd->GetTile(vector2(1, 7));
	Land(origin, false);

	//Scoring
	combo = 0;

	//Falling vars
	uClock = system->GenClock();
	falling = 0;
	acceleration = 0;
	velocity = 0;
}

//Descructor
Rook::~Rook()
{ }

//Set the position of the Rook
void Rook::SetPosition(vector3 newPos)
{
	matrix4 matrix = glm::translate(newPos) * rotation;// * glm::scale(vector3(0.1f, 0.08f, 0.1f));
	entityMngr->GetRigidBody(uniqueID)->SetModelMatrix(matrix);
	entityMngr->SetModelMatrix(matrix, uniqueID);
}

//Makes the Rook fall downwards
void Rook::Fall()
{
	float delta = system->GetDeltaTime(uClock); //get the delta time for that timer

	if (falling == 2)
		return;

	//Add or subtract to acceleration
	acceleration += GRAVITY;

	//Apply velocity
	velocity += acceleration * delta;
	yPos += velocity * delta;

	//Constrain y
	if (yPos <= FALL_TARGET)
	{
		yPos = FALL_TARGET;
		falling = 2;
	}

	//Set position
	vector3 tilePos = destination->GetKnightPosition();
	SetPosition(vector3(tilePos.x, yPos, tilePos.z));

	//Reset acceleration
	acceleration = 0;
}

//Interpolates Rook from origin tile to destination tile
void Rook::Jump()
{
	static float fTimer = 0;//store the new timer
	static uint uClock = system->GenClock(); //generate a new clock for that timer
	float delta = system->GetDeltaTime(uClock); //get the delta time for that timer

	//If the heart is being placed, don't jump
	//Has to be placed after timer calculation
	if (board->IsPlacingHeart())
		return;

	if (falling > 0)
	{
		this->Fall();
		return;
	}

	//map the percentage to be between 0.0 and 1.0
	fTimer += delta; //add that delta to the timer
	fPercentage = MapValue(fTimer, 0.0f, fTimeBetweenStops, 0.0f, 1.0f);

	//calculate the current position
	vector3 v3CurrentPos = glm::lerp(origin->GetKnightPosition(), destination->GetKnightPosition(), fPercentage);
	float arc = sin(fPercentage * 3.14f);
	v3CurrentPos.y += arc * maxHeight;
	SetPosition(v3CurrentPos);

	//if we are done with this route
	if (fPercentage >= 1.0f)
	{
		fTimer = system->GetDeltaTime(uClock);//restart the clock
		Land(destination);
	}
}

//Called upon the end of a jump
void Rook::Land(Tile* target, bool stepTile)
{
	//TO DO: Check if Rook dies
	if (!target->IsAlive())
	{
		falling = 1;
		yPos = destination->GetKnightPosition().y;
		system->GetDeltaTime(uClock); //reset the clock
		return;
	}

	//Check if the heart is on this tile
	//board->HandleIfOnHeart(target->coordinate);

	//Decrement target's health
	/*if (stepTile)
		target->Step();*/

	//Set this tile as the new origin
	origin = target;

	//Replace validMoves
	validMoves.clear();

	//Set new default destination
	destination = ChooseMove(target);
	destinationIndex = 0;

	SetRotation(origin, destination);
	board->MoveEnemyReticule(destination->GetKnightPosition());
}

//Sets rotation matrix to make Rook face correct direction
matrix4 Simplex::Rook::SetRotation(Tile* start, Tile* end)
{
	vector3 distance = end->GetKnightPosition() - start->GetKnightPosition();
	float angle = atan2(distance.x, distance.z);
	matrix4 newRotation = ToMatrix4(glm::angleAxis(angle, vector3(0.0f, 1.0f, 0.0f)));
	rotation = newRotation;
	return newRotation;
}

//Reset the board
void Rook::Reset()
{
	falling = 0;
	combo = 0;
	gridIndex = vector2(3, 7);

	//Set initial position
	this->SetPosition(board->GetKnightPositionOnTile(gridIndex));

	//ReInitialize lerp values
	origin = board->GetTile(vector2(3, 7));
	destination = board->GetTile(vector2(1, 7));
	Land(origin, false);
}

//Check if the Rook is alive
bool Rook::IsAlive()
{
	return falling != 2;
}

Tile * Simplex::Rook::ChooseMove(Tile * target)
{
	//Chooses a direction with at least 1 space to move to
	int direction = 0;
	int maxSpaces = 0;
	while (maxSpaces == 0)
	{
		direction = rand() % 4;
		maxSpaces = target->GetRookMoves()[direction];
	}

	//Chooses a number of spaces to move in the chosen direction
	int spaces = (rand() % (maxSpaces)) + 1;
	if (spaces >= maxSpaces) spaces = maxSpaces - 1;
	if (spaces <= 0) spaces = 1;

	vector2 newCoord;

	switch (direction)
	{
	case 0:
		newCoord = vector2(target->coordinate.x, target->coordinate.y - spaces);
		break;
	case 1:
		newCoord = vector2(target->coordinate.x + spaces, target->coordinate.y);
		break;
	case 2:
		newCoord = vector2(target->coordinate.x, target->coordinate.y + spaces);
		break;
	case 3:
		newCoord = vector2(target->coordinate.x - spaces, target->coordinate.y);
		break;
	default:
		return target;
		break;
	}

	return board->GetTile(newCoord);
}
