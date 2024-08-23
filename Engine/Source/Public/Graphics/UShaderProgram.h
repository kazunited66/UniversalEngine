#pragma once

//System Libs 
#include <EngineTypes.h>
//External Libs 
#include<GLM/mat4x4.hpp>
class UTexture;
struct USCamera;
// enum to determain the type of shader 
enum UEShaderType : UUi8 {
	ST_VERTEXT = 0U,
	ST_FRAGMENT
};

struct USTransform;
struct USLight;
struct USMaterial;

class UShaderProgram {
public:
	UShaderProgram();
	~UShaderProgram();

	//create the shader using a vertex and fragment file 
	bool InitShader(const UString& vShaderPath, const UString& fShaderPath);

	//activate the shader to update 
	//you can't change value in a shader without activating it 
	void Activate();

	//set the transform of the model in the shader 
	void SetMeshTransform(const glm::mat4& matTransform);

	//set the transform of the model in the shader 
	void SetModelTransform(const USTransform& transform);

	//set the 3D coordinates for the model 
	void SetWorldTransform(const TShared<USCamera>& camera);

	//set a texture in shader based on the slot 
	void RunTexture(const TShared < UTexture>& texture, const UUi32& slot);

	void SetLights(const TArray<TShared<USLight>>& lights);

	//set the material in the shader 
	void SetMaterial(const TShared<USMaterial>& material);

	void SetWireColour(const glm::vec3 colour); 


private:
	//import a shader based on the shader type 
	bool ImportShaderByType(const UString& filePath, UEShaderType shaderType);

	//convert a file into a string 
	UString ConvertFileToString(const UString& filePath); \

		//link the shader to the GPU through open gl 
		bool LinkToGPU();

private:
	//store the file paths 
	UString m_filePath[2] = { "","" };
	//store the file ids
	UUi32 m_shaderIDs[2] = { 0, 0 };

	//store the id for the progarm 
	UUi32 m_programID;



};