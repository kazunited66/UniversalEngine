#pragma once
#include "Game/GameObjects/UWorldObject.h"

class Helmet : public UWorldObject {
public: 
	Helmet(); 

protected:
	void OnTick(float deltaTime) override;
};