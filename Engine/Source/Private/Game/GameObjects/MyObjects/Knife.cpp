#include "Game/GameObjects/MyObjects/Knife.h"
#include "Graphics/UModel.h"
#include "Graphics/UTexture.h"
#include "Graphics/USMaterial.h"
Knife::Knife()
{
	
}

void Knife::OnStart()
{
	if (const auto& modelRef = ImportModel("Models/MilitaryKnife/Military_Knife_low.fbx").lock()) {

		//set scale 
		GetTransform().scale = glm::vec3(0.1f);

		//set posiotion
		modelRef->m_offset.position.x = 0.0f;

		//creating a texture 
		TShared<UTexture> texKnife = TMakeShared<UTexture>();
		texKnife->LoadTexture("bandle texture base colour", "Models/MilitaryKnife/textures/Blade_M_Base_color.png");

		//creating second texture 
		TShared<UTexture> texKnife2 = TMakeShared<UTexture>();
		texKnife2->LoadTexture("bandle texture base colour", "Models/MilitaryKnife/textures/Handle_M_Base_color.png");

		//creating a specilar texture 
		TShared<UTexture> spectexKnife = TMakeShared<UTexture>();
		spectexKnife->LoadTexture("bandle texture spec colour", "Models/Helmet3/textures/Blade_M_Specular.png");

		//creating a specilar texture 
		TShared<UTexture> spectexKnife2 = TMakeShared<UTexture>();
		spectexKnife2->LoadTexture("bandle texture spec colour", "Models/Helmet3/textures/Handle_M_Specular.png");

		//creating a material 
		TShared<USMaterial> matKnife = TMakeShared<USMaterial>();
		TShared<USMaterial> matKnife2 = TMakeShared<USMaterial>();
		matKnife2->specularStrength = 0.1f;

		//assigning the texture to the base colour map for the  material 
		matKnife->m_baseColourMap = texKnife;
		matKnife->m_specularMap = spectexKnife;
		matKnife2->m_baseColourMap = texKnife2;
		matKnife2->m_specularMap = spectexKnife2;

		//setting the materials to the 0 slot in the model 
		modelRef->SetMaterialBySlot(1, matKnife);
		modelRef->SetMaterialBySlot(0, matKnife2);

		
		
	}
	

	AddCollision({ GetTransform().position, glm::vec3(1.0f) });
}

void Knife::OnTick(float deltaTime)
{
	UWorldObject::OnTick(deltaTime);

	GetTransform().rotation.y += 10.0f * deltaTime;
}

void Knife::OnOverlap(const TShared<UWorldObject>& other, const TShared<USCollision>& otherCol)
{
	if (otherCol->type == UECollisionTypes::PLAYER) {
		Destroy();
	}
}
