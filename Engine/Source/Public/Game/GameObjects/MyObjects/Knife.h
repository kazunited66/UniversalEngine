
#pragma once
#include "Game/GameObjects/UWorldObject.h"

class Knife : public UWorldObject {
public:
	Knife();

protected:
	void OnTick(float deltaTime) override;
};