#include "Game/GameObjects/MyObjects/Helmet.h"
#include "Graphics/UModel.h"
#include "Graphics/UTexture.h"
#include "Graphics/USMaterial.h"
Helmet::Helmet()
{
	

	/*if (const auto& modelRef = ImportModel("Models/MilitaryKnife/Military_Knife_low.fbx").lock()) {

		modelRef->m_offset.position.x = -20.0f;
		

	}*/

	
}

void Helmet::OnStart()
{
	if (const auto& modelRef = ImportModel("Models/Helmet3/Helmet3.fbx").lock()){
		//set scale 
		GetTransform().scale = glm::vec3(0.1f);

		//set position
		modelRef->m_offset.position.x = 0.0f;

		//creating a texture 
		TShared<UTexture> tex = TMakeShared<UTexture>();
		tex->LoadTexture("face texture base colour", "Models/Helmet3/Textures/facetexture_Base_color.png");

		//creating a second texture
		TShared<UTexture> tex2 = TMakeShared<UTexture>();
		tex2->LoadTexture("head base colour", "Models/Helmet3/Textures/Head_Base_color.png");


		//creating a specilar texture 
		TShared<UTexture> spectex = TMakeShared<UTexture>();
		spectex->LoadTexture("face texture spec colour", "Models/Helmet3/Textures/facetexture_Specular.png");

		//creating a specilar texture 
		TShared<UTexture> spectex2 = TMakeShared<UTexture>();
		spectex2->LoadTexture("head texture spec colour", "Models/Helmet3/Textures/Head_Specular.png");

		//creating a material 
		TShared<USMaterial> mat = TMakeShared<USMaterial>();
		TShared<USMaterial> mat2 = TMakeShared<USMaterial>();
		mat2->specularStrength = 0.1f;

		//assigning the texture to the base colour map for the  material 
		mat->m_baseColourMap = tex;
		mat->m_specularMap = spectex;
		mat2->m_baseColourMap = tex2;
		mat->m_specularMap = spectex2;
	
		modelRef->SetMaterialBySlot(0, mat2);
		modelRef->SetMaterialBySlot(1, mat);

		

	}
  	
    
	
	AddCollision({ GetTransform().position, glm::vec3(10.0f) });
}

void Helmet::OnTick(float deltaTime)
{
	UWorldObject::OnTick(deltaTime); 
	
	GetTransform().rotation.y += 10.0f * deltaTime; 
}

void Helmet::OnOverlap(const TShared<UWorldObject>& other, const TShared<USCollision>& otherCol)
{
	if (otherCol->type == UECollisionTypes::PLAYER) {
		Destroy();
	}
}

