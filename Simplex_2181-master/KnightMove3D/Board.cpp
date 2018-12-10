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
			//Decide color
			vector3 color = C_BLUE_CORNFLOWER;
			if ((i + j) % 2 == 0)
				color = vector3(0.6f, 0.6f, 0.6f);

			//Create the tile
			Tile* tile = new Tile(vector3((j * squareSize - xOffset) + 0.5f, 0 + 0.5f, (i * squareSize - zOffset) + 0.5f),
							vector2(i, j), color, system);
			tiles[i][j] = tile;
		}
	}

	//Create the heart
	heart = new Heart("KnightMove3D\\heart.obj", "Heart");
	SetRandHeartPosition();
	placingHeart = false;

	//Create the reticule
	/*EntityManager::GetInstance()->AddEntity("Minecraft\\Cube.obj", "Reticule");
	EntityManager::GetInstance()->GetEntityIndex("Reticule");*/

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
	//Update each tile
	for (int i = 0; i < NUM_ROWS; i++)
	{
		for (int j = 0; j < NUM_COLS; j++)
		{
			tiles[i][j]->Update();
		}
	}

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
		this->GetTile(placeIndex)->CheckAndReviveTile();

		//Set heart position and restart the clock
		heart->SetPosition(GetKnightPositionOnTile(placeIndex), placeIndex); 
		placeTimer = system->GetDeltaTime(uClock);
	}
}

//Display all tiles
void Board::Display()
{
	for (uint i = 0; i < NUM_ROWS; i++)
	{
		for (uint j = 0; j < NUM_COLS; j++)
		{
			//Delete each tile
			tiles[i][j]->Display();
		}
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
Tile* Board::GetTile(vector2 coord)
{
	for (uint i = 0; i < NUM_ROWS; i++)
	{
		for (uint j = 0; j < NUM_COLS; j++)
		{
			if (i == coord.x && j == coord.y)
			{
				return tiles[i][j];
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
	this->GetTile(placeIndex)->CheckAndReviveTile();
	heart->SetPosition(GetKnightPositionOnTile(placeIndex), placeIndex);
	score++;
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

//Check if the board is placing the heart
bool Board::IsPlacingHeart()
{
	return placingHeart;
}

//Get the score variable
int Board::GetScore()
{
	return score;
}
