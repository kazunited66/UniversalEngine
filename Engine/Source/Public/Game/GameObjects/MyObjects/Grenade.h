#pragma once
#pragma once
#include "Game/GameObjects/UWorldObject.h"

class Grenade : public UWorldObject {
public:
	Grenade();

protected:
	void OnStart() override;

	void OnTick(float deltaTime) override;

	void OnOverlap(const TShared<UWorldObject>& other, const TShared<USCollision>& otherCol) override;
};