#include "Game/GameObjects/MyObjects/Helmet.h"
#include "Graphics/UModel.h"
Helmet::Helmet()
{
	if (const auto& modelRef = ImportModel("Models/Helmet3/Helmet3.fbx").lock()) {

		modelRef->m_offset.position.x = 0.0f; 
		GetTransform().scale = glm::vec3(0.1f);

	}

	/*if (const auto& modelRef = ImportModel("Models/MilitaryKnife/Military_Knife_low.fbx").lock()) {

		modelRef->m_offset.position.x = -20.0f;
		

	}*/

	
}

void Helmet::OnTick(float deltaTime)
{
	UWorldObject::OnTick(deltaTime); 
	
	GetTransform().rotation.y += 10.0f * deltaTime; 
}

