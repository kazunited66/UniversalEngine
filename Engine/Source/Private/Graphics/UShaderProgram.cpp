#include <Graphics/UShaderProgram.h>
#include <Debug/UDebug.h>
#include <Math/USTransform.h>
#include <Graphics/UTexture.h>
#include <Graphics/USCamera.h>
#include "Graphics/USLight.h"
#include "Graphics/USMaterial.h"

//External Libs
#include <GLEW/glew.h>
#include <GLM/gtc/type_ptr.hpp>


//System Libs
#include <fstream>
#include <sstream>

#define UGET_GREW_ERROR reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
//const value for light amounts
const UUi32 maxDirLights = 2;
const UUi32 maxPoinLight = 20;

UShaderProgram::UShaderProgram()
{
	m_programID = 0;
}

UShaderProgram::~UShaderProgram()
{
	UDebug::Log("Shader program " + std::to_string(m_programID) + "destroyed");
}

bool UShaderProgram::InitShader(const UString& vShaderPath, const UString& fShaderPath)
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
		UDebug::Log("Shader failed to initialize, could't import shaders ");
		return false;
	}
	return LinkToGPU();
}

void UShaderProgram::Activate()
{
	glUseProgram(m_programID);
}

void UShaderProgram::SetMeshTransform(const glm::mat4& matTransform)
{
	//find the variable in the shader 
	//all uniform variables are given an ID by gl 
	const int varID = glGetUniformLocation(m_programID, "mesh");
	//update the value 
	glUniformMatrix4fv(
		varID, 1, GL_FALSE, glm::value_ptr(matTransform)
	);
}

void UShaderProgram::SetModelTransform(const USTransform& transform)
{
	//translate(move)>rotate > scale (this allows us to rotate around the new location) 
	//initialise default matrix transform 

	glm::mat4  matrixT = glm::mat4(1.0f);

	//translate the matrix 
	matrixT = glm::translate(matrixT, transform.position);

	//rotate
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

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

void UShaderProgram::RunTexture(const TShared<UTexture>& texture, const UUi32& slot)
{

}

void UShaderProgram::SetLights(const TArray<TShared<USLight>>& lights)
{
	UUi32 dirLights = 0;
	UUi32 pointLights = 0;
	int varID = 0;
	//add a dir light and use a index 
	UString lightIndexStr = "";


	//loop thorugh all of lights and add the to the shader 
	for (UUi32 i = 0; i < lights.size(); ++i) {

		if (const auto& lightRef = std::dynamic_pointer_cast<USDirLight>(lights[i])) {
			//ignore the light if we have alredy maxed out 
			if (dirLights >= maxDirLights) {
				continue;
			}

			//add a dir light and use a index 
			lightIndexStr = "dirLights[" + std::to_string(dirLights) + "]";

			//_____COLOUR 
			//get the colour variable from the dir light struct in the shader 
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".colour").c_str());

			//change the colour 
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->colour));

			//_____AMBIENT
			//get the ambient variable from the fir light in the shader 
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".ambient").c_str());
			//change theabmient ambient 
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->ambient));

			//_____DIRECTION
			//get the direction variable from the fir light in the shader 
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".direction").c_str());
			//change theabmient direction 
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->direction));


			//_____INTENSITY
			//get the colour variable from the fir light in the shader 
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".intensity").c_str());

			//change theabmient intensity 
			glUniform1f(varID, lightRef->intensity);


			//increase the dirLights count 
			++dirLights;
			continue;
		}

		if (const auto& lightRef = std::dynamic_pointer_cast<USPointLight>(lights[i])) {
			//ensure only max light 
			if (pointLights >= maxPoinLight) {
				continue;
			}
			//add a dir light and use a index 
			lightIndexStr = "pointLights[" + std::to_string(pointLights) + "]";

			//_____COLOUR 
			//get the colour variable from the dir light struct in the shader 
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".colour").c_str());

			//change the colour 
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->colour));

			//_____POSITION
			//get the shader variable id
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".position").c_str());

			//update the shader value 
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->position));

			//_____INTENSITY
			//get the colour variable from the fir light in the shader 
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".intensity").c_str());

			//change theabmient intensity 
			glUniform1f(varID, lightRef->intensity);

			//_____LINEAR ATTENUATION
			//get the variable id
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".linear").c_str());

			//change the vale 
			glUniform1f(varID, lightRef->linear);

			//_____QUADRATIC ATTENUATION
			//get the variable id
			varID = glGetUniformLocation(m_programID,
				(lightIndexStr + ".quadratic").c_str());

			//change the vale 
			glUniform1f(varID, lightRef->quadratic);

			//increment the point light index 
			++pointLights;
		}
	}


}

void UShaderProgram::SetMaterial(const TShared<USMaterial>& material)
{
	if (material == nullptr)
		return;


	//the id for the variable in the shader 
	int varID = 0;


	//______BASE COLOUR 
	if (material->m_baseColourMap) {
		// bind the texture to the 0 index 
		material->m_baseColourMap->BindTexture(0);

		//get the base clour map id 
		varID = glGetUniformLocation(m_programID, "material.baseColourMap");

		//update the shader 
		glUniform1i(varID, 0);
	}


	//______SPECULAR MAP 
	if (material->m_specularMap) {
		// bind the texture to the 1 index 
		material->m_specularMap->BindTexture(1);

		//get the base clour map id 
		varID = glGetUniformLocation(m_programID, "material.specularMap");

		//update the shader 
		glUniform1f(varID, 1);

	}


	//______SHININESS
	varID = glGetUniformLocation(m_programID, "material.shininess");

	//update the shader 
	glUniform1f(varID, material->shininess);

	//______SPECULAR STRENGH
	varID = glGetUniformLocation(m_programID, "material.specularStrength");

	//update the shader 
	glUniform1f(varID, material->specularStrength);


}

void UShaderProgram::SetWorldTransform(const TShared<USCamera>& camera)
{
	//initialise a matrics 
	glm::mat4 matrixT = glm::mat4(1.0f);

	//handle the view matrix

	//handle the view matrix 
	//matrixT = glm::translate(matrixT, camera->transform.position);
	//translate & rotate the matrics based on the camera's forwars and up vector 
	matrixT = glm::lookAt(
		camera->transform.position,
		camera->transform.position + camera->transform.Forward(),
		camera->transform.Up()
	);


	//find the variable in the shader and update it 
	int varID = glGetUniformLocation(m_programID, "view");

	//update the value 
	glUniformMatrix4fv(
		varID, 1, GL_FALSE, glm::value_ptr(matrixT)
	);

	//handle the projection matrix

	//set the projection to a perspective view 
	matrixT = glm::perspective(
		glm::radians(camera->fov),//the zoom of your camera 
		camera->aspectRation,//how wide the view is 
		camera->nearClip,//how close you can see 3D models
		camera->farClip  //how far you can see 3D models - all other models will not render 
	);

	//find the variable in the shader for the projection matrix 
	varID = glGetUniformLocation(m_programID, "projection");

	//update the projection matrix in the shader 
	glUniformMatrix4fv(
		varID, 1, GL_FALSE, glm::value_ptr(matrixT)
	);

}

bool UShaderProgram::ImportShaderByType(const UString& filePath, UEShaderType shaderType)
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
	if (m_shaderIDs[shaderType] == 0) {
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

UString UShaderProgram::ConvertFileToString(const UString& filePath)
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
