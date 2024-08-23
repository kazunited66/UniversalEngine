#include "Graphics/UGraphicsEngine.h"
#include "Graphics/UMesh.h"
#include "Graphics/UModel.h"
#include "Graphics/UShaderProgram.h"
#include "Math/USTransform.h"
#include "Graphics/Utexture.h"
#include "Graphics/USCamera.h"
#include "Graphics/USLight.h"
#include "Math/USCollision.h"
//External Libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

//collisoin cube vertices
const std::vector<USVertextData> colMeshVData = {
	// x   // y   // z 
{ {	-1.0f, -1.0f,  1.0f } }, // bl f
{ {	 1.0f, -1.0f,  1.0f } }, // br f
{ {  1.0f,  1.0f,  1.0f } }, // tr f
{ { -1.0f,  1.0f,  1.0f } }, // tl f
{ {	-1.0f, -1.0f, -1.0f } }, // bl b
{ {	 1.0f, -1.0f, -1.0f } }, // br b
{ {  1.0f,  1.0f, -1.0f } }, // tr b
{ { -1.0f,  1.0f, -1.0f } }, // tl b
};

//collisoin cube indices
const std::vector<uint32_t> colMeshIData = {
	0, 1, 1, 2, 2, 3, 3, 0, // front
	4, 5, 5, 6, 6, 7, 7, 4, // back
	0, 4, 1, 5, 2, 6, 3, 7  // sides
};


TWeak<USPointLight> m_pointLight;


bool UGraphicsEngine::InitEngine(SDL_Window* sdlWindow, const bool& vsync)
{
	if (sdlWindow == nullptr) {
		UDebug::Log("SDL window was null", LT_ERROR);
		UDebug::Log("Graphics engine failed to initialise", LT_ERROR);
		return false;
	}
	//create an open gl context 
	m_sdlGLContext = SDL_GL_CreateContext(sdlWindow);

	//test if the context failed
	if (m_sdlGLContext == nullptr) {
		UDebug::Log("SDL failed to create gl context: " + std::string(SDL_GetError()), LT_ERROR);
		UDebug::Log("Graphics engine failed to initialise", LT_ERROR);
		return false;
	}

	//make the current context active for the SDL window 
	//test if it failed 
	if (SDL_GL_MakeCurrent(sdlWindow, m_sdlGLContext) != 0) {
		UDebug::Log("SDL failed to make gl context current, graphics" + std::string(SDL_GetError()), LT_ERROR);
		UDebug::Log("Graphics engine failed to initialise", LT_ERROR);
		return false;
	}

	if (vsync) {
		//try enable adaptive vsync and test if it failed 
		if (SDL_GL_SetSwapInterval(-1) != 0) {
			//try to enable standard vsync and if it failed 
			if (SDL_GL_SetSwapInterval(1) != 0) {
				UDebug::Log("Graphics engine failed to initialise vsync: " + std::string(SDL_GetError()),
					LT_WARN);
				return false;
			}
		}
	}

	//Initilaise glew
	GLenum glewResult = glewInit();

	//test if glew failed 
	if (glewResult != GLEW_OK) {
		std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glewResult));
		UDebug::Log("Graphics engine failed to initialise GLEW: " + errorMsg);
	}

	glEnable(GL_DEPTH_TEST);


	//attempt to init shader and test if failed 
	m_shader = TMakeShared<UShaderProgram>();


	if (!m_shader->InitShader(
		"Shaders/SimpleShader/SimpleShader.vertex",
		"Shaders/SimpleShader/SimpleShader.frag"

	)) {
		UDebug::Log("Graphic engine failed to initialise due to shader failure");
		return false;
	}

	m_wireShader = TMakeShared<UShaderProgram>();


	if (!m_wireShader->InitShader(
		"Shaders/Wireframe/Wireframe.vertex",
		"Shaders/Wireframe/Wireframe.frag"

	)) {
		UDebug::Log("Graphic engine failed to initialise due to shader failure");
		return false;
	}

	//create the camera 
	m_camera = TMakeShared<USCamera>();
	m_camera->transform.position.z -= 50.0f;

	//create the texture object 
	TShared<UTexture> defaultTexture = TMakeShared<UTexture>();

	//add the texture to the mesh if it successfully created  
	if (!defaultTexture->LoadTexture("Default Grid", "Texture/T_DefaultGrid.png")) {
		UDebug::Log("Graphics Engine default texture failed to load ", LT_ERROR);

	}
	//init a default material for all models 
	m_defaultMaterials = TMakeShared <USMaterial>(); 

	//set the texture of the default material 
	m_defaultMaterials->m_baseColourMap = defaultTexture;
	//DEBUG




	//setting the materials to the 0 slot in the model 
	//m_model3.lock()->SetMaterialBySlot(1, matKnife);
	const auto& dirLight = CreateDirLight();

	if (const auto& lightRef = dirLight.lock()) {
		lightRef->colour = glm::vec3(1.0f, 1.0f, 0.0f);
		lightRef->direction = glm::vec3(0.0f, -1.0f, 0.0f);
		lightRef->ambient = glm::vec3(0.2f);
	}

	const auto& pointLight = CreatePointLight();
	if (const auto& lightRef = pointLight.lock()) {
		lightRef->colour = glm::vec3(0.0f, 0.0f, 1.0f);
		lightRef->position = glm::vec3(-25.0f,0.0f,0.0f);
		lightRef->intensity = 10.0f;
	}

	//log the success if the graphics engine init
	UDebug::Log("Successfully initialize graphics engine", LT_SUCCESS);

	return true;
}

void UGraphicsEngine::Render(SDL_Window* sdlWindow)
{


	//set a background colour 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//clear the back last frame	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//activate shader 
	m_shader->Activate();

	//set the world transformations based on the camera 
	//models will updae their positions in the mesh based on the transform 
	m_shader->SetWorldTransform(m_camera);

	//render custom graphics
	for (int i = m_models.size() - 1; i >=0; --i) {
		if (const auto& modelRef = m_models[i].lock()) {
			modelRef->Render(m_shader, m_lights);

		}
		else {
			//erase from the array if there is no reference 
			m_models.erase(m_models.begin() + i );

		}
		
	}

	//--------WIREFRAME SHADER

	if (m_collisions.size() > 0) {
		//activate shader 
		m_wireShader->Activate();

		//set the world transformations based on the camera 
		//models will updae their positions in the mesh based on the transform 
		m_wireShader->SetWorldTransform(m_camera);

		//render custom graphics
		for (int i = m_collisions.size() - 1; i >= 0; --i) {
			if (const auto& colRef = m_collisions[i].lock()) {
				//convert position of collision into transform 
				USTransform transform; 
				transform.position = colRef->box.position;
				transform.scale = colRef->box.halfSize; 

				m_wireShader->SetWireColour(colRef->debugcolour);
				colRef->debugMesh->WireRender(m_wireShader, transform);

			}
			else {
				//erase from the array if there is no reference 
				m_collisions.erase(m_collisions.begin() + i);

			}

		}
	}

	//presend the frame to the window 
	//swqaping the back buffer with thre front buffer
	SDL_GL_SwapWindow(sdlWindow);

}

TWeak<USPointLight> UGraphicsEngine::CreatePointLight()
{
	const auto& newLight = TMakeShared<USPointLight>();
	m_lights.push_back(newLight);
	return newLight;
}

TWeak<USDirLight> UGraphicsEngine::CreateDirLight()
{

	const auto& newLight = TMakeShared<USDirLight>();
	m_lights.push_back(newLight);
	return newLight;
}

TShared<UModel> UGraphicsEngine::ImportModel(const UString& path)
{
	const auto& newModel = TMakeShared<UModel>();
	newModel->ImportModel(path, m_defaultMaterials);
	m_models.push_back(newModel);

	return newModel;
}

TShared<USMaterial> UGraphicsEngine::CreateMaterial()
{

	return TMakeShared<USMaterial>();
}

void UGraphicsEngine::CreateCollisionMesh(const TWeak<USCollision>& col)
{
	if (const auto& colRef = col.lock()) {

		TShared<UMesh> newMesh = TMakeShared<UMesh>();
		//create a box of lines 
		newMesh->CreateMesh(colMeshVData, colMeshIData);

		//store a box of lines 
		colRef->debugMesh = newMesh;


		//add the mesh to be rendered 
		m_collisions.push_back(col);
	}

	
}

