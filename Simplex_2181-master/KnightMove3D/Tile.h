#pragma once
#include "Definitions.h"

namespace Simplex
{

	class Tile
	{
	private:
		EntityManager* entityMngr = nullptr;
		vector3 knightPosition; //the position the knight is placed on
		String uniqueID;

		//Falling vars
		short falling;
		int fallTimer; 
		vector3 origPosition;
		float yPos;
		float acceleration;
		float velocity;

		//System vars
		SystemSingleton* system = nullptr;
		uint uClock;

	public:
		Tile(String fileName, String uniqueID, vector3 position, vector2 coord, SystemSingleton* a_system);
		Tile();
		~Tile();
		
		//Public Member Variables
		uint health;
		vector2 coordinate; //this tile's coordinates
		vector2 moves[8]; //array of possible moves

		//Public Member Functions
		/*
		USAGE: Gets the position that the knight should be on when on this tile
		ARGUMENTS: ---
		OUTPUT: A vector3 of that position
		*/
		vector3 GetKnightPosition();
		/*
		USAGE: Decrement health and change color
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Step(); 
		/*
		USAGE: Calculate and set legal coordinates to move to from this tile
		ARGUMENTS: ---
		OUTPUT: list of moves
		*/
		std::vector<vector2> GetMoves(); 
		/*
		USAGE: Checks if the tile is dead, and revives it if it is
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void CheckAndReviveTile();
		/*
		USAGE: Update the tile every frame
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Update();
		/*
		USAGE: Returns true if health > 0
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		bool IsAlive(); 

	private:
		/*
		USAGE: Set the Y position of the tile
		ARGUMENTS: float newY -> the new y position
		OUTPUT: ---
		*/
		void SetYPosition(float newY);
	};

}

