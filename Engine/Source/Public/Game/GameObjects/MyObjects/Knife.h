
#pragma once
#include "Game/GameObjects/UWorldObject.h"

class Knife : public UWorldObject {
public:
	Knife();

protected:
	void OnStart() override;

	void OnTick(float deltaTime) override;

	void OnOverlap(const TShared<UWorldObject>& other, const TShared<USCollision>& otherCol) override;
};