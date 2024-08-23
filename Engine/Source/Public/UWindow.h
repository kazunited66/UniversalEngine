#pragma once
#include "EngineTypes.h"
#include "Math/USTransform.h"
class UGraphicsEngine; 
class UInput;

struct LSWindowParmas
{
	//default constructor
	LSWindowParmas() {
		title = "Universal Engine Window";
		x = y = 0;
		w = 1280;
		h = 720;
		vsync = false;
		fullscreen = false;
	}

	//setting constructor
	LSWindowParmas(std::string title, int x, int y, unsigned int w, unsigned int h) :
		title(title),
		x(x), y(y),
		w(w), h(h),
		vsync(false), fullscreen(false) {}


	//title of window
	std::string title;
	//position of the window 
	int x, y;
	//width and height of the window 
	unsigned int w, h;
	//vsync enable
	bool vsync;
	//fullscreen enable
	bool fullscreen;

};

struct SDL_Window;

class UWindow {

public:
	UWindow();
	~UWindow();

	//create window with parameters
	bool CreateWindow(const LSWindowParmas& params);

	//close the window 
	void CloseWindow() { m_shouldClose = true; }

	//check if the window has been set to close 
	bool IsPendingClose() { return m_shouldClose; }
	//listen for input 
	void RegisterInput(const TShared<UInput>& m_input);

	//render the graphics engine 
	void Render();

	TUnique<UGraphicsEngine>& GetGraphics() { return m_graphicsEngine;  }

private:
	//a ref to the window in sdl 
	SDL_Window* m_sdlWindow;
	//window parameters 
	LSWindowParmas m_params;
	//determine is the window should close 
	bool m_shouldClose;
	//store the graphics engine 
	TUnique< UGraphicsEngine> m_graphicsEngine;
	//direction to move the camera
	glm::vec3 m_cameraDirection;
	//amout to rotate camera
	glm::vec3 m_cameraRotation;

	//can zoom
	bool m_canZoom;

	//is the use in input mode
	bool m_inputMode;
};