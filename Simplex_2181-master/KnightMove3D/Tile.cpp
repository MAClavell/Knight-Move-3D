#include "Tile.h"
using namespace Simplex;


Tile::Tile()
{
}


Tile::~Tile()
{
}

void Simplex::Tile::Init()
{
}

void Simplex::Tile::Step()
{
}

void Simplex::Tile::GetMoves()
{
	int tempMoves[8][2] = { {position[0] + 1, position[1] + 2}, {position[0] + 2, position[1] + 1}, 
			{position[0] + 2, position[1] - 1}, {position[0] + 1, position[1] - 2}, 
			{position[0] - 1, position[1] - 2}, {position[0] - 2, position[1] + 1}, 
			{position[0] - 2, position[1] + 1}, {position[0] - 1, position[1] + 2}};
}

bool Simplex::Tile::IsAlive()
{
	if (health > 0) return true;
	else return false;
}
