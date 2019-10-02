#pragma once

#include "Component.h"

class TransformComponent : public Component
{
public:
	TransformComponent(Vector2 position, Vector2 velocity) {
		this->position = position;
		this->velocity = velocity;
	}

	void Process() override {
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

	glm::vec2

private:
	Vector2 position, velocity;
	float speed = 3.0f;
};
