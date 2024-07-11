#pragma once
#include "EngineTypes.h"
#include "Listeners/UEvents.h"

//External Libs
#include <SDL/SDL_keycode.h>
#include <SDL/SDL_events.h>


class UWindow;

class UInput {
public:

	UInput();
	//initialise the window 
	void InitInput(const TShared<UWindow>& window);

	//updating the inputs
	void UpdateInputs();

	//On a pressed event 
	TShared<UEvents<SDL_Scancode>> OnKeyPressed;

	//On a released event 
	TShared<UEvents<SDL_Scancode>> OnKeyReleased;

	//On a mouse move
	//@param 1: mouse pos x
	//@param 2: mouse pos y
	//@param 3: x relative to last x
	//@param 4: y relative to last y 
	TShared<UEvents<float, float, float, float>> OnMouseMove;

	//When the mouse scrolls pass the delta
	TShared<UEvents<float>> OnMouseScroll;

	//listen for mouse pressed events
	TShared<UEvents<UUi8>> OnMousePressed;

	//listen for mouse released events
	TShared<UEvents<UUi8>> OnMouseReleased;


	//hide mouse function 
	//this will also make sure the mouse is center to the screen when moving 
	void ShowCursor(const bool& enable);

	//get the state of the cursor
	bool IsCursorHidden() const;


private:

	//week pointer to the window to not count as a reference 
	//so that the input does not controll if the window gets destroyed or not 
	TWeak<UWindow>m_window;

	//last mouse motion event 
	SDL_MouseMotionEvent m_lastMotion;


};