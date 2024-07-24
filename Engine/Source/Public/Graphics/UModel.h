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

class UModel {

public:
	UModel() = default;
	~UModel() = default;
    // create a poly model and add a texture to it 
	void MakePoly(const TShared<UTexture>& texture);

	// create a cube model and add a texture to it 
	void MakeCube(const TShared<UTexture>& texture);

	//import a 3d model from fil
	//uses the ASSIMP import library, check docs to know which file types accpeted
	void ImportModel(const UString& filePath);

	//render all of the member within the model
	//transform of meshes will be based on the models transform 
	void Render(const TShared<UShaderProgram>& shader);

	//get the transform of the model 
	USTransform& GetTransform() { return m_transform; }
private:
	//find all of the meshes in a scene and concert them to a Umesh
	bool FindImportMeshes(const aiNode& node, const aiScene& scene, 
		const aiMatrix4x4& parentTransform );
private: 
	//array of meshes
	TArray<TUnique<UMesh>> m_meshStack;

	//transform for the model in 3D space
	USTransform m_transform;

	
};