#pragma once

#include <GL/glut.h>
#include <Windows.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <memory>
#include <string>

struct Object_t
{
	float height, width, x, y;
	GLuint texture;
};

class Object
{
public:
	virtual void Paint();
	Object_t object;
};

