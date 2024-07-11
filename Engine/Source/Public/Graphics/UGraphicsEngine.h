#pragma once
#include<EngineTypes.h>
typedef void* SDL_GLContext;
struct SDL_Window; 
class UMesh;
class UShaderProgram;
struct USCamera;

class UGraphicsEngine {
public:
	UGraphicsEngine() = default;
	~UGraphicsEngine() = default;

	//initialise the graphics engine 
	bool InitEngine(SDL_Window* sdlWindow, const bool& vsync);

	//render the graphics engine 
	void Render(SDL_Window* sdlWindow);
	//return a weak version of the camera 
	TWeak<USCamera>GetCamera() { return m_camera; }

private:
	// storing memory location for open gl context 
	SDL_GLContext m_sdlGLContext; 

	//store the shader for the engine
	TShared<UShaderProgram> m_shader;
	//store camera 
	TShared<USCamera> m_camera;

};