#pragma once

#include "Object.hpp"

class Asteroid : public Object
{
public:
	void Fly();
	float speed;
};