#include <engine/Engine.hpp>

GameProcess* gameProcess;

void Display()
{
	gameProcess->Display();
}

void Reshape(int width, int height)
{
	Display();
}

void Special(int key, int x, int y)
{
	gameProcess->Special(key, x, y);
}

void Keyboard(unsigned char key, int x, int y)
{
	gameProcess->Keyboard(key, x, y);
}

void Loop(int t)
{
	gameProcess->Loop(t);
	glutPostRedisplay();
	glutTimerFunc(CONSTANTS::LOOP, Loop, 0);
}

void CreateAsteroid(int t)
{
	gameProcess->CreateAsteroid(t);
	glutTimerFunc(CONSTANTS::CREATE_ASTEROID, CreateAsteroid, 0);
}

void Time(int t)
{
	gameProcess->Time(t);
	glutTimerFunc(CONSTANTS::TIME, Time, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand(time(NULL));

	int argc = 1;
	char* argv[1] = { (char*)"" };
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(CONSTANTS::WINDOW_WIDTH, CONSTANTS::WINDOW_HEIGHT);
	glutCreateWindow(CONSTANTS::WINDOW_NAME);

	GameProcess process;
	gameProcess = &process;
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutSpecialFunc(Special);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(CONSTANTS::LOOP, Loop, 0);
	glutTimerFunc(CONSTANTS::CREATE_ASTEROID, CreateAsteroid, 0);
	glutTimerFunc(CONSTANTS::TIME, Time, 0);
	glutMainLoop();

	return 0;
}