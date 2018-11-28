#pragma once
#include "Definitions.h"

namespace Simplex
{

	class Tile
	{
	private:
		EntityManager* entityMngr = nullptr;
		vector3 knightPosition; //the position the knight is placed on

	public:
		Tile(String fileName, String uniqueID, vector3 position, vector2 coord);
		~Tile();
		
		//Public Member Variables
		uint health;
		vector2 coordinate; //this tile's coordinates
		int moves[8][2]; //array of possible moves

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
		USAGE: Calculate and set possible moves
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void GetMoves(); 
		/*
		USAGE: Returns true if health > 0
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		bool IsAlive(); 

		//Knight Stuff, whoops
		//int origin[2]; //ij-coordinate of 
		//int destination[2];
	};

}

