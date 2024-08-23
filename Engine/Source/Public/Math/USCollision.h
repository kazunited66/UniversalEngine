#pragma once
#include "Math/USBox.h"
#include "EngineTypes.h"


enum class UECollisionTypes {
	ALL,
    COLLECTIBLE,
	PLAYER
};

struct USCollision {
	USCollision() {
		box.position = glm::vec3(0.0f); 
		box.halfSize = glm::vec3(0.0f);
		type = UECollisionTypes::ALL;
		tag = ""; 
	}

	static bool IsOverlapping(const USCollision& col1, const USCollision& col2) {
		return USBox::BoxOverlap(col1.box, col2.box); 
	}

	//collision type 
	UECollisionTypes type;

	//custom tag for detection
	UString tag; 

	//box bounds 
	USBox box; 
};