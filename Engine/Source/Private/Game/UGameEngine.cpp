#include"Game/UGameEngine.h"
#include"Game/GameObjects/UWorldObject.h"

//CUSTOM
#include"Game/GameObjects/MyObjects/Helmet.h"
#include"Game/GameObjects/MyObjects/Knife.h"
#include"Game/GameObjects/MyObjects/Player.h"
#include"Game/GameObjects/MyObjects/Grenade.h"
UGameEngine* UGameEngine::GetGameEngine()
{
	static UGameEngine* instance = new UGameEngine();
	return instance;
}

void UGameEngine::DestroyEngine()
{
	delete GetGameEngine();

}

bool UGameEngine::Run()
{
	if (!Init()) {
		UDebug::Log("Game engine failed to initialise", LT_ERROR);
		return false;
	}

	Start();

	GameLoop();


	

	return true;
}

void UGameEngine::DestroyObject(const TShared<UObject>& object)
{
	m_objectsPendingDestroy.push_back(object); 
}

TUnique<UGraphicsEngine>& UGameEngine::GetGraphics()
{
	return m_window->GetGraphics(); 
}

UGameEngine::UGameEngine()
{
	m_lastTickTime = 0.0;
	m_dealtaTime = 0.0; 
	UDebug::Log("Game Engine created");
}

UGameEngine::~UGameEngine()
{
	CleanUp();

	UDebug::Log("Game Engine destroyed");
}

bool UGameEngine::Init()
{
	//initialize the component od SDL that we have 
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {

		UDebug::Log("Falied to init SDL: " + UString(SDL_GetError()), LT_ERROR);
		return false;

	}

	//tell SDL that we will be rendering in open gl version 460 or 4.60
	//4 is maijor version 
	//.60 is the minor version 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	//make sure if open gl is a using a version not in SDL we run SDL in compatibility mode 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	//set the bit depth for each colour 
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);



	//creating window object 
	m_window = TMakeShared<UWindow>();

	//creating an sdl window 
	if (!m_window->CreateWindow({ "Game Window",
		SDL_WINDOWPOS_CENTERED_DISPLAY(1),SDL_WINDOWPOS_CENTERED_DISPLAY(1),
		720, 720 }))
		return false;

	//create the input class and assign the window 
	m_input = TMakeShared<UInput>();
	m_input->InitInput(m_window);
	


	return true;

}

void UGameEngine::Start()
{
	//register the window input 
	m_window->RegisterInput(m_input);
	TWeak<Helmet> helmet = CreateObject<Helmet>(); 
	TWeak<Knife> knife = CreateObject<Knife>();
	TWeak<Grenade> grenade = CreateObject<Grenade>();
	CreateObject<Player>();
	

	helmet.lock()->GetTransform().position = glm::vec3(0.0f, 50.0f, 50.0f);
	knife.lock()->GetTransform().position = glm::vec3(0.0f, 0.0f, 50.0f);
	grenade.lock()->GetTransform().position = glm::vec3(0.0f, -50.0f, 50.0f);
}

void UGameEngine::GameLoop()
{
	// keep the game open as long as the window is open 
	while (!m_window->IsPendingClose()) {
		//create delta time 
		//set the current tick time 
		//SDL_GetTick64() gives us time since the engine started ibn millisecond 
		double curTickTime = static_cast<double>(SDL_GetTicks64()); 
		//convert the tick time into delta 
		//how much time has passed since the last frame 
		double deltaMili = curTickTime - m_lastTickTime; 
		//convert delta mili to secconds 
		m_dealtaTime = deltaMili / 1000.0;
		//updates the last tick time to the current time for the next loop 
		m_lastTickTime = curTickTime; 

		//the order of thoes functions is important 
		//we want to detect input> react  to input with logis > render based on logic 
	
		PreLoop();
		
		//process for engine input 
		ProcessInput();

		//process all engine tick functions 
		Tick();

		//process all engine render functions 
		Render();

		PostLoop();
		
	}

}

void UGameEngine::CleanUp()
{
	m_input = nullptr;
	m_window = nullptr; 


	SDL_Quit();
}

void UGameEngine::Tick()
{
	//Run through all objects in the game and run their ticks 
	for (const auto& UObjectRef : m_objectStack) {
		UObjectRef->Tick(DeltaTimeF());

		//check the object is a world object, otherwise skip logic 
		if (const auto& woRef = std::dynamic_pointer_cast<UWorldObject>(UObjectRef)) {
			//check the object has collisions 
			if (woRef->HasCollisions()) {
				//loop though all objects to ttest again 
				for (const auto& otherObj : m_objectStack) {
					if (const auto& otherWoRef = std::dynamic_pointer_cast<UWorldObject>(otherObj)) {
						//test other object is also a world object 
						if (!otherWoRef->HasCollisions())
							continue;
						//if all is good, test is the collisions are overlapping 
						woRef->TestCollision(otherWoRef);
					}
				}
			}
		}

		//UObjectRef->Tick(DeltaTimeF());
		UObjectRef->PostTick(DeltaTimeF());
	}



}

void UGameEngine::ProcessInput()
{
	if (!m_input)
		return;


	//handle inputs
	m_input->UpdateInputs();
}

void UGameEngine::Render()
{
	if (!m_window)
		return;

	//render the window 
	m_window->Render();
}

void UGameEngine::PreLoop()
{
	//running though all object to be spawned and running their start logic 
	//and adding them into the game objects stack 
	for (auto& UObjectRef : m_objectsToBeSpawned) {
		UObjectRef->Start();
		m_objectStack.push_back(std::move(UObjectRef));

	}

	m_objectsToBeSpawned.clear(); 

}

void UGameEngine::PostLoop()
{

	//loop through all objects pending destroy and remove their references from object stack 
	for (auto& UObjectRef : m_objectsPendingDestroy) {
		auto  it = std::find(m_objectStack.begin(), m_objectStack.end(), UObjectRef);
		if (it == m_objectStack.end())
			continue;

		m_objectStack.erase(it);
	}

	//make sure to clear the pending destroy  array so no references of the object exist 
	m_objectsPendingDestroy.clear(); 
}

