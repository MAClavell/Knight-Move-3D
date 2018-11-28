#include "Tile.h"
using namespace Simplex;

void Simplex::Tile::Init()
{
	health = 3;
}

Tile::Tile()
{
	Init();
}


Tile::~Tile()
{
}

void Simplex::Tile::Step()
{
	health--;
	if (health < 0) health = 0;
	//TO DO: change color/delete based on health
}

void Simplex::Tile::GetMoves()
{
	int tempMoves[8][2] = { {position[0] + 1, position[1] + 2}, {position[0] + 2, position[1] + 1}, 
			{position[0] + 2, position[1] - 1}, {position[0] + 1, position[1] - 2}, 
			{position[0] - 1, position[1] - 2}, {position[0] - 2, position[1] + 1}, 
			{position[0] - 2, position[1] + 1}, {position[0] - 1, position[1] + 2}};

	for (uint i = 0; i < 8; i++)
	{
		for (uint j = 0; j < 8; j++)
		{
			moves[i][j] = tempMoves[i][j];
		}
	}
}

bool Simplex::Tile::IsAlive()
{
	if (health > 0) return true;
	else return false;
}
