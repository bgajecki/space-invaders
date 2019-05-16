#include "Head.hpp"

void Object::Paint()
{
	glBindTexture(GL_TEXTURE_2D, this->object.texture);
	glBegin(GL_QUADS);
	glTexCoord3f(0.0f, 0.0f, 0.0f);
	glVertex3f(this->object.x, this->object.y, 0.0f);
	glTexCoord3f(0.0f, 1.0f, 0.0f);
	glVertex3f(this->object.x, this->object.y + this->object.height, 0.0f);
	glTexCoord3f(1.0f, 1.0f, 0.0f);
	glVertex3f(this->object.x + this->object.width, this->object.y + this->object.height, 0.0f);
	glTexCoord3f(1.0f, 0.0f, 0.0f);
	glVertex3f(this->object.x + this->object.width, this->object.y, 0.0f);
	glEnd();
}
