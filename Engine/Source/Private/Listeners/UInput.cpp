#include "Listeners/UInput.h"
#include "UWindow.h"


UInput::UInput()
{
	OnKeyPressed = TMakeShared<UEvents<SDL_Scancode>>();
	OnKeyReleased = TMakeShared<UEvents<SDL_Scancode>>();
	OnMouseMove = TMakeShared<UEvents<float, float, float, float>>();
	OnMouseScroll = TMakeShared<UEvents<float>>();
	OnMousePressed = TMakeShared<UEvents<UUi8>>();
	OnMouseReleased = TMakeShared<UEvents<UUi8>>();

	m_lastMotion = SDL_MouseMotionEvent();

	
}

void UInput::InitInput(const TShared<UWindow>& window)
{
	//to assign a weak pointer, just plug in the shared pointer 
	m_window = window;
}

void UInput::UpdateInputs()
{
	//TO DO: Game Loop
	SDL_Event e;
	bool mouseMoved = false;
	


	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			if (const auto& windowRef = m_window.lock()) {
				windowRef->CloseWindow();
			}
		}
		//run different events depending on the type of input 
		switch (e.type) {
		case SDL_KEYDOWN:
			//is this key repeated since it was held down 
			if(e.key.repeat == 0 )
			   OnKeyPressed->Run(e.key.keysym.scancode);
			break;
		case SDL_KEYUP:
			if(e.key.repeat==0)
			   OnKeyReleased->Run(e.key.keysym.scancode);
			break;

		case SDL_MOUSEMOTION:
			//when mouse moves run the mouse move event 
			//this only runs when the mouse moves so we need another point...
			//...to determine when the mouse stopped motion 
			OnMouseMove->Run(
				static_cast<float>(e.motion.x),
				static_cast<float>(e.motion.y),
				static_cast<float>(e.motion.xrel),
				static_cast<float>(e.motion.yrel)
			); 
		    m_lastMotion = e.motion;
			mouseMoved = true;
			break;

		case SDL_MOUSEWHEEL:
			//run the scroll event when the mouse wheel scrolls
			OnMouseScroll->Run(e.wheel.preciseY);
			break;

		case SDL_MOUSEBUTTONDOWN:
			//when the mouse button down run pressed event
			OnMousePressed->Run(e.button.button);
			break;

		case SDL_MOUSEBUTTONUP:
			//when the mouse button up run pressed event
			OnMouseReleased->Run(e.button.button);
			break;
		}
	}

	//if the mouse has'not moved and last motion was movement 
	//run the ouse function nd 0 out relative movement 
	if (!mouseMoved && (m_lastMotion.xrel != 0 || m_lastMotion.yrel != 0) ) {
		OnMouseMove->Run(
		static_cast<float>(e.motion.x),
		static_cast<float>(m_lastMotion.y),
		0.0f, 0.0f
		);

		m_lastMotion.xrel = 0;
		m_lastMotion.yrel = 0;
	}
}

void UInput::ShowCursor(const bool& enable)
{
	//hide the mouse and set the cursor to the center of the screen if true 
	//using shorthand if atatement to get result 
	//condition ? [result if true] : [result if false] 
	SDL_SetRelativeMouseMode(enable ? SDL_FALSE : SDL_TRUE);

}

bool UInput::IsCursorHidden() const
{
	
	return SDL_GetRelativeMouseMode() == SDL_TRUE ?  true : false;
}
