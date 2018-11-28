#include "Tile.h"
using namespace Simplex;

//Constructor
Tile::Tile(String fileName, String uniqueID, vector3 position, vector2 coord)
{
	//Initialize entity manager and entity
	entityMngr = EntityManager::GetInstance();
	entityMngr->AddEntity(fileName, uniqueID);

	//Set positon
	matrix4 m4Position = glm::translate(position);
	entityMngr->SetModelMatrix(m4Position, -1);

	//Set knight position
	position.x += 0.5f;
	position.y += 1;
	position.z += 0.5f;
	knightPosition = vector3(position);

	//Set data
	health = 3;
	coordinate.x = coord.x;
	coordinate.y = coord.y;
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

//Decrement health and change color
void Tile::Step()
{
	health--;
	if (health < 0) health = 0;
	//TODO: change color/delete based on health
}

//Calculate and set possible moves
void Tile::GetMoves()
{
	moves[0] = vector2(coordinate.x + 1, coordinate.y + 2);
	moves[1] = vector2(coordinate.x + 2, coordinate.y + 1);
	moves[2] = vector2(coordinate.x + 2, coordinate.y - 1);
	moves[3] = vector2(coordinate.x + 1, coordinate.y - 2);
	moves[4] = vector2(coordinate.x - 1, coordinate.y - 2);
	moves[5] = vector2(coordinate.x - 2, coordinate.y + 1);
	moves[6] = vector2(coordinate.x - 2, coordinate.y + 1);
	moves[7] = vector2(coordinate.x - 1, coordinate.y + 2);
}

//Returns true if health > 0
bool Tile::IsAlive()
{
	if (health > 0) return true;
	else return false;
}
