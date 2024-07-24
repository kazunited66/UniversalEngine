#include "Graphics/UGraphicsEngine.h"
#include "Graphics/UModel.h"
#include "Graphics/UShaderProgram.h"
#include "Math/USTransform.h"
#include "Graphics/Utexture.h"
#include "Graphics/USCamera.h"

//External Libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

//test mesh for debug 

TUnique<UModel> m_model;



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
	m_model = TMakeUnique<UModel>();
	m_model->ImportModel("Models/Lambo/Lambo.fbx");    
	m_model->GetTransform().scale = glm::vec3(0.01f);
	m_model->GetTransform().position.z += 100.0f;
	//m_model->GetTransform().position.x = 2.0f;

	//log the success if the graphics engine init
	UDebug::Log("Successfully initialize graphics engine", LT_SUCCESS);
	
	return true;
}

void UGraphicsEngine::Render(SDL_Window* sdlWindow)
{


	//set a background colour 
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	//clear the back last frame	
	glClear(GL_COLOR_BUFFER_BIT);

    m_model->GetTransform().rotation.x += 0.01f;
	m_model->GetTransform().rotation.y += 0.01f;
	m_model->GetTransform().rotation.z += 0.01f;

	//activate shader 
	m_shader->Activate();

	//set the world transformations based on the camera 
	//models will updae their positions in the mesh based on the transform 
	m_shader->SetWorldTransform(m_camera);

	//render custom graphics
	m_model->Render(m_shader);


	//presend the frame to the window 
	//swqaping the back buffer with thre front buffer
	SDL_GL_SwapWindow(sdlWindow);

}

