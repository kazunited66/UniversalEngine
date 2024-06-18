#include "Graphics/UGraphicsEngine.h"
#include "Debug/UDebug.h"
#include "Graphics/UMesh.h"
#include "Graphics/UShaderProgram.h"
#include "Math/USTransform.h"
//External Libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

std::vector<USVertextData> vertexData;
std::vector<uint32_t> indexData;

std::vector<USVertextData> vertexData2;
std::vector<uint32_t> indexData2;

std::vector<USVertextData> vertexData3;
std::vector<uint32_t> indexData3;


//test mesh for debug 
std::unique_ptr<UMesh> m_mesh;
std::unique_ptr<UMesh> m_mesh2;
std::unique_ptr<UMesh> m_mesh3;

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
	m_shader = std::make_shared<UShaderProgram>();
	if (!m_shader->InitShader(
		"Shaders/SimpleShader/SimpleShader.vertex",
		"Shaders/SimpleShader/SimpleShader.frag"

		)) {
		UDebug::Log("Graphic engine failed to initialise due to shader failure");
		return false;
	}

	//log the success if the graphics engine init
	UDebug::Log("Successfully initialize graphics engine", LT_SUCCESS);

	//crete the debug mesh
	m_mesh = std::make_unique<UMesh>();

	//crete the debug mesh
	m_mesh2 = std::make_unique<UMesh>();

	//crete the debug mesh
	m_mesh3 = std::make_unique<UMesh>();

	//create square
	vertexData.resize(4);
	//vertex1
	vertexData[0].m_position[0] = -0.5f;//x
	vertexData[0].m_position[1] = 0.5f;//y
	vertexData[0].m_position[2] = 0.0f;//z
	//colour for v1
	vertexData[0].m_colour[0] = 1.0f;//r
	vertexData[0].m_colour[1] = 1.0f;//g
	vertexData[0].m_colour[2] = 0.0f;//b
	//vertex2
	vertexData[1].m_position[0] = -0.5f;
	vertexData[1].m_position[1] = -0.5f;
	vertexData[1].m_position[2] = 0.0f;
	//colour for v2
	vertexData[1].m_colour[0] = 1.0f;//r
	vertexData[1].m_colour[1] = 1.0f;//g
	vertexData[1].m_colour[2] = 0.0f;//b
	//vertex3
	vertexData[2].m_position[0] = 0.5f;
	vertexData[2].m_position[1] = -0.5f;
	vertexData[2].m_position[2] = 0.0f;
	//colour for v3
	vertexData[2].m_colour[0] = 1.0f;//r
	vertexData[2].m_colour[1] = 1.0f;//g
	vertexData[2].m_colour[2] = 0.0f;//b
	//vertex4
	vertexData[3].m_position[0] = 0.5f;//x
	vertexData[3].m_position[1] = 0.5f;//y
	vertexData[3].m_position[2] = 0.0f;//y
	//colour for v4
	vertexData[3].m_colour[0] = 1.0f;//r
	vertexData[3].m_colour[1] = 1.0f;//g
	vertexData[3].m_colour[2] = 0.0f;//b

	//triangle 1
	indexData.resize(6);
	indexData[0] = 0;//vertex1
	indexData[1] = 1;//vertex2
	indexData[2] = 2;//vertex3

	//triangle 2 
	indexData[3] = 3;//vertex3
	indexData[4] = 0;//vertex2
	indexData[5] = 2;//vertex3

	//create the mesh and test if it failed 
	if (!m_mesh->CreateMesh(vertexData, indexData)) {
		UDebug::Log("Failed to create debug mesh");
	}

	//create triangle 
	vertexData2.resize(3);
	//vertex1
	vertexData2[0].m_position[0] = -0.5f;//x
	vertexData2[0].m_position[1] = 0.5f;//y
	vertexData2[0].m_position[2] = 0.0f;//z
	//colour for v1
	vertexData2[0].m_colour[0] = 1.0f;//r
	vertexData2[0].m_colour[1] = 0.0f;//g
	vertexData2[0].m_colour[2] = 0.0f;//b
	//vertex2
	vertexData2[1].m_position[0] = 0.0f;
	vertexData2[1].m_position[1] = -0.5f;
	vertexData2[1].m_position[2] = 0.0f;
	//colour for v2
	vertexData2[1].m_colour[0] = 0.0f;//r
	vertexData2[1].m_colour[1] = 1.0f;//g
	vertexData2[1].m_colour[2] = 0.0f;//b
	//vertex3
	vertexData2[2].m_position[0] = 0.5f;
	vertexData2[2].m_position[1] = 0.5f;
	vertexData2[2].m_position[2] = 0.0f;
	//colour for v3
	vertexData2[2].m_colour[0] = 0.0f;//r
	vertexData2[2].m_colour[1] = 0.0f;//g
	vertexData2[2].m_colour[2] = 1.0f;//b

	indexData2.resize(3);
	indexData2[0] = 0;//vertex1
	indexData2[1] = 1;//vertex2
	indexData2[2] = 2;//vertex3

	//create the mesh and test if it failed 
	if (!m_mesh2->CreateMesh(vertexData2, indexData2)) {
		UDebug::Log("Failed to create debug mesh");
	}



	//create diamond
	vertexData3.resize(6);
	//vertex1
	vertexData3[0].m_position[0] = 0.0f;//x
	vertexData3[0].m_position[1] = 0.5f;//y
	vertexData3[0].m_position[2] = 0.0f;//z
	//colour for v1
	vertexData3[0].m_colour[0] = 0.0f;//r
	vertexData3[0].m_colour[1] = 0.0f;//g
	vertexData3[0].m_colour[2] = 1.0f;//b
	//vertex2
	vertexData3[1].m_position[0] = -0.5f;
	vertexData3[1].m_position[1] = -0.5f;
	vertexData3[1].m_position[2] = 0.0f;
	//colour for v2
	vertexData3[1].m_colour[0] = 0.0f;//r
	vertexData3[1].m_colour[1] = 0.0f;//g
	vertexData3[1].m_colour[2] = 1.0f;//b
	//vertex3
	vertexData3[2].m_position[0] = 0.5f;
	vertexData3[2].m_position[1] = -0.5f;
	vertexData3[2].m_position[2] = 0.0f;
	//colour for v3
	vertexData3[2].m_colour[0] = 0.0f;//r
	vertexData3[2].m_colour[1] = 0.0f;//g
	vertexData3[2].m_colour[2] = 1.0f;//b
	//vertex4
	vertexData3[3].m_position[0] = -0.5f;//x
	vertexData3[3].m_position[1] = 0.2f;//y
	vertexData3[3].m_position[2] = 0.0f;//z
	//colour for v4
	vertexData3[3].m_colour[0] = 0.0f;//r
	vertexData3[3].m_colour[1] = 0.0f;//g
	vertexData3[3].m_colour[2] = 1.0f;//b
	//vertex5
	vertexData3[4].m_position[0] = 0.0f;
	vertexData3[4].m_position[1] = -0.7f;
	vertexData3[4].m_position[2] = 0.0f;
	//colour for v5
	vertexData3[4].m_colour[0] = 0.0f;//r
	vertexData3[4].m_colour[1] = 0.0f;//g
	vertexData3[4].m_colour[2] = 1.0f;//b
	//vertex6
	vertexData3[5].m_position[0] = 0.5f;
	vertexData3[5].m_position[1] = 0.2f;
	vertexData3[5].m_position[2] = 0.0f;
	//colour for v6
	vertexData3[5].m_colour[0] = 0.0f;//r
	vertexData3[5].m_colour[1] = 0.0f;//g
	vertexData3[5].m_colour[2] = 1.0f;//b


	indexData3.resize(6);
	indexData3[0] = 0;//vertex1
	indexData3[1] = 1;//vertex2
	indexData3[2] = 2;//vertex3

	indexData3[3] = 3;//vertex1
	indexData3[4] = 4;//vertex2
	indexData3[5] = 5;//vertex3

	//create the mesh and test if it failed 
	if (!m_mesh3->CreateMesh(vertexData3, indexData3)) {
		UDebug::Log("Failed to create debug mesh");
	}


	
	return true;
}

void UGraphicsEngine::Render(SDL_Window* sdlWindow)
{


	//set a background colour 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//clear the back last frame	
	glClear(GL_COLOR_BUFFER_BIT);

	static USTransform transform;
	transform.position.x = 0.7f;
	transform.rotation.z += 0.01f;
	transform.scale  = glm::vec3(0.5f);

	//render custom graphics
	m_mesh->Render(m_shader, transform);

	transform.position.x = 0.0f;
	//render custom graphics
	m_mesh2->Render(m_shader, transform);

	transform.position.x = -0.7f;
	//render custom graphics
	m_mesh3->Render(m_shader, transform);



	//presend the frame to the window 
	//swqaping the back buffer with thre front buffer
	SDL_GL_SwapWindow(sdlWindow);

}

