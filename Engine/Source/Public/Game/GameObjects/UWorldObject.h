#pragma once
#include "Game/GameObjects/UObject.h"
#include "Math/USTransform.h"
#include "Math/USCollision.h"
class UModel; 


class UWorldObject : public UObject {
public:
	UWorldObject() = default; 
	virtual ~UWorldObject() = default; 

	//import a model and return a weak reference 
	TWeak<UModel> ImportModel(const UString& path); 

	//add a collision to the object 
	TWeak<USCollision> AddCollision(const USBox& box, const bool& debug = false); 

	//get the objects transform 
	USTransform& GetTransform() { return m_transform;  }

	//runt a trest to see if another object is ovellaping 
	void TestCollision(const TShared<UWorldObject>& other); 

	//do not store these as a referencce for memory 
	bool HasCollisions() const { return m_objectCollisions.size() > 0; }

	

protected:

	void OnPostTick(float deltaTime) override; 

	//on collision overlap 
	//detect every frame a collision is overlap 
	virtual void OnOverlap(const TShared<UWorldObject>& other, const TShared<USCollision>&otherCol) {}


private: 
	//transform 
	USTransform m_transform; 

	//store any models attached to this object 
	TArray<TShared<UModel>> m_objectModels; 

	//store the collisions for the model 
	TArray<TShared<USCollision>> m_objectCollisions; 


};