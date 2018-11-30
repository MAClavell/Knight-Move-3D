#include "Board.h"

using namespace Simplex;

//Constructor
Board::Board()
{
	//Define square size
	float squareSize = 1.0f;
	float xOffset = 4 * squareSize;
	float zOffset = 2 * squareSize;

	//Create the board
	vector3* knightPositions[NUM_ROWS][NUM_COLS];
	for (uint i = 0; i < NUM_ROWS; i++)
	{
		for (uint j = 0; j < NUM_COLS; j++)
		{
			//Create the tile
			Tile* tile = new Tile("Minecraft\\Cube.obj", "Grid(" + std::to_string(i) + "," + std::to_string(j) + ")",
							vector3(j * squareSize - xOffset, 0, i * squareSize - zOffset),
							vector2(i, j));
			tiles[i][j] = tile;
		}
	}

	//Create the heart
	heart = new Heart("Minecraft//Cube.obj", "Heart");
	SetRandHeartPosition();
}

//Desctructor
Board::~Board()
{
	for (uint i = 0; i < NUM_ROWS; i++)
	{
		for (uint j = 0; j < NUM_COLS; j++)
		{
			//Delete each tile
			SafeDelete(tiles[i][j]);
		}
	}

	SafeDelete(heart);
}

//Get the dimensions of the board
vector2 Board::GetBoardDimensions()
{
	return vector2(NUM_ROWS, NUM_COLS);
}

//Get the knight position on a tile at the specified index
vector3 Board::GetKnightPositionOnTile(int row, int col)
{
	return tiles[row][col]->GetKnightPosition();
}

//Get the tile at the specified coordinates
Tile Board::GetTile(vector2 coord)
{
	for (uint i = 0; i < 4; i++)
	{
		for (uint j = 0; j < 8; j++)
		{
			if (j == coord.x && i == coord.y)
			{
				return *tiles[i][j];
			}
		}
	}

}

//Set a new random position for the heart
void Board::SetRandHeartPosition()
{
	int row = -1;
	int col = -1;

	while (row < 0 || col < 0)
	{
		row = rand() % NUM_ROWS;
		col = rand() % NUM_COLS;
	}

	heart->SetPosition(GetKnightPositionOnTile(row, col), vector2(row, col));

}

//Set a new random position for the heart
Heart* Board::GetHeart()
{
	return heart;
}
