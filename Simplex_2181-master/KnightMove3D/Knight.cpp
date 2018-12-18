#include "Knight.h"

using namespace Simplex;

#define FALL_TARGET -30
#define GRAVITY -9.8

//Constructor
Knight::Knight(String fileName, String uniqueID, Board* brd, uint rookEntityIndex, SystemSingleton* a_system)
{
	//Initialize entity manager and entity
	entityMngr = EntityManager::GetInstance();
	entityMngr->AddEntity(fileName, uniqueID);
	this->uniqueID = uniqueID;
	entityIndex = entityMngr->GetEntityIndex(uniqueID);
	this->rookEntityIndex = rookEntityIndex;
	entityMngr->GetRigidBody(uniqueID)->SetVisibleOBB(false);
	system = a_system;

	//Add board and set initial position
	board = brd;
	gridIndex = vector2(0, 0);
	this->SetPosition(board->GetKnightPositionOnTile(gridIndex));

	//Initialize lerp values
	origin = brd->GetTile(vector2(0, 0));
	destination = brd->GetTile(vector2(1, 2));
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
Knight::~Knight()
{ }

//Set the position of the knight
void Knight::SetPosition(vector3 newPos)
{
	matrix4 matrix = glm::translate(newPos) * rotation;// *glm::scale(vector3(0.25f, 0.25f, 0.25f));
	entityMngr->GetRigidBody(uniqueID)->SetModelMatrix(matrix);
	entityMngr->SetModelMatrix(matrix, uniqueID);
}

//Makes the knight fall downwards
void Knight::Fall()
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

//Interpolates knight from origin tile to destination tile
void Knight::Jump()
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

	//Detect collisions with the rook
	if (entityMngr->GetEntity(entityIndex)->IsColliding(entityMngr->GetEntity(rookEntityIndex)))
	{
		//TODO do something with collisions
		std::cout << "colliding" << std::endl;
		falling = 2;
	}

	//map the percentage to be between 0.0 and 1.0
	if(!speeding) fTimer += delta; //add that delta to the timer
	else
	{
		fTimer += delta * 7;
		board->AddToScore(delta * 10);
	}
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
void Knight::Land(Tile* target, bool stepTile)
{
	//TO DO: Check if knight dies
	if (!target->IsAlive())
	{
		falling = 1;
		yPos = destination->GetKnightPosition().y;
		system->GetDeltaTime(uClock); //reset the clock
		return;
	}

	//Check if the heart is on this tile
	board->HandleIfOnHeart(target->coordinate);

	//Decrement target's health
	if(stepTile)
		target->Step();
	 
	//Add the combo if this tile is dead
	if (!target->IsAlive())
	{
		//Combo increments based on the original knight move
		//The more tiles you break, the more the combo increases
		//Caps out at +2000
		if (combo < 350)
			combo += 50;
		else if (combo < 1000)
			combo = 1000;
		else if (combo < 1600)
			combo += 100;
		else if (combo < 3000)
			combo = 3000;
		else if (combo < 6000)
			combo += 500;
		else if (combo < 10000)
			combo = 10000;
		else
			combo += 2000;

		board->AddToScore(combo);
	}
	else combo = 0;

	//Set this tile as the new origin
	origin = target;

	//Replace validMoves
	validMoves.clear();

	//Iterate through tile's array of moves
	for (int i = 0; i < 8; i++)
	{
		//If the coordinate is on the board, add it to the list of valid moves
		vector2 move = target->GetMoves()[i];
		if (move.x >= 0 && move.x < 4 && move.y >= 0 && move.y < 8)
		{
			validMoves.push_back(board->GetTile(move));
		}
	}

	//Set new default destination
	destination = validMoves[0];
	destinationIndex = 0;

	SetRotation(origin, destination);
	board->MoveReticule(destination->GetKnightPosition());
}

//Sets fTimeBetweenStops
void Simplex::Knight::SetSpeed()
{
	if (!speeding) speeding = true;
}

//Changes destination of knight's current jump
void Simplex::Knight::ChangeMove(bool clockwise)
{
	if (falling > 0)
		return;

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
	board->MoveReticule(destination->GetKnightPosition());
}

//Sets rotation matrix to make knight face correct direction
matrix4 Simplex::Knight::SetRotation(Tile* start, Tile* end)
{
	vector3 distance = end->GetKnightPosition() - start->GetKnightPosition();
	float angle = atan2(distance.x, distance.z);
	matrix4 newRotation = ToMatrix4(glm::angleAxis(angle, vector3(0.0f, 1.0f, 0.0f)));
	rotation = newRotation;
	return newRotation;
}

void Simplex::Knight::SlowDown()
{
	speeding = false;
}

//Reset the board
void Knight::Reset()
{
	falling = 0;
	combo = 0;
	gridIndex = vector2(0, 0);

	//Set initial position
	this->SetPosition(board->GetKnightPositionOnTile(gridIndex));

	//ReInitialize lerp values
	origin = board->GetTile(vector2(0, 0));
	destination = board->GetTile(vector2(1, 2));
	Land(origin, false);
}

//Check if the knight is alive
bool Knight::IsAlive()
{
	return falling != 2;
}

//Get the current combo of the knight
int Knight::GetCombo()
{
	return combo;
}