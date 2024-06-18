#include <iostream>

// Extarnal Libs 
#include <SDL/SDL.h>

//External Libs
#include "UWindow.h"

//source variables 
std::unique_ptr<UWindow> m_window;


//source fanctions 
bool Initialise()
{
	//initialize the component od SDL that we have 
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
		std::cout << "Failed to init SDL:" << SDL_GetError() << std::endl;
		return false;

	}

	//tell SDL that we will be rendering in open gl version 460 or 4.60
	//4 is maijor version 
	//.60 is the minor version 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	//make sure if open gl is a using a version not in SDL we run SDL in compatibility mode 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	//set the bit depth for each colour 
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);



	//creating window object 
	m_window = std::make_unique<UWindow>();

	//creating an sdl window 
	if (!m_window->CreateWindow({ "Game Window",
		SDL_WINDOWPOS_CENTERED_DISPLAY(1),SDL_WINDOWPOS_CENTERED_DISPLAY(1),
		720, 720 }))
		return false;


	return true;
}

void CleanUp() {
	//exit SDK and deallocate all initialised SDL component 
	SDL_Quit();

}


int main(int argc, char* argv[]) {
	if (!Initialise()) {
		CleanUp();
		return-1;
	}


	// keep the game open as long as the window is open 
	while (!m_window->IsPendingClose()) {
		//TO DO: Game Loop
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				m_window->CloseWindow();
			}
		}
		//render the window 
		m_window->Render();
	}



	//clean up the engine
	CleanUp();

	return 0;
}