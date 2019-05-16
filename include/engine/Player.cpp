#include "Player.hpp"
#include "Asteroids.hpp"

void Player::Move(float x)
{
	this->object.x += x;
}

void Player::Shoot()
{
	int i = -1;
	if (bullet.size() > 0)
	{
		for (unsigned j = 0; j < bullet.size(); ++j)
		{
			if (bullet[j] == nullptr)
			{
				bullet[j] = std::make_unique<Bullet>();
				i = j;
			}
		}
	}
	if (i == -1)
	{
		bullet.push_back(std::make_unique<Bullet>());
		i = bullet.size() - 1;
	}

	this->bullet[i]->object.x = this->object.x + (this->object.width / 2.0f) - 0.015f;
	this->bullet[i]->object.y = this->object.y + this->object.height;
	this->bullet[i]->object.width = this->bulletWidth;
	this->bullet[i]->object.height = this->bulletHeight;
	this->bullet[i]->object.texture = this->bulletTexture;
	this->bullet[i]->speed = this->bulletSpeed;
}
