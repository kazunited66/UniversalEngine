#pragma once
#include "Game/GameObjects/UWorldObject.h"

class Helmet : public UWorldObject {
public: 
	Helmet(); 

protected:
	void OnStart() override; 
	void OnTick(float deltaTime) override;

	void OnOverlap(const TShared<UWorldObject>& other, const TShared<USCollision>& otherCol) override; 
};