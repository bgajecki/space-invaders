#include "Player.hpp"
#include "Asteroids.hpp"

void Player::Move(float x)
{
	this->object.x += x;
}

void Player::Shoot()
{
	int i = -1;
	if (bullets.size() > 0)
	{
		for (unsigned j = 0; j < bullets.size(); ++j)
		{
			if (bullets[j] == nullptr)
			{
				bullets[j] = std::make_unique<Bullet>();
				i = j;
			}
		}
	}
	if (i == -1)
	{
		bullets.push_back(std::make_unique<Bullet>());
		i = bullets.size() - 1;
	}

	this->bullets[i]->object.x = this->object.x + (this->object.width / 2.0f) - 0.015f;
	this->bullets[i]->object.y = this->object.y + this->object.height;
	this->bullets[i]->object.width = this->bulletWidth;
	this->bullets[i]->object.height = this->bulletHeight;
	this->bullets[i]->object.texture = this->bulletTexture;
	this->bullets[i]->speed = this->bulletSpeed;
}
