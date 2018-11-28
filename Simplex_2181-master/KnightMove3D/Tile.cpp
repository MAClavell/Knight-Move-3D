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
	int tempMoves[8][2] = { {coordinate[0] + 1, coordinate[1] + 2}, {coordinate[0] + 2, coordinate[1] + 1}, 
			{coordinate[0] + 2, coordinate[1] - 1}, {coordinate[0] + 1, coordinate[1] - 2}, 
			{coordinate[0] - 1, coordinate[1] - 2}, {coordinate[0] - 2, coordinate[1] + 1}, 
			{coordinate[0] - 2, coordinate[1] + 1}, {coordinate[0] - 1, coordinate[1] + 2}};

	for (uint i = 0; i < 8; i++)
	{
		for (uint j = 0; j < 8; j++)
		{
			moves[i][j] = tempMoves[i][j];
		}
	}
}

//Returns true if health > 0
bool Tile::IsAlive()
{
	if (health > 0) return true;
	else return false;
}
