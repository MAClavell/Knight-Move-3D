#include "Tile.h"
using namespace Simplex;

#define MAX_HEALTH 1
#define GRAVITY -9.8
#define FALL_TARGET -30

//Constructor
Tile::Tile(String fileName, String uniqueID, vector3 position, vector2 coord, SystemSingleton* a_system)
{
	//Initialize entity manager and entity
	entityMngr = EntityManager::GetInstance();
	entityMngr->AddEntity(fileName, uniqueID);
	this->uniqueID = uniqueID;

	//Set positon
	matrix4 m4Position = glm::translate(position);
	entityMngr->SetModelMatrix(m4Position, -1);

	//Faling vars
	origPosition = position;
	yPos = origPosition.y;
	acceleration = 0;
	velocity = 0;

	//Set knight position
	position.x += 0.5f;
	position.y += 1;
	position.z += 0.5f;
	knightPosition = vector3(position);

	//Set data
	health = MAX_HEALTH;
	coordinate.x = coord.x;
	coordinate.y = coord.y;
	falling = 0;
	system = a_system;
	fallTimer = 0;
	uClock = system->GenClock(); //generate a new clock for the timer
}

Tile::Tile()
{

}

//Destructor
Tile::~Tile()
{ }

//Gets the position that the knight should be on when on this tile
vector3 Tile::GetKnightPosition()
{
	return knightPosition;
}

//Update the tile every frame
void Tile::Update()
{
	if (falling == 0)
		return;

	//Get the delta time
	float deltaTime = system->GetDeltaTime(uClock);
	fallTimer += deltaTime;
	
	//Add or subtract to acceleration
	if(falling == 1)
		acceleration += GRAVITY;
	if (falling == 2)
		acceleration -= GRAVITY;

	//Apply velocity
	velocity += acceleration * deltaTime;
	yPos += velocity * deltaTime;
	
	//Constrain y
	if (falling == 1 && yPos <= FALL_TARGET)
	{
		yPos = FALL_TARGET;
		falling = 0;
	}
	else if (falling == 2 && yPos >= origPosition.y)
	{
		yPos = origPosition.y;
		falling = 0;
	}

	//Set position
	SetYPosition(yPos);

	//Reset acceleration
	acceleration = 0;
}

//Decrement health and change color
void Tile::Step()
{
	health--;
	if (health <= 0)
	{
		falling = 1;
		velocity = 0;
		uClock = system->GenClock(); //generate a new clock for the timer
	}
	//TODO: change color based on health
}

//Calculate and set possible moves
std::vector<vector2> Tile::GetMoves()
{
	moves[0] = vector2(coordinate.x + 1, coordinate.y + 2);
	moves[1] = vector2(coordinate.x + 2, coordinate.y + 1);
	moves[2] = vector2(coordinate.x + 2, coordinate.y - 1);
	moves[3] = vector2(coordinate.x + 1, coordinate.y - 2);
	moves[4] = vector2(coordinate.x - 1, coordinate.y - 2);
	moves[5] = vector2(coordinate.x - 2, coordinate.y - 1);
	moves[6] = vector2(coordinate.x - 2, coordinate.y + 1);
	moves[7] = vector2(coordinate.x - 1, coordinate.y + 2);

	//Creates a temporary vector version to return
	std::vector<vector2> tempList;
	for (int i = 0; i < 8; i++)
	{
		tempList.push_back(moves[i]);
	}
	return tempList;
}

//Checks if the tile is dead, and revives it if it is
void Tile::CheckAndReviveTile()
{
	if (!IsAlive())
	{
		health = MAX_HEALTH;
		falling = 2;
		velocity = 0;
		uClock = system->GenClock(); //generate a new clock for the timer
	}
}

//Returns true if health > 0
bool Tile::IsAlive()
{
	return health > 0;
}

//Set the y position of the tile
void Tile::SetYPosition(float newY)
{
	matrix4 m4Position = glm::translate(vector3(origPosition.x, newY, origPosition.z));
	entityMngr->SetModelMatrix(m4Position, entityMngr->GetEntityIndex(uniqueID));
}
