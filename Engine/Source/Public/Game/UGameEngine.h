#pragma once
#include"EngineTypes.h"
#include"UWindow.h"
#include"Listeners/UInput.h"


//Exterenal Libs 
#include<SDL/SDL.h>


class UObject; 


class UGameEngine {

public: 
	
	//get or create a game engine if one does not exist 
	static UGameEngine* GetGameEngine();
	//destroy the game engine 
	static void DestroyEngine();

	//run the game engine 
	bool Run();

	//retrun the deltatime between frames 
	double DeltaTime() const { return m_dealtaTime; }

	//return the delta time between frame as a float 
	float DeltaTimeF() const { return static_cast<float>(m_dealtaTime); }
	
	//create a UObject type 
	template<typename T, std::enable_if_t<std::is_base_of_v<UObject,T>, T>* = nullptr>
	TWeak<T> CreateObject()  { 
		//create an object with the btemplate class 
		TShared<T> newObject = TMakeShared<T>();

		//add the object into the stack 
		m_objectsToBeSpawned.push_back(newObject);

		return newObject;
	}

	//mark an object for destroy 
	//all gameobject destroy function will automatically run this 
	void DestroyObject(const TShared<UObject>& object); 

	//get the graphics engine 
	TUnique<UGraphicsEngine>& GetGraphics(); 

private: 

	//constructor and destructor are private to ensure we can only have 1 game engine 
	UGameEngine();
	~UGameEngine();

	//initialise all required libraries for the game engine 
	bool Init();

	//runs after initialise pf the engine 
	void Start();

	//run the loop of the game 
	void GameLoop();

	//clean up game engine 
	void CleanUp();

	//Loop function 
	void Tick(); 

	//process the input for each frame 
	void ProcessInput();

	//render the graphics for each frame 
	void Render(); 

	//runs at the start of each loop 
	void PreLoop();

	//runs at the end of each loop 
	void PostLoop();

	


private:
	//store the window for the game engine 
	TShared<UWindow> m_window;

	//store the input for the game engine 
	TShared<UInput> m_input;

	//last time ticked 
	double m_lastTickTime; 

	//delta time between frame 
	double m_dealtaTime; 

	//store all object in the game engine 
	TArray<TShared<UObject>> m_objectStack;

	//store all UObjects to be started next frame 
	TArray<TShared<UObject>> m_objectsToBeSpawned;

	//store all UObjects have been marked for destroy 
	TArray<TShared<UObject>> m_objectsPendingDestroy;
};