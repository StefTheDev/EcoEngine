#pragma once

#include "Utility.h"

class Object;

class Component
{
public:

	Object* object;

	virtual void Render() {}
	virtual void Process() {}
	virtual bool Initialise() {}

	virtual ~Component() {}
};