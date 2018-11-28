#pragma once
#include "Definitions.h"

namespace Simplex
{

	class Knight : public Entity
	{
	private:
		EntityManager* entityMngr = nullptr;
		vector3* grid[4][8];
		vector2* gridIndices;

	public:
		Knight(Simplex::String fileName, Simplex::String uniqueID, vector3* gridPositions[4][8]);
		~Knight();

		/*
		USAGE: moves the knight in a given direction
		ARGUMENTS: int dir -> direction to move the knight. 1 = up, 2 = left, 3 = down, 4 = right
		OUTPUT: ---
		*/
		void MoveKnight(int dir);
	};

}