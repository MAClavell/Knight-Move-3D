#pragma once
#include "Definitions.h"

namespace Simplex
{

	class Tile
	{
	public:
		Tile();
		~Tile();
		void Init();
		void Step(); //Decrement health and change color
		void GetMoves(); //Calculate and set possible moves
		bool IsAlive(); //Returns true if health > 0
		uint health;
		int position[2]; //this tile's coordinates
		int moves[8][2]; //array of possible moves

		//Knight Stuff, whoops
		//int origin[2]; //ij-coordinate of 
		//int destination[2];
	};

}

