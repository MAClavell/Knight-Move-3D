#include "Heart.h"

using namespace Simplex;

Heart::Heart(String fileName, String uniqueID)
{
	entityMngr = EntityManager::GetInstance();
	entityMngr->AddEntity(fileName, uniqueID);
}


Heart::~Heart()
{
}

//Set the position of the knight
void Heart::SetPosition(vector3 newPos, vector2 index)
{
	gridIndex = index;
	matrix4 matrix = glm::translate(newPos) * glm::scale(vector3(0.25f, 0.25f, 0.25f));
	entityMngr->GetEntity(entityMngr->GetEntityIndex("Heart"))->SetModelMatrix(matrix);
}

//Set the position of the knight
vector2 Heart::GetIndex()
{
	return gridIndex;
}