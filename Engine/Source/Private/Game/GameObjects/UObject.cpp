#include "Game/GameObjects/UObject.h"

UObject::UObject()
{
	m_pendingDestroy = false; 
	m_lifeTime = 0.0f;
	m_lifeTimeTimer = 0.0f; 


	UDebug::Log("UObject created");
}

UObject::~UObject()
{

	UDebug::Log("UObject destroyed");
}

void UObject::Start()
{
	OnStart();
}

void UObject::Tick(float deltaTime)
{
	OnTick(deltaTime);

	if (m_lifeTime > 0.0f) {
		//countdown the life timer 
		m_lifeTimeTimer -= deltaTime; 

		//when the timer hits 0 destroy the object 
		if (m_lifeTimeTimer <= 0.0f) {
			Destroy(); 
		}
	}
}

void UObject::PostTick(float deltaTime)
{
	OnPostTick(deltaTime);
}

void UObject::Destroy()
{
	m_pendingDestroy = true; 
	UGameEngine::GetGameEngine()->DestroyObject(shared_from_this());
	
}
