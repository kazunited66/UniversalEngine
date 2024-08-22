#include "Graphics/UGraphicsEngine.h"
#include "Graphics/UModel.h"
#include "Graphics/UShaderProgram.h"
#include "Math/USTransform.h"
#include "Graphics/Utexture.h"
#include "Graphics/USCamera.h"
#include "Graphics/USLight.h"

//External Libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

//test mesh for debug 

TWeak<UModel> m_model;
TWeak<UModel> m_model2;
TWeak<UModel> m_model3;
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

	//create the camera 
	m_camera = TMakeShared<USCamera>();
	m_camera->transform.position.z -= 5.0f;

	//create the texture object 
	TShared<UTexture> defaultTexture = TMakeShared<UTexture>();

	//add the texture to the mesh if it successfully created  
	if (!defaultTexture->LoadTexture("Default Grid", "Texture/T_DefaultGrid.png")) {
		UDebug::Log("Graphics Engine default texture failed to load ", LT_ERROR);

	}

	//DEBUG
	m_model = ImportModel("Models/Helmet3/Helmet3.fbx");
	m_model.lock()->GetTransform().scale = glm::vec3(0.1f);
	m_model.lock()->GetTransform().position = glm::vec3(0.0f, -1.5f, 0.0f);

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

	//setting the materials to the 0 slot in the model 
	m_model.lock()->SetMaterialBySlot(1, mat);
	m_model.lock()->SetMaterialBySlot(0, mat2);





	//import Military Knife
	m_model2 = ImportModel("Models/MilitaryKnife/Military_Knife_low.fbx");
	m_model2.lock()->GetTransform().scale = glm::vec3(0.1f);
	m_model2.lock()->GetTransform().position = glm::vec3(1.0f, 2.0f, 0.0f);

	//creating a texture 
	TShared<UTexture> texKnife = TMakeShared<UTexture>();
	texKnife->LoadTexture("bandle texture base colour", "Models/MilitaryKnife/textures/Blade_M_Base_color.png");

	//creating second texture 
	TShared<UTexture> texKnife2 = TMakeShared<UTexture>();
	texKnife2->LoadTexture("bandle texture base colour", "Models/MilitaryKnife/textures/Handle_M_Base_color.png");

	//creating a specilar texture 
	TShared<UTexture> spectexKnife = TMakeShared<UTexture>();
	spectexKnife->LoadTexture("bandle texture spec colour", "Models/Helmet3/textures/Blade_M_Specular.png");

	//creating a specilar texture 
	TShared<UTexture> spectexKnife2 = TMakeShared<UTexture>();
	spectexKnife2->LoadTexture("bandle texture spec colour", "Models/Helmet3/textures/Handle_M_Specular.png");

	//creating a material 
	TShared<USMaterial> matKnife = TMakeShared<USMaterial>();
	TShared<USMaterial> matKnife2 = TMakeShared<USMaterial>();
	matKnife2->specularStrength = 0.1f;

	//assigning the texture to the base colour map for the  material 
	matKnife->m_baseColourMap = texKnife;
	matKnife->m_specularMap = spectexKnife;
	matKnife2->m_baseColourMap = texKnife2;
	matKnife2->m_specularMap = spectexKnife2;

	//setting the materials to the 0 slot in the model 
	m_model2.lock()->SetMaterialBySlot(1, matKnife);
	m_model2.lock()->SetMaterialBySlot(0, matKnife2);
	


	//import Cambot
	m_model3=ImportModel("Models/Cambat/axe_finished.fbx"); 
	m_model3.lock()->GetTransform().scale = glm::vec3(0.001f);
	m_model3.lock()->GetTransform().position = glm::vec3(-2.0f, 1.0f, 0.0f);

	//creating a texture 
	TShared<UTexture> texCambat = TMakeShared<UTexture>();
	texCambat->LoadTexture("checher texture base colour", "Models/Cambat/textures/UV_Checker_BaseColor.png");


	//creating a specilar texture 
	TShared<UTexture> spectexCambat = TMakeShared<UTexture>();
	spectexCambat->LoadTexture("bandle texture spec colour", "Models/Cambat/textures/UV_Checker_Specular.png");


	//creating a material 
	TShared<USMaterial> matCambat = TMakeShared<USMaterial>();
	matCambat->specularStrength = 0.1f;

	//assigning the texture to the base colour map for the  material 
	matCambat->m_baseColourMap = texCambat;
	matCambat->m_specularMap = spectexCambat;

	//setting the materials to the 0 slot in the model 
	m_model3.lock()->SetMaterialBySlot(1, matKnife);
	




	const auto& dirLight = CreateDirLight();

	if (const auto& lightRef = dirLight.lock()) {
		lightRef->colour = glm::vec3(1.0f, 1.0f, 0.0f);
		lightRef->direction = glm::vec3(0.0f, -1.0f, 0.0f);
		lightRef->ambient = glm::vec3(0.1f);
	}

	const auto& pointLight = CreatePointLight();
	if (const auto& lightRef = pointLight.lock()) {
		lightRef->colour = glm::vec3(0.0f, 0.0f, 1.0f);
		lightRef->position = glm::vec3(5.0f,0.0f,0.0f);
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

	m_model.lock()->GetTransform().rotation.x += 0.01f;
	m_model.lock()->GetTransform().rotation.y += 0.01f;
	m_model.lock()->GetTransform().rotation.z += 0.01f;

	m_model2.lock()->GetTransform().rotation.x += 0.01f;
	m_model2.lock()->GetTransform().rotation.y += 0.01f;
	m_model2.lock()->GetTransform().rotation.z += 0.01f;

	m_model3.lock()->GetTransform().rotation.x += 0.01f;
	m_model3.lock()->GetTransform().rotation.y += 0.01f;
	m_model3.lock()->GetTransform().rotation.z += 0.01f;


	//activate shader 
	m_shader->Activate();

	//set the world transformations based on the camera 
	//models will updae their positions in the mesh based on the transform 
	m_shader->SetWorldTransform(m_camera);

	//render custom graphics
	for (const auto& modelRef : m_models) {
		modelRef->Render(m_shader, m_lights);
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

TWeak<UModel> UGraphicsEngine::ImportModel(const UString& path)
{
	const auto& newModel = TMakeShared<UModel>();
	newModel->ImportModel(path);
	m_models.push_back(newModel);

	return newModel;
}

TShared<USMaterial> UGraphicsEngine::CreateMaterial()
{

	return TMakeShared<USMaterial>();
}

