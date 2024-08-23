#pragma once
#include "EngineTypes.h"
#include "Graphics/UMesh.h"
#include "Math/USTransform.h"
//Eternal Libs 
#include <ASSIMP/matrix4x4.h>

class UTexture;
class UShaderProgram;
struct aiScene;
struct aiNode;
struct USLight;
struct USMaterial;

class UModel {

public:
	UModel() = default;
	~UModel() { UDebug::Log("Model destroyed"); }


	//import a 3d model from fil
	//uses the ASSIMP import library, check docs to know which file types accpeted
	void ImportModel(const UString& filePath, const TShared<USMaterial>& defaultMaterial);

	//render all of the member within the model
	//transform of meshes will be based on the models transform 
	void Render(const TShared<UShaderProgram>& shader, const TArray<TShared<USLight>>& lights);

	//get the transform of the model 
	USTransform& GetTransform() { return m_transform; }

	void SetMaterialBySlot(unsigned int slot, TShared<USMaterial>& material);

	//transform offset 
	USTransform m_offset;
private:
	//find all of the meshes in a scene and concert them to a Umesh
	bool FindImportMeshes(const aiNode& node, const aiScene& scene,
		const aiMatrix4x4& parentTransform, UUi32* meshesCreated);


private:
	//array of meshes
	TArray<TUnique<UMesh>> m_meshStack;

	//array of materials model 
	TArray<TShared<USMaterial>> m_materialsStack;


	//transform for the model in 3D space
	USTransform m_transform;


};