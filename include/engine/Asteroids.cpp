#include "Asteroids.hpp"

void Asteroid::Fly()
{
	this->object.y -= this->speed;
}
