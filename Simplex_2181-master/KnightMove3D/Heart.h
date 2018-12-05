#pragma once
#include "Definitions.h"

namespace Simplex
{
	class Heart
	{
	private:
		EntityManager * entityMngr = nullptr;
		vector2 gridIndex;
		String uniqueID;

	public:
		Heart(String fileName, String uniqueID);
		~Heart();

		/*
		USAGE: Set the heart's position
		ARGUMENTS: 
		-			vector3 newPos -> the new position of the heart
		-			vector2 index -> the grid index of the heart
		OUTPUT: ---
		*/
		void SetPosition(vector3 newPos, vector2 index);

		/*
		USAGE: Get the heart's position
		ARGUMENTS: vector3 newPos -> the position of the heart
		OUTPUT: ---
		*/
		vector2 GetIndex();
	};
}

