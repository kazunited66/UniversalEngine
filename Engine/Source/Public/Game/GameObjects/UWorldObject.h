#pragma once
#include "Game/GameObjects/UObject.h"
#include "Math/USTransform.h"

class UModel; 


class UWorldObject : public UObject {
public:
	UWorldObject() = default; 
	virtual ~UWorldObject() = default; 

	//import a model and return a weak reference 
	TWeak<UModel> ImportModel(const UString& path); 

	//get the objects transform 
	USTransform& GetTransform() { return m_transform;  }

protected:

	void OnPostTick(float deltaTime) override; 
private: 
	//transform 
	USTransform m_transform; 

	//store any models attached to this object 
	TArray<TShared<UModel>> m_objectModels; 
};