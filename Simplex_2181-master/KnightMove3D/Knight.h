#pragma once

#include "Definitions.h"
#include "Board.h"
#include "Tile.h"

namespace Simplex
{

	class Knight
	{
	private:
		EntityManager* entityMngr = nullptr;
		vector2 gridIndex;
		Board* board;
		std::vector<Tile> validMoves;
		Tile origin; //Tile that knight is currently jumping from
		Tile destination; //Tile that knight is currently jumping to
		float maxHeight = 2.0f;
		int destinationIndex = 0;
		matrix4 rotation;
		String uniqueID; //uniqueID in the entity manager
		bool falling;

		//LERP timing
		SystemSingleton* system = nullptr; //Singleton of the system
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
		/*
		USAGE: sets fTimeBetweenStops
		ARGUMENTS: amount of seconds to have LERP take
		OUTPUT: ---
		*/
		void SetSpeed(float newTime);
		/*
		USAGE: changes destination of knight's current jump
		ARGUMENTS: true if going to the next move clockwise
		OUTPUT: ---
		*/
		void ChangeMove(bool clockwise);
		/*
		USAGE: sets rotation matrix to make knight face correct direction
		ARGUMENTS: Origin and destination tiles
		OUTPUT: new rotation matrix
		*/
		matrix4 SetRotation(Tile start, Tile end);
	};

}