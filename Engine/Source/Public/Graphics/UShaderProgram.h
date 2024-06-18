#pragma once

//System Libs 
#include <string>
#include <cstdint>

// enum to determain the type of shader 
enum UEShaderType : uint8_t {
	ST_VERTEXT = 0U,
	ST_FRAGMENT
};

struct USTransform;


class UShaderProgram {
public: 
	UShaderProgram();
	~UShaderProgram();

	//create the shader using a vertex and fragment file 
	bool InitShader(const std::string& vShaderPath, const std::string& fShaderPath);

	//activate the shader to update 
	//you can't change value in a shader without activating it 
	void Activate();

	//set the transform of the model in the shader 
	void SetModelTransform(const USTransform& transform);



private: 
	//import a shader based on the shader type 
	bool ImportShaderByType(const std::string& filePath, UEShaderType shaderType);
	//convert a file into a string 
	std::string ConvertFileToString(const std::string& filePath);
	//link the shader to the GPU through open gl 
	bool LinkToGPU();

private:
	//store the file paths 
	std::string m_filePath[2] = { "","" };
	//store the file ids
	uint32_t m_shaderIDs[2] = { 0, 0 };

	//store the id for the progarm 
	uint32_t m_programID;



};