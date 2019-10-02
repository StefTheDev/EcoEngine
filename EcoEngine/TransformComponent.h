#pragma once

#include "Component.h"

class TransformComponent : public Component
{
public:
	TransformComponent(glm::vec2 position, glm::vec2 velocity) {
		this->position = position;
		this->velocity = velocity;
	}

	void Process() override {
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

	void Translate(glm::vec2 position)
	{
		this->position += position;
	}

	glm::vec2 GetPosition()
	{
		return position;
	}

	glm::vec2 GetVelocity()
	{
		return velocity;
	}

	float GetSpeed()
	{
		return speed;
	}

private:
	glm::vec2 position, velocity;
	float speed = 3.0f;
};
