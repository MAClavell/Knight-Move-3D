#include "Heart.h"

#define Y_INC 0.05f;

using namespace Simplex;

Heart::Heart(String fileName, String uniqueID)
{
	entityMngr = EntityManager::GetInstance();
	entityMngr->AddEntity(fileName, uniqueID);
	entityMngr->GetRigidBody(uniqueID)->SetVisibleOBB(true);

	this->uniqueID = uniqueID;
}


Heart::~Heart()
{
}

//Set the position of the knight
void Heart::SetPosition(vector3 newPos, vector2 index)
{
	gridIndex = index;
	newPos.y += Y_INC;
	matrix4 matrix = glm::translate(newPos) * glm::scale(vector3(0.5f, 0.5f, 0.5f));
	entityMngr->SetModelMatrix(matrix, uniqueID);
}

//Set the position of the knight
vector2 Heart::GetIndex()
{
	return gridIndex;
}