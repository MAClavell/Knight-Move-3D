#pragma once
#include "Definitions.h"

namespace Simplex
{

	class Tile
	{
	public:
		Tile();
		~Tile();
		
		//Public Member Variables
		uint health;
		int position[2]; //this tile's coordinates
		int moves[8][2]; //array of possible moves

		//Public Member Functions
		/*
		USAGE: Initialize member variables
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Init();
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

