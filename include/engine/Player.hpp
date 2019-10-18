#pragma once

#include "Bullet.hpp"

class Player : public Object
{
public:
	void Move(float);
	void Shoot();

	//bullet
	std::vector<std::unique_ptr<Bullet>> bullets;
	GLuint bulletTexture;
	float bulletWidth, bulletHeight, bulletSpeed;
};