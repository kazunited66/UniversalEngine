#include <EngineTypes.h>

// Extarnal Libs 
#include <SDL/SDL.h>

//External Libs
#include "Game/UGameEngine.h"
#include "UWindow.h"
#include "Listeners/UInput.h"
#include "Graphics/USCamera.h"


int main(int argc, char* argv[]) {
	
	int result = 0;
	//initialise the engine 
	//test if int failed 
	if (!UGameEngine::GetGameEngine()->Run()) {
		
		return -1;
	}

	UGameEngine::DestroyEngine();

	return result;
}