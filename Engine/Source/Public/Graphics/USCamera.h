#pragma once
#include "Math/USTransform.h"
#include "Game/UGameEngine.h"

struct USCamera {
	USCamera() {
		fov = 70.0f;
		defaultFov = fov;
		aspectRation = 1.0f;
		nearClip = 0.01f;
		farClip = 10000.0f;
		rotateSpeed = 0.1f;
		moveSpeed = 50.0f;

	}
	//Rotate the camera based on the rotation passed in 
	void Rotate(glm::vec3 rotation, glm::vec3 scale = glm::vec3(1.0f)) {

		transform.rotation += rotation * scale * rotateSpeed;

		if (transform.rotation.x < -89.0f) {
			transform.rotation.x = -89.0f;
		}
		if (transform.rotation.x > 89.0f) {
			transform.rotation.x = 89.0f;
		}
	}

	 
	//  the camera based on the rotation passed in 
	void Translate(glm::vec3 translation, glm::vec3 scale = glm::vec3(1.0f)) {
		//move the inout direction forward if required 
		glm::vec3 moveDir = transform.Forward() * translation.z;
		moveDir += transform.Right() * translation.x;
		moveDir.y = translation.y;

		if (glm::length(moveDir) != 0.0f)
			moveDir = glm::normalize(moveDir);

		glm::vec3 direction = moveDir * scale; 

		float deltaTime = 1.0f;
		if (const auto& ge = UGameEngine::GetGameEngine()) {

			deltaTime = ge->DeltaTimeF();
		}
		
		transform.position += direction * moveSpeed * deltaTime;

	}

	//zoom in the fov based ont the amount 
	void Zoom(const float& amount) {
		fov -= amount;
	}

	//set the fov to the default 
	void ResetZoom() {
		fov = defaultFov;
	}
	//use the set the fov so that default fov is updated 
	void SetFOV(const float& newFov) {
		fov = newFov;
		defaultFov = defaultFov;
	}
	USTransform transform;
	float fov;
	//do not change will auto set based on the fov on initialise 
	float defaultFov;
	float aspectRation;
	float nearClip;
	float farClip;
	float rotateSpeed;
	float moveSpeed;

};