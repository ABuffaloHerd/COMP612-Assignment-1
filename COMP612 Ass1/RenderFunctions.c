#include <freeglut.h>
#include "Shape.h"
#include "RenderFunctions.h"
#include <stdio.h>

// CUSTOM RENDER FUNCTIONS
// MUST USE THIS SIGNATURE
// void <name>(struct _shape* self)

inline double rad(double deg)
{
	return deg * (180.0 / 3.14159);
}

void render_sky(struct _shape* self)
{
	// set the size to 2.0
	self->scale = 2.0f;

	// set the colours and begin drawing
	glBegin(GL_QUADS);
	glColor4f(0, 169.0/255.0f, 239.0/255.0f, 1.0f);

	glVertex2f(1.0f, 1.0f);
	glVertex2f(-1.0f, 1.0f);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(1.0f, -1.0f);

	glEnd();
}

void render_ground(float* f, int size)
{
	// calculate distance between each point
	float dx = 2.0f / (float)size;
	float x = 1.0f;
	float x2 = -1.0f;

	float colour[3] = { 116.0f / 255.0f, 116.0f / 255.0f, 116.0f / 255.0f };

	glBegin(GL_POLYGON);
	// bottom left to right		
	glColor3fv(colour);

	for (int i = 0; i < size; i++)
	{
		glVertex2f(x2, -1.0f);
		x2 += dx;
	}

	glVertex2f(1.0f, -1.0f);

	// starting at the top right, using quad mode
	for (int i = 0; i < size; i++)
	{	
		// dirty pointer arithmetic.
		glColor3f(1.0f, 1.0f, 1.0f);
		float y = *(f + i);
		glVertex2f(x, y);
		x -= dx;
	}

	// last vertex for a smooth transition
	glVertex2f(-1.0f, -0.3f);

	glEnd();
}

void render_spy_balloon(struct _shape* self)
{
	// NOT WORKING: ASK FOR HELP

	//// text that goes on top of the balloon
	const char* text = "WEATHER";

	//glScalef(2.0f, 2.0f, 1.0f);

	// position is the left end of the balloon


	//glScalef(1.0f, 1.0f, 1.0f);

	// balloon
	// the center of this shape parameter is the bottom of the balloon
	Shape balloon;
	balloon.scale = self->scale;
	balloon.pos[0] = self->pos[0];
	balloon.pos[1] = self->pos[1] + balloon.scale;

	// "Only available in C++11 or above"
	//balloon.colour[0] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//balloon.colour[1] = { 1.0f, 1.0f, 1.0f, 1.0f };

	balloon.colour[0][0] = 1.0f;
	balloon.colour[0][1] = 1.0f;
	balloon.colour[0][2] = 1.0f;
	balloon.colour[0][3] = 1.0f;
	balloon.colour[1][0] = 1.0f;
	balloon.colour[1][1] = 1.0f;
	balloon.colour[1][2] = 1.0f;
	balloon.colour[1][3] = 1.0f;

	render_circle(&balloon);

	// lines
	// left line extends from centre of balloon by scale * 0.5
	glLineWidth(1.0f);
	glBegin(GL_LINES);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

	glVertex2f(self->pos[0], self->pos[1]);
	glVertex2f(self->pos[0], self->pos[1] - (self->scale * 0.5f));

	glEnd();

	// black circle that represents the "camera equipment"
	Shape cam;
	cam.scale = self->scale * 0.3f;
	cam.pos[0] = self->pos[0];
	cam.pos[1] = self->pos[1] - self->scale * 0.9f;

	cam.colour[0][0] = 0;
	cam.colour[0][1] = 0;
	cam.colour[0][2] = 0;
	cam.colour[0][3] = 1.0f;
	cam.colour[1][0] = 0;
	cam.colour[1][1] = 0;
	cam.colour[1][2] = 0;
	cam.colour[1][3] = 1.0f;

	render_circle(&cam);

	// the box
	Shape box;
	box.pos[0] = self->pos[0];
	box.pos[1] = self->pos[1] - self->scale * 0.7f;
	box.scale = self->scale * 0.7f;
	box.rotation = 0;

	box.colour[0][0] = 1.0f;
	box.colour[0][1] = 0;
	box.colour[0][2] = 1.0f;
	box.colour[0][3] = 1.0f;

	render_square(&box);

	// text
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glRasterPos2f(self->pos[0] - self->scale + 0.1f * self->scale, self->pos[1] + self->scale);
	glutBitmapString(GLUT_BITMAP_HELVETICA_10, text);
}

void label(float px, float py, char* text)
{
	glRasterPos2f(px, py);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, text);
}