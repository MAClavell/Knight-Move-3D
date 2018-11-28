#pragma once
#include "Definitions.h"
#include "Board.h"

namespace Simplex
{

	class Knight : public Entity
	{
	private:
		EntityManager* entityMngr = nullptr;
		vector2 gridIndex;
		Board* board;
		//vector of possible moves
		//origin coords
		//destination coords

	public:
		Knight(String fileName, String uniqueID, Board* brd);
		~Knight();

		/*
		USAGE: Set the knight's position
		ARGUMENTS: vector3 newPos -> the new position of the knight
		OUTPUT: ---
		*/
		void SetPosition(vector3 newPos);
		/*
		USAGE: moves the knight in a given direction
		ARGUMENTS: int dir -> direction to move the knight. 1 = up, 2 = left, 3 = down, 4 = right
		OUTPUT: ---
		*/
		void MoveKnight(int dir);
		/*
		USAGE: Interpolates knight from origin tile to destination tile
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Jump();
	};

}