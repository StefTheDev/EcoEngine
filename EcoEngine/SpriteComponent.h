#pragma once

#include "Component.h"

class SpriteComponent : public Component
{
public:


	void Render() override {

	}

private:
	TransformComponent* transform;
};
