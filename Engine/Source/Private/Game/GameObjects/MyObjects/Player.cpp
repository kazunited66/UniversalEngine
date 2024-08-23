#include "Game/GameObjects/MyObjects/Player.h"
#include "Graphics/UGraphicsEngine.h"
#include "Graphics/USCamera.h"
Player::Player()
{
	
}

void Player::OnStart()
{
	if (const auto& colRef = AddCollision({ GetTransform().position, glm::vec3(10.0f) }, true).lock()) {

		colRef->type = UECollisionTypes::PLAYER;
	}
}

void Player::OnTick(float deltaTime)
{
	UWorldObject::OnTick(deltaTime);

	if (const auto& camRef = UGameEngine::GetGameEngine()->GetGraphics()->GetCamera().lock()) {
		GetTransform() = camRef->transform; 
	}
}
