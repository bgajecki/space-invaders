#pragma once

#include "Player.hpp"
#include "Asteroids.hpp"
#include "Constants.hpp"

class GameProcess
{
public:
	GameProcess();

	void Display();
	void Reshape(int, int);
	void Special(int, int, int);
	void Keyboard(unsigned char, int, int);
	void Loop(int);
	void CreateAsteroid(int);
	void Time(int);

private:
	bool isCollision(Object_t&, Object_t&);
	bool isCollision(Object&, Object&);
	GLuint loadTexture(const char*);
	float calibrateWidth(float);
	float calibrateHeight(float);
	float calibratePosX(float);
	float calibratePosY(float);

	Player player;
	std::vector<std::unique_ptr<Asteroid>> asteroids;
	GLuint asteroidTexture[3];
	unsigned score = 0;
};
