#include "Bullet.hpp"

void Bullet::Fly()
{
	this->object.y += this->speed;
}
