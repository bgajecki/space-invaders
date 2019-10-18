#include "Engine.hpp"

GameProcess::GameProcess()
{
	this->player.object.texture = this->loadTexture(CONSTANTS::SHIP_TEXTURE);
	this->player.bulletTexture = this->loadTexture(CONSTANTS::BULLET_TEXTURE);

	for(unsigned i = 0; i < CONSTANTS::ASTEROID_TEXTURE_COUNT; i++)
		this->asteroidTexture[i] = this->loadTexture(CONSTANTS::ASTEROID_TEXTURE[i]);

	this->player.bulletSpeed = this->calibrateHeight(30);
	this->player.bulletHeight = this->calibrateHeight(50);
	this->player.bulletWidth = this->calibrateWidth(25);
	this->player.object.x = this->calibratePosX(350);
	this->player.object.y = this->calibratePosY(10);
	this->player.object.width = this->calibrateWidth(200);
	this->player.object.height = this->calibrateHeight(200);
}

bool GameProcess::isCollision(Object_t& o1, Object_t& o2)
{
	if (o1.x < o2.x + o2.width && o1.x + o1.width > o2.x
		&& o1.y < o2.y + o2.height && o1.y + o1.height > o2.y)
		return true;
	else
		return false;
}

bool GameProcess::isCollision(Object& o1, Object& o2)
{
	return this->isCollision(o1.object, o2.object);
}

GLuint GameProcess::loadTexture(const char* filename)
{
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;

	unsigned char* data;

	FILE* file;
	fopen_s(&file, filename, "rb");

	if (file == NULL) return false;

	if (fread(header, 1, 54, file) != 54)
	{
		printf("Not a correct BMP file\n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		printf("Not a correct BMP file\n");
		return 0;
	}

	dataPos = *(int*) & (header[0x0A]);
	imageSize = *(int*) & (header[0x22]);
	width = *(int*) & (header[0x12]);
	height = *(int*) & (header[0x16]);

	if (imageSize == 0)    imageSize = width * height * 3;
	if (dataPos == 0)      dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return texture;
}


float GameProcess::calibrateWidth(float size)
{
	return size * (1.0f / (float)CONSTANTS::WINDOW_WIDTH);
}

float GameProcess::calibrateHeight(float size)
{
	return size * (1.0f / (float)CONSTANTS::WINDOW_HEIGHT);
}

float GameProcess::calibratePosX(float x)
{
	return x * (2.0f / (float)CONSTANTS::WINDOW_WIDTH) - 1.0f;
}

float GameProcess::calibratePosY(float y)
{
	return y * (2.0f / (float)CONSTANTS::WINDOW_HEIGHT) - 1.0f;
}

void GameProcess::Display()
{
	// Background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Start painting
	this->player.Paint();
	for (auto& i : this->player.bullets)
		if (i)
			i->Paint();
	for (auto& i : this->asteroids)
		if (i)
			i->Paint();
	// End painting
	glFlush();
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

void GameProcess::Reshape(int width, int height)
{
	this->Display();
}

void GameProcess::Special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		this->player.Move(this->calibrateWidth(-20));
		break;
	case GLUT_KEY_RIGHT:
		this->player.Move(this->calibrateWidth(20));
		break;
	case GLUT_KEY_UP:
		this->player.Shoot();
		break;
	}
}

void GameProcess::Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case VK_ESCAPE:
		exit(0);
		break;
	}
}

void GameProcess::Loop(int t)
{
	for (auto& i : this->player.bullets)
		if (i)
			if (i->object.y > 0.5f)
				i.reset();
	for (auto& i : this->asteroids)
		if (i)
			if (i->object.y < -1.0f)
				i.reset();
}

void GameProcess::CreateAsteroid(int t)
{
	int i = -1;
	if (this->asteroids.size() > 0)
	{
		for (unsigned j = 0; j < this->asteroids.size() - 1; ++j)
		{
			if (this->asteroids[j] == nullptr)
			{
				this->asteroids[j] = std::make_unique<Asteroid>();
				i = j;
			}
		}
	}
	if (i == -1)
	{
		this->asteroids.push_back(std::make_unique<Asteroid>());
		i = this->asteroids.size() - 1;
	}
	this->asteroids[i]->object.width = this->calibrateWidth(rand() % 50 + 80);
	this->asteroids[i]->object.height = this->calibrateHeight(rand() % 50 + 80);
	this->asteroids[i]->object.x = this->calibratePosX(rand() % CONSTANTS::WINDOW_WIDTH);
	this->asteroids[i]->object.y = this->calibratePosY(CONSTANTS::WINDOW_HEIGHT);
	this->asteroids[i]->object.texture = this->asteroidTexture[rand() % 3];
	this->asteroids[i]->speed = this->calibrateHeight(rand() % 20 + 10);
}

void GameProcess::Time(int t)
{
	for (auto& i : this->player.bullets)
		if (i)
			i->Fly();
	for (auto& i : this->asteroids)
		if (i)
			i->Fly();
	for (auto& i : this->asteroids)
	{
		if (i)
		{
			if (this->isCollision(*i, this->player))
			{
				std::string text = "Zosta³eœ trafiony przez asteroide.\nZestrzeli³eœ " + std::to_string(this->score) + " asteroid.";
				MessageBoxA(0, const_cast<char*>(text.c_str()), "Niestety!", 0);
				exit(0);
			}

			for (auto& j : this->player.bullets)
			{
				if (j)
				{
					if (isCollision(*i, *j))
					{
						j.reset();
						i.reset();
						this->score += 1;
						break;
					}
				}
			}
		}
	}
}


