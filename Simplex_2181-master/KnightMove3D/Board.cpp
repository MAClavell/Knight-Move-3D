#include "Board.h"

using namespace Simplex;

//Constructor
Board::Board(SystemSingleton* a_system)
{
	//Get singleton
	system = a_system;

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
	heart = new Heart("KnightMove3D//heart.obj", "Heart");
	SetRandHeartPosition();
	placingHeart = false;

	//Instantiate heart timer vars
	placeTimer = 0;//store the new timer
	uClock = system->GenClock(); //generate a new clock for the timer
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

//Update the board
void Board::Update()
{
	if (!placingHeart)
		return;

	//Step between each tile "regenerating" them
	placeTimer += system->GetDeltaTime(uClock); //get the delta time for that timer

	//Increment tile
	if (placeTimer > placeTimeStep)
	{
		//Increment the index of the heart
		placeIndex.x++;
		if (placeIndex.x == NUM_ROWS)
		{
			placeIndex.x = 0;
			placeIndex.y++;
		}

		//Check if we are past the max tile
		if (placeIndex.y >= NUM_COLS)
		{
			placingHeart = false;
			SetRandHeartPosition();
			return;
		}

		//Reset tile
		this->GetTile(placeIndex).CheckAndReviveTile();

		//Set heart position and restart the clock
		heart->SetPosition(GetKnightPositionOnTile(placeIndex), placeIndex); 
		placeTimer = system->GetDeltaTime(uClock);
	}
}

//Get the dimensions of the board
vector2 Board::GetBoardDimensions()
{
	return vector2(NUM_ROWS, NUM_COLS);
}

//Get the knight position on a tile at the specified index
vector3 Board::GetKnightPositionOnTile(vector2 index)
{
	return tiles[(int)index.x][(int)index.y]->GetKnightPosition();
}

//Get the tile at the specified coordinates
Tile Board::GetTile(vector2 coord)
{
	for (uint i = 0; i < NUM_ROWS; i++)
	{
		for (uint j = 0; j < NUM_COLS; j++)
		{
			if (i == coord.x && j == coord.y)
			{
				if(tiles[i][j])
					return *tiles[i][j];
			}
		}
	}

}

//Get if a position is the heart's index
void Board::HandleIfOnHeart(vector2 gridIndex)
{
	if (gridIndex != heart->GetIndex())
		return;

	placeTimer = system->GetDeltaTime(uClock);
	placingHeart = true;
	placeIndex = vector2(0, 0);
	heart->SetPosition(GetKnightPositionOnTile(placeIndex), placeIndex);
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

	vector2 pos(row, col);
	heart->SetPosition(GetKnightPositionOnTile(pos), pos);

}

//Set a new random position for the heart
Heart* Board::GetHeart()
{
	return heart;
}

bool Board::IsPlacingHeart()
{
	return placingHeart;
}
