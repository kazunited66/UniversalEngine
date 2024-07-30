#pragma once
#include "EngineTypes.h"
#include "Game/UGameEngine.h"

class UObject: public std::enable_shared_from_this<UObject> {
public: 
	UObject();
	virtual ~UObject();

	//Run When the object spawn in 
	void Start();

	//Run every frame, passes in deltaTime 
	void Tick(float deltaTime);

	//Run after each tick every frame 
	void PostTick(float deltaTime);

	//mark the object for destroy 
	void Destroy(); 

	//test if the object is marked for destroy 
	bool IsPendingDestroy() const { return m_pendingDestroy;  }

	//set the lifeTime of the object to be destroyed after seconds 
	void SetLifeTime(float lifeTime) { 
		
		m_lifeTime = lifeTime; 
		m_lifeTimeTimer = m_lifeTime; 
	
	}

protected: 
	//Run When the object spawn in 
	virtual void  OnStart() {}

	//Run every frame, passes in deltaTime 
	virtual void  OnTick(float deltaTime) {}

	//Run after each tick every frame
	virtual void  OnPostTick(float deltaTime) {}


private: 
	//if marked for destroyed 
	bool m_pendingDestroy; 

	//if set destroy object after value of time 
	float m_lifeTime; 

	//time b4 destroyed 
	float m_lifeTimeTimer;
};