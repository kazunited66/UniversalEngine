#pragma once

#include "EngineTypes.h"


class UTexture;


struct USMaterial {
	USMaterial() = default;

	//this is the clour map for the material 
	TShared<UTexture> m_baseColourMap;

	//this is the shininess of the object 
	//a black and white map 
	//white is completly shiny (reflective) 
	//reflection of the light 
	TShared<UTexture> m_specularMap;

	//naterial properties 
	//shiness of the material 
	float shininess = 32.0f;

	//intesity of this specular 
	float specularStrength = 0.5;

};
