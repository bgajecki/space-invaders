#pragma once

namespace CONSTANTS
{
	const unsigned WINDOW_WIDTH = 800u;
	const unsigned WINDOW_HEIGHT = 600u;
	const char WINDOW_NAME[] = "Space Invaders";
	const char SHIP_TEXTURE[] = "ship.bmp";
	const char BULLET_TEXTURE[] = "bullet.bmp";
	const unsigned ASTEROID_TEXTURE_COUNT = 3;
	const char ASTEROID_TEXTURE[ASTEROID_TEXTURE_COUNT][9] = { {"ast1.bmp"}, {"ast2.bmp"}, {"ast3.bmp"} };
	const unsigned LOOP = 13u;
	const unsigned CREATE_ASTEROID = 1000u;
	const unsigned TIME = 100u;
}