#pragma once
#include "Definitions.h"

namespace Simplex
{

	class Tile
	{
	private:
		MeshManager* meshMngr = nullptr;
		vector3 knightPosition; //the position the knight is placed on
		String uniqueID;
		vector3 origColor;
		vector3 color;

		//Falling vars
		short falling;
		int fallTimer; 
		float origY;
		vector3 tilePosition;
		float yPos;
		float acceleration;
		float velocity;

		//System vars
		SystemSingleton* system = nullptr;
		uint uClock;

	public:
		Tile(vector3 position, vector2 coord, vector3 aColor, SystemSingleton* a_system);
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
		USAGE: Display the cube
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Display();
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
		/*
		USAGE: Reset the tile
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Tile::Reset();
	};

}

