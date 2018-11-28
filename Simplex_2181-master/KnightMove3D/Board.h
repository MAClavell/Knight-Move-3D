#pragma once
#include "Definitions.h"
#include "Tile.h"

#define NUM_ROWS 4
#define NUM_COLS 8

namespace Simplex
{
	class Board
	{
	private:
		EntityManager* entityMngr = nullptr;
		Tile* tiles[NUM_ROWS][NUM_COLS];

	public:
		Board();
		~Board();

		/*
		USAGE: Get the dimensions of the board
		ARGUMENTS: ---
		OUTPUT: A vector2 containing the dimensions
		*/
		vector2 GetBoardDimensions();

		/*
		USAGE: Get the knight position on a tile at the specified index
		ARGUMENTS: ---
		OUTPUT: A vector3 containing the knight position
		*/
		vector3 GetKnightPositionOnTile(int row, int col);
	};
}