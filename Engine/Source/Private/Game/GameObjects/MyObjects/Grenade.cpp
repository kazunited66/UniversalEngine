#include "Game/GameObjects/MyObjects/Grenade.h"
#include "Graphics/UModel.h"
#include "Graphics/UTexture.h"
#include "Graphics/USMaterial.h"

Grenade::Grenade()
{
}

void Grenade::OnStart()
{
	if (const auto& modelRef = ImportModel("Models/Grenade/GrenadeF1.fbx").lock()) {
		//set scale 
		//GetTransform().scale = glm::vec3(0.1f);


		//creating a texture 
		TShared<UTexture> tex = TMakeShared<UTexture>();
		tex->LoadTexture("grenade base colour", "Models/Grenade/textures/GrenadeF1_Base_color.png");


		//creating a specilar texture 
		TShared<UTexture> spectex = TMakeShared<UTexture>();
		spectex->LoadTexture("grenade spec colour", "Models/Grenade/textures/GrenadeF1_Specular.png");

		//creating a material 
		TShared<USMaterial> mat = TMakeShared<USMaterial>();
		mat->specularStrength = 0.1f;

		//assigning the texture to the base colour map for the  material 
		mat->m_baseColourMap = tex;
		mat->m_specularMap = spectex;
		modelRef->SetMaterialBySlot(0, mat);
	}

	AddCollision({ GetTransform().position, glm::vec3(10.0, 10.0, 10.0f) }, true);
}

void Grenade::OnTick(float deltaTime)
{
	UWorldObject::OnTick(deltaTime);

	GetTransform().rotation.y += 10.0f * deltaTime;
}

void Grenade::OnOverlap(const TShared<UWorldObject>& other, const TShared<USCollision>& otherCol)
{
	if (otherCol->type == UECollisionTypes::PLAYER) {
		Destroy();
	}
}
