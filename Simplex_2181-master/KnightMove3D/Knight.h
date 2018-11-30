#pragma once
#include "Definitions.h"
#include "Board.h"
#include "Tile.h"

namespace Simplex
{

	class Knight
	{
	private:
		EntityManager* entityMngr = nullptr;
		vector2 gridIndex;
		Board* board;
		std::vector<vector2> validMoves;
		Tile origin; //Tile that knight is currently jumping from
		Tile destination; //Tile that knight is currently jumping to

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
		/*
		USAGE: Called upon the end of a jump
		ARGUMENTS: Destination tile that has just been reached
		OUTPUT: ---
		*/
		void Land(Tile target);
	};

}