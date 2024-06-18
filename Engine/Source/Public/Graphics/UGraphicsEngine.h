#pragma once
#include<iostream>
typedef void* SDL_GLContext;
struct SDL_Window; 
class UMesh;
class UShaderProgram;

class UGraphicsEngine {
public:
	UGraphicsEngine() = default;
	~UGraphicsEngine() = default;

	//initialise the graphics engine 
	bool InitEngine(SDL_Window* sdlWindow, const bool& vsync);

	//render the graphics engine 
	void Render(SDL_Window* sdlWindow);

private:
	// storing memory location for open gl context 
	SDL_GLContext m_sdlGLContext; 

	//store the shader for the engine
	std::shared_ptr<UShaderProgram> m_shader;


};