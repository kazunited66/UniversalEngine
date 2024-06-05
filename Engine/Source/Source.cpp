#include <iostream>

// Extarnal Libs 
#include <SDL/SDL.h>

//External Libs
#include "LWindow.h"

//source variables 
std::unique_ptr<LWindow> m_window;


//source fanctions 
bool Initialise()
{
	//initialize the component od SDL that we have 
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
		std::cout << "Failed to init SDL:" << SDL_GetError() << std::endl;
		return false;

	}

	//creating window object 
	m_window = std::make_unique<LWindow>();

	//creating an sdl window 
	if (!m_window->CreateWindow({ "Game Window",
		SDL_WINDOWPOS_CENTERED_DISPLAY(1),SDL_WINDOWPOS_CENTERED_DISPLAY(1),
		1280, 720 }))
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

	}



	//clean up the engine
	CleanUp();

	return 0;
}