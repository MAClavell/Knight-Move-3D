#include "Tile.h"
using namespace Simplex;

#define MAX_HEALTH 3
#define GRAVITY -9.8
#define FALL_TARGET -30
#define COLOR_STEP 0.15f

//Constructor
Tile::Tile(vector3 position, vector2 coord, vector3 aColor, SystemSingleton* a_system)
{
	//Initialize mesh manager
	meshMngr = MeshManager::GetInstance();

	origColor = aColor;
	color = aColor;

	//Set positon
	this->tilePosition = vector3(position);

	//Faling vars
	origY = position.y;
	yPos = origY;
	acceleration = 0;
	velocity = 0;

	//Set knight position
	position.y += 0.5f;
	knightPosition = vector3(position);

	//Set data
	health = MAX_HEALTH;
	coordinate.x = coord.x;
	coordinate.y = coord.y;
	falling = 0;
	system = a_system;
	fallTimer = 0;
	uClock = system->GenClock(); //generate a new clock for the timer

	//Set moves for knight
	moves[0] = vector2(coordinate.x + 1, coordinate.y + 2);
	moves[1] = vector2(coordinate.x + 2, coordinate.y + 1);
	moves[2] = vector2(coordinate.x + 2, coordinate.y - 1);
	moves[3] = vector2(coordinate.x + 1, coordinate.y - 2);
	moves[4] = vector2(coordinate.x - 1, coordinate.y - 2);
	moves[5] = vector2(coordinate.x - 2, coordinate.y - 1);
	moves[6] = vector2(coordinate.x - 2, coordinate.y + 1);
	moves[7] = vector2(coordinate.x - 1, coordinate.y + 2);

	//Set moves for rook
	rookMoves[0] = coordinate.y;
	rookMoves[1] = 3 - coordinate.x;
	rookMoves[2] = 7 - coordinate.y;
	rookMoves[3] = coordinate.x;
}

Tile::Tile()
{

}

//Destructor
Tile::~Tile()
{ }

//Display the cube
void Tile::Display()
{
	matrix4 m4Position = glm::translate(tilePosition) * glm::scale(vector3(1.0f, 1.0f, 1.0f));
	meshMngr->AddCubeToRenderList(m4Position, color, RENDER_SOLID | RENDER_WIRE);
}

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
	else if (falling == 2 && yPos >= origY)
	{
		yPos = origY;
		falling = 0;
	}

	//Set position
	tilePosition.y = yPos;

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
		system->GetDeltaTime(uClock); //generate a new clock for the timer
	}
	else if(origColor.x - COLOR_STEP <= 0)
	{
		color += COLOR_STEP;
	}
	else
	{
		color -= COLOR_STEP;
	}
}

//Return possible moves for knight
std::vector<vector2> Tile::GetMoves()
{
	std::vector<vector2> tempList;
	for (int i = 0; i < 8; i++)
	{
		tempList.push_back(moves[i]);
	}
	return tempList;
}

//Return directions for rook
std::vector<int> Tile::GetRookMoves()
{
	std::vector<int> tempList;
	for (int i = 0; i < 4; i++)
	{
		tempList.push_back(rookMoves[i]);
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
		color = origColor;
		uClock = system->GenClock(); //generate a new clock for the timer
	}
}

//Returns true if health > 0
bool Tile::IsAlive()
{
	return health > 0;
}

//Reset the tile
void Tile::Reset()
{
	health = MAX_HEALTH;
	falling = 2;
	velocity = 0;
	color = origColor;
}