#pragma once

#include "Player.hpp"
#include "Asteroids.hpp"

bool isCollision(Object_t& o1, Object_t& o2)
{
	if (o1.x < o2.x + o2.width && o1.x + o1.width > o2.x
		&& o1.y < o2.y + o2.height && o1.y + o1.height > o2.y)
		return true;
	else
		return false;
}
