#pragma once
#include "Game/GameObjects/UWorldObject.h"

class Player : public UWorldObject {
public: 
	Player(); 

protected: 
	void OnStart() override;
	void OnTick(float deltaTime) override; 
};