#pragma once
#include "EngineTypes.h"
#include "Graphics/UMesh.h"
#include "Math/USTransform.h"

class UTexture;
class UShaderProgram;

class UModel {

public:
	UModel() = default;
	~UModel() = default;
    // create a poly model and add a texture to it 
	void MakePoly(const TShared<UTexture>& texture);

	// create a cube model and add a texture to it 
	void MakeCube(const TShared<UTexture>& texture);

	//render all of the member within the model
	//transform of meshes will be based on the models transform 
	void Render(const TShared<UShaderProgram>& shader);

	//get the transform of the model 
	USTransform& GetTransform() { return m_transform; }
private: 
	//array of meshes
	TArray<TUnique<UMesh>> m_meshStack;

	//transform for the model in 3D space
	USTransform m_transform;
};