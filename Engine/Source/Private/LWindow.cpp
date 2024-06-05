#include "LWindow.h"
// External Libs 
#include <SDL/SDL.h>

LWindow::LWindow()
{
	m_sdlWindow = nullptr;
	m_shouldClose = false;

	std::cout << "Window created" << std::endl;
}

LWindow::~LWindow()
{
	//if sdl window exsist, destroy it 
	if (m_sdlWindow)
		SDL_DestroyWindow(m_sdlWindow);

	std::cout << "Window destroyed" << std::endl;
}

bool LWindow::CreateWindow(const LSWindowParmas& params)
{
	//enabling the paramater to the member for the window 
	unsigned int windowFlags = SDL_WINDOW_OPENGL;

	//assigning the paramaters to the memebr for the window 
	m_params = params;

	//add vsync flag is it's selected 
	if (m_params.vsync)
		windowFlags += SDL_WINDOW_ALLOW_HIGHDPI;

	//set fullscreen if it's set 
	if (m_params.fullscreen)
		windowFlags += SDL_WINDOW_FULLSCREEN_DESKTOP; //fullscreen borderless
	else
		windowFlags += SDL_WINDOW_SHOWN; //window mode

	//create the sdl window 
	m_sdlWindow = SDL_CreateWindow(
		m_params.title.c_str(),
		m_params.x,
		m_params.y,
		m_params.w,
		m_params.h,
		windowFlags
	);

	//check if window was created 
	if (!m_sdlWindow) {
		std::cout << "SDL failed to create window" << SDL_GetError() << std::endl;
		CloseWindow();
		return false;
	}



	return true;
}
