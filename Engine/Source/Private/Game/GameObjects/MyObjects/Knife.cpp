#include "Game/GameObjects/MyObjects/Knife.h"
#include "Graphics/UModel.h"
Knife::Knife()
{
	if (const auto& modelRef = ImportModel("Models/MilitaryKnife/Military_Knife_low.fbx").lock()) {

		modelRef->m_offset.position.x = 0.0f;
		GetTransform().scale = glm::vec3(0.1f);

	}
}

void Knife::OnTick(float deltaTime)
{
	UWorldObject::OnTick(deltaTime);

	GetTransform().rotation.y += 10.0f * deltaTime;
}
