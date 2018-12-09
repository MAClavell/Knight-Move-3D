#pragma once
#include "Definitions.h"
#include "Tile.h"
#include "Heart.h"

#define NUM_ROWS 4
#define NUM_COLS 8
#define PLACE_TIME 3

namespace Simplex
{
	class Board
	{
	private:
		EntityManager* entityMngr = nullptr;
		Tile* tiles[NUM_ROWS][NUM_COLS];
		Heart* heart = nullptr;
		int score = 0;

		//System vars
		SystemSingleton* system = nullptr; //Singleton of the system
		uint uClock;

		//Heart placement vars
		bool placingHeart = false;
		vector2 placeIndex = vector2(0, 0);
		float placeTimeStep = ((float)PLACE_TIME) / (NUM_ROWS * NUM_COLS);
		float placeTimer = 0;

	public:
		Board(SystemSingleton* a_system);
		~Board();
		
		/*
		USAGE: Update the board
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Update();

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
		vector3 GetKnightPositionOnTile(vector2 index);

		/*
		USAGE: Get the tile at the specified coordinates
		ARGUMENTS: Coordinates to search for
		OUTPUT: The tile at the specified coordinates
		*/
		Tile* GetTile(vector2 coord);

		/*
		USAGE: Get if a position is the heart's index, then apply the effect
		ARGUMENTS: The index to check
		OUTPUT: ---
		*/
		void HandleIfOnHeart(vector2 gridIndex);

		/*
		USAGE: Get the heart object
		ARGUMENTS: 
		OUTPUT: The heart object
		*/
		Heart* GetHeart();

		/*
		USAGE: Check if the board is placing the heart
		ARGUMENTS: ---
		OUTPUT: If the heart is being placed
		*/
		bool IsPlacingHeart();

		/*
		USAGE: Get the score variable
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		int GetScore();

	private:
		/*
		USAGE: Set a random position for the heart
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void SetRandHeartPosition();
	};
}