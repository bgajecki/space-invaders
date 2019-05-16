#include <engine/Engine.hpp>
#include "constants.hpp"


GLuint LoadTexture(const char* filename)
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

float calibrateWidth(float size)
{
	return size * (1.0f / (float)WINDOW_WIDTH);
}

float calibrateHeight(float size)
{
	return size * (1.0f / (float)WINDOW_HEIGHT);
}

float calibratePosX(float x)
{
	return x * (2.0f / (float)WINDOW_WIDTH) - 1.0f;
}

float calibratePosY(float y)
{
	return y * (2.0f / (float)WINDOW_HEIGHT) - 1.0f;
}


Player player;
std::vector<std::unique_ptr<Asteroid>> asteroid;
GLuint asteroidTexture[3];
unsigned score = 0;

void Display()
{
	// Background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// Start painting
	player.Paint();
	for (auto &i : player.bullet)
		if(i)
			i->Paint();
	for (auto &i : asteroid)
		if(i)
			i->Paint();
	// End painting
	glFlush();
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}


void Reshape(int width, int height)
{
	Display();
}

void Special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		player.Move(calibrateWidth(-20));
		break;
	case GLUT_KEY_RIGHT:
		player.Move(calibrateWidth(20));
		break;
	case GLUT_KEY_UP:
		player.Shoot();
		break;
	}
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case VK_ESCAPE:
		exit(0);
		break;
	}
}

void Loop(int t)
{
	for (auto &i : player.bullet)
		if(i)
			if (i->object.y > 0.5f)
				i.reset();
	for (auto &i : asteroid)
		if (i)
			if (i->object.y < -1.0f)
				i.reset();
	glutPostRedisplay();
	glutTimerFunc(13, Loop, 0);
}

void CreateAsteroid(int t)
{
	int i = -1;
	if (asteroid.size() > 0)
	{
		for (unsigned j = 0; j < asteroid.size(); ++j)
		{
			if (asteroid[j] == nullptr)
			{
				asteroid[j] = std::make_unique<Asteroid>();
				i = j;
			}
		}
	}
	if (i == -1)
	{
		asteroid.push_back(std::make_unique<Asteroid>());
		i = asteroid.size() - 1;
	}
	asteroid[i]->object.width = calibrateWidth(rand() % 50 + 80);
	asteroid[i]->object.height = calibrateHeight(rand() % 50 + 80);
	asteroid[i]->object.x = calibratePosX(rand() % WINDOW_WIDTH);
	asteroid[i]->object.y = calibratePosY(WINDOW_HEIGHT);
	asteroid[i]->object.texture = asteroidTexture[rand()%3];
	asteroid[i]->speed = calibrateHeight(rand() % 20 + 10);
	glutTimerFunc(1000, CreateAsteroid, 0);
}

void Time(int t)
{
	for (auto& i : player.bullet)
		if (i)
			i->Fly();
	for (auto& i : asteroid)
		if (i)
			i->Fly();
	for (auto& i : asteroid)
	{
		if (i)
		{
			if (isCollision(i->object, player.object))
			{
				std::string text = "Zosta�e� trafiony przez asteroide.\nZestrzeli�e� " + std::to_string(score) + " asteroid.";
				MessageBoxA(0, const_cast<char*>(text.c_str()), "Niestety!", 0);
				exit(0);
			}

			for (auto& j : player.bullet)
			{
				if (j)
				{
					if (isCollision(i->object, j->object))
					{
						j.reset();
						i.reset();
						score += 1;
						break;
					}
				}
			}
		}
	}
	glutTimerFunc(100, Time, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand(time(NULL));

	int argc = 1;
	char* argv[1] = {(char*)""};
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow(WINDOW_NAME);
	//
	player.object.texture = LoadTexture("ship.bmp");
	player.bulletTexture = LoadTexture("bullet.bmp");
	asteroidTexture[0] = LoadTexture("ast1.bmp");
	asteroidTexture[1] = LoadTexture("ast2.bmp");
	asteroidTexture[2] = LoadTexture("ast3.bmp");
	player.bulletSpeed = calibrateHeight(30);
	player.bulletHeight = calibrateHeight(50);
	player.bulletWidth = calibrateWidth(25);
	player.object.x = calibratePosX(350);
	player.object.y = calibratePosY(10);
	player.object.width = calibrateWidth(200);
	player.object.height = calibrateHeight(200);
	//
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutSpecialFunc(Special);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(13, Loop, 0);
	glutTimerFunc(1000, CreateAsteroid, 0);
	glutTimerFunc(100, Time, 0);
	glutMainLoop();
	return 0;

}