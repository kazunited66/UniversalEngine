#include "UWindow.h"
#include "Graphics/UGraphicsEngine.h"
#include "Debug/UDebug.h"
#include "Listeners/UInput.h"
#include "Graphics/USCamera.h"


// External Libs 
#include <SDL/SDL.h>

UWindow::UWindow()
{
	m_sdlWindow = nullptr;
	m_shouldClose = false;
	m_cameraDirection = glm::vec3(0.0f);
	m_cameraRotation = glm::vec3(0.0f);
	m_canZoom = false;
	m_inputMode = false;

	std::cout << "Window created" << std::endl;
}

UWindow::~UWindow()
{
	//if sdl window exsist, destroy it 
	if (m_sdlWindow)
		SDL_DestroyWindow(m_sdlWindow);

	std::cout << "Window destroyed" << std::endl;
}

bool UWindow::CreateWindow(const LSWindowParmas& params)
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

	//create graphics engine object 
	m_graphicsEngine = std::make_unique<UGraphicsEngine>();
	//initialise the graphics engine and test if it falied 
	if (!m_graphicsEngine->InitEngine(m_sdlWindow, m_params.vsync)) {
		UDebug::Log("Window failed to initialise graphics engine", LT_ERROR);
		m_graphicsEngine = nullptr;
		return false;
	}

	return true;
}

void UWindow::RegisterInput(const TShared<UInput>& m_input)
{
	m_input->ShowCursor(false);

	m_input->OnKeyPressed->Bind([this,m_input](const SDL_Scancode& key) {


		//quick exsit program
		if (key == SDL_SCANCODE_ESCAPE) {
			CloseWindow();
		}
		//toggle the cursor visibility 
		if (key == SDL_SCANCODE_PERIOD) {
			m_input->ShowCursor(m_input->IsCursorHidden());
			//set the game to input mode if curdor is visible 
			m_inputMode = !m_input->IsCursorHidden();
		}
		//add 1 to the direction
		if (key == SDL_SCANCODE_W) {
			m_cameraDirection.z += 1.0f;
		}
		//minus 1 to the direciton 
		if (key == SDL_SCANCODE_S) {
			m_cameraDirection.z += -1.0f;
		}
		if (key == SDL_SCANCODE_A) {
			m_cameraDirection.x += 1.0f;
		}
		//minus 1 to the direciton 
		if (key == SDL_SCANCODE_D) {
			m_cameraDirection.x += -1.0f;
		}
		if (key == SDL_SCANCODE_E) {
			m_cameraDirection.y += 1.0f;
		}
		//minus 1 to the direciton 
		if (key == SDL_SCANCODE_Q) {
			m_cameraDirection.y += -1.0f;
		}
		});
	
	m_input->OnKeyReleased->Bind([this](const SDL_Scancode& key) {
		if (key == SDL_SCANCODE_W) {
			m_cameraDirection.z += -1.0f;
		}
		//minus 1 to the direciton 
		if (key == SDL_SCANCODE_S) {
			m_cameraDirection.z += 1.0f;
		}
		if (key == SDL_SCANCODE_A) {
			m_cameraDirection.x += -1.0f;
		}
		//minus 1 to the direciton 

		if (key == SDL_SCANCODE_D) {
			m_cameraDirection.x += 1.0f;
		}
		if (key == SDL_SCANCODE_E) {
			m_cameraDirection.y += -1.0f;
		}
		//minus 1 to the direciton 
		if (key == SDL_SCANCODE_Q) {
			m_cameraDirection.y += 1.0f;
		}

	});

	//on mouse move rotate the camera if one exsist 
	m_input->OnMouseMove->Bind([this](const float& x, const float& y, 
		const float& xrel, const float& yrel) {
			m_cameraRotation.x = yrel;
			m_cameraRotation.y = xrel;
             
		});
	m_input->OnMouseScroll->Bind([this](const float& delta) {
		if (m_canZoom) {
			if (const auto& camRef = m_graphicsEngine->GetCamera().lock()) {
				camRef->Zoom(delta);
			}
		}
	});

	m_input->OnMousePressed->Bind([this](const UUi8& button) {
		if (button == SDL_BUTTON_RIGHT) {
			m_canZoom = true;

		 }
     });

	m_input->OnMouseReleased->Bind([this](const UUi8& button) {
		if (button == SDL_BUTTON_RIGHT) {
			m_canZoom = false;
			if (const auto& camRef = m_graphicsEngine->GetCamera().lock()) {
				camRef->ResetZoom();
			}

		}
		});
		
		
} 
	


void UWindow::Render()
{
	//render the graphics engine if one exists
	if (m_graphicsEngine) {
		//test if there is  a camera 
		if (const auto& camRef = m_graphicsEngine->GetCamera().lock()) {
			if (!m_inputMode) {
				//translatea the camara based on input direction
				camRef->Translate(m_cameraDirection);
				//rotate the camara based on input direction
				camRef->Rotate(m_cameraRotation, glm::abs(m_cameraRotation));
			}
		}

		m_graphicsEngine->Render(m_sdlWindow);
	}
}
