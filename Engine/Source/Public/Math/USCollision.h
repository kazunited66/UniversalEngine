#pragma once
#include "Math/USBox.h"
#include "EngineTypes.h"


enum class UECollisionTypes {
	ALL,
    COLLECTIBLE,
	PLAYER
};

class UMesh;

struct USCollision {
	USCollision() {
		box.position = glm::vec3(0.0f); 
		box.halfSize = glm::vec3(0.0f);
		type = UECollisionTypes::ALL;
		debugcolour = glm::vec3(0.0f, 1.0f, 0.0f); 
		tag = ""; 
	}

	static bool IsOverlapping(const USCollision& col1, const USCollision& col2) {
		return USBox::BoxOverlap(col1.box, col2.box); 
	}

	//store the debug mesh 
	TShared<UMesh>debugMesh;

	glm::vec3 debugcolour;

	//collision type 
	UECollisionTypes type;

	//custom tag for detection
	UString tag; 

	//box bounds 
	USBox box; 
};