#include <Graphics/UShaderProgram.h>
#include <Debug/UDebug.h>
#include <Math/USTransform.h>

//External Libs
#include <GLEW/glew.h>
#include <GLM/gtc/type_ptr.hpp>


//System Libs
#include <fstream>
#include <sstream>

#define UGET_GREW_ERROR reinterpret_cast<const char*>(glewGetErrorString(glGetError()));

UShaderProgram::UShaderProgram()
{
	m_programID = 0;
}

UShaderProgram::~UShaderProgram()
{
	UDebug::Log("Shader program " + std::to_string(m_programID) + "destroyed");
}

bool UShaderProgram::InitShader(const std::string& vShaderPath, const std::string& fShaderPath)
{
	//create the shader program in open gl 
	m_programID = glCreateProgram();

	//test if the create program failed 
	if (m_programID == 0) {
		const std::string errorMsg = UGET_GREW_ERROR;
		UDebug::Log("Shader failed to initialize, could't create program: " + errorMsg);
		return false; 
	}

	if (!ImportShaderByType(vShaderPath, ST_VERTEXT) || !ImportShaderByType(fShaderPath, ST_FRAGMENT)) {
		UDebug::Log("Shader failed to initialize, could't import shaders " );
		return false;
	}
	return LinkToGPU();
}

void UShaderProgram::Activate()
{
	glUseProgram(m_programID);
}

void UShaderProgram::SetModelTransform(const USTransform& transform)
{
	// translate(move)>rotate > scale (this allows us to rotate around the new location) 
	//initialise default matrix transform 
	glm::mat4  matrixT = glm::mat4(1.0f);

	//translate the matrix 
	matrixT = glm::translate(matrixT, transform.position);

	//rotate
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.x), glm::vec3(1.0f,0.0f,0.0f));
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.y), glm::vec3(0.0f,1.0f,0.0f));
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.z), glm::vec3(0.0f,0.0f,1.0f));

	//scale the matrix 
	matrixT = glm::scale(matrixT, transform.scale);

	//find the variable in the shader 
	//all uniform variables are given an ID by gl 
	const int varID = glGetUniformLocation(m_programID, "model");
	//update the value 
	glUniformMatrix4fv(
		varID, 1, GL_FALSE, glm::value_ptr(matrixT)
		);
}

bool UShaderProgram::ImportShaderByType(const std::string& filePath, UEShaderType shaderType)
{
	//convert the shader to a string 
	const std::string shaderStr = ConvertFileToString(filePath);

	//make sure there is a string path 
	if (shaderStr.empty()) {
		//erro that the string failed to import 
		UDebug::Log("Shader failed to import", LT_ERROR);
		return false;
	}

	//set and create an id for the shader based on the shader type 
	switch (shaderType) 
	{
	case ST_VERTEXT:
		m_shaderIDs[shaderType] = glCreateShader(GL_VERTEX_SHADER);
		break;
	case ST_FRAGMENT:
		m_shaderIDs[shaderType] = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	default:
		break;
	}

	//make sure there is a string path 
	if (m_shaderIDs[shaderType]==0) {
		//erro that the string failed to import 
		const std::string errorMsg = UGET_GREW_ERROR;
		UDebug::Log("Shader program could not assign shader id: " + errorMsg, LT_ERROR);
		return false;
	}

	//compile the shader onto the GPU 
	const char* shaderCStr = shaderStr.c_str();
	glShaderSource(m_shaderIDs[shaderType], 1, &shaderCStr, nullptr);
	glCompileShader(m_shaderIDs[shaderType]);

	//test of the compile worked 
	GLint success;
	glGetShaderiv(m_shaderIDs[shaderType], GL_COMPILE_STATUS, &success);

	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(m_shaderIDs[shaderType], 512, nullptr, infoLog);
		//log it 
		UDebug::Log("Shader compiled error:" + std::string(infoLog), LT_ERROR);
		return false;
	}

	//attach the shader to the program ID
	glAttachShader(m_programID, m_shaderIDs[shaderType]);

	return true;
}

std::string UShaderProgram::ConvertFileToString(const std::string& filePath)
{
	// convert the file path into an ifstream 
	std::ifstream shaderSource(filePath);

	//test if we can open the file 
	if (!shaderSource.is_open()) {
		UDebug::Log("Failed to open file: " + filePath, LT_ERROR);
		return{};
	}

	//initialize a sstream
	std::stringstream shaderStream;

	//convert the fstream into an sstream
	//basically turns the file into a string readable by our code 
	shaderStream << shaderSource.rdbuf();

	//close the file since we don't need it anymore 
	shaderSource.close();

	return shaderStream.str();
}

bool UShaderProgram::LinkToGPU()
{
	//link the program to the GPU
	glLinkProgram(m_programID);

	//test of the compile worked 
	GLint success;
	glGetProgramiv(m_programID, GL_COMPILE_STATUS, &success);

	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(m_programID, 512, nullptr, infoLog);
		//log it 
		UDebug::Log("Shader Link error:" + std::string(infoLog), LT_ERROR);
		return false;
	}

	UDebug::Log("Shader successfully initialised and linked at index :" + std::to_string(m_programID));

	return true;
}
