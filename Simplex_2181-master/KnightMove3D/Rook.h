#pragma once

#include "Definitions.h"
#include "Board.h"
#include "Tile.h"

namespace Simplex
{

	class Rook
	{
	private:
		EntityManager* entityMngr = nullptr;
		vector2 gridIndex;
		Board* board;
		std::vector<Tile*> validMoves;
		Tile* origin; //Tile that Rook is currently jumping from
		Tile* destination; //Tile that Rook is currently jumping to
		float maxHeight = 2.0f;
		int destinationIndex = 0;
		matrix4 rotation;
		String uniqueID; //uniqueID in the entity manager
		uint entityIndex;
		int combo;
		bool frozen;

		//LERP timing
		SystemSingleton* system = nullptr; //Singleton of the system
		float fTimeBetweenStops = 5.0;
		float fPercentage;

		//Falling vars
		float yPos;
		short falling;
		float acceleration;
		float velocity;
		uint uClock;

	public:
		Rook(String fileName, String uniqueID, Board* brd, SystemSingleton* a_system);
		~Rook();

		/*
		USAGE: Set the Rook's position
		ARGUMENTS: vector3 newPos -> the new position of the Rook
		OUTPUT: ---
		*/
		void SetPosition(vector3 newPos);
		/*
		USAGE: Interpolates Rook from origin tile to destination tile
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Jump();
		/*
		USAGE: Makes the Rook fall downwards
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Fall();
		/*
		USAGE: Called upon the end of a jump
		ARGUMENTS: Tile* target -> Destination tile that has just been reached
		OUTPUT: ---
		*/
		void Land(Tile* target, bool stepTile = true);
		/*
		USAGE: Sets rotation matrix to make Rook face correct direction
		ARGUMENTS: Origin and destination tiles
		OUTPUT: new rotation matrix
		*/
		matrix4 SetRotation(Tile* start, Tile* end);
		/*
		USAGE: Reset the Rook
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Rook::Reset();
		/*
		USAGE: Check if the Rook is alive
		ARGUMENTS: ---
		OUTPUT: bool if the Rook is alive
		*/
		bool Rook::IsAlive();
		/*
		USAGE: Get a random space to move in
		ARGUMENTS: ---
		OUTPUT: bool if the Rook is alive
		*/
		Tile* Rook::ChooseMove(Tile* target);
		/*
		USAGE: Makes the rook stop moving
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Rook::Freeze(void);
		/*
		USAGE: Returns frozen
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		bool Rook::IsFrozen(void);
	};

}

