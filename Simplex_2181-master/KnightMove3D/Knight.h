#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <math.h>
#include "Definitions.h"
#include "Board.h"
#include "Tile.h"

namespace Simplex
{

	class Knight : public Entity
	{
	private:
		EntityManager* entityMngr = nullptr;
		SystemSingleton* system = nullptr;
		vector2 gridIndex;
		Board* board;
		std::vector<Tile> validMoves;
		Tile origin; //Tile that knight is currently jumping from
		Tile destination; //Tile that knight is currently jumping to
		float maxHeight = 2.0f;

		//LERP timing
		SystemSingleton* m_pSystem = nullptr; //Singleton of the system
		float fTimeBetweenStops = 5.0;
		float fPercentage;
		

	public:
		Knight(String fileName, String uniqueID, Board* brd, SystemSingleton* a_system);
		~Knight();

		/*
		USAGE: Set the knight's position
		ARGUMENTS: vector3 newPos -> the new position of the knight
		OUTPUT: ---
		*/
		void SetPosition(vector3 newPos);
		/*
		USAGE: moves the knight in a given direction
		ARGUMENTS: int dir -> direction to move the knight. 1 = up, 2 = left, 3 = down, 4 = right
		OUTPUT: ---
		*/
		void MoveKnight(int dir);
		/*
		USAGE: Interpolates knight from origin tile to destination tile
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Jump();
		/*
		USAGE: Called upon the end of a jump
		ARGUMENTS: Destination tile that has just been reached
		OUTPUT: ---
		*/
		void Land(Tile target);
	};

}