#include "Shape.h"
#include <math.h>
#include <stdio.h>
#include "freeglut.h"

// Radian convertinator 4000
inline double rad(double deg)
{
	return deg * (3.14159 / 180.0);
}

Shape* new_shape(char* tag, unsigned int corners,
	float x, float y, float scale, float rotation,
	float c1r, float c1g, float c1b, float c1a,
	float c2r, float c2g, float c2b, float c2a,
	ShapeType type)
{
	Shape* shape = (Shape*)malloc(sizeof(Shape));

	int len = snprintf(shape->tag, TAGSIZE, "%s", tag);
	if (len >= TAGSIZE) 
	{
		shape->tag[TAGSIZE - 1] = '\0'; // Ensure the tag is null-terminated
	}

	shape->corners = corners;
	shape->pos[0] = x;
	shape->pos[1] = y;
	shape->scale = scale;
	shape->rotation = rotation;

	// Copy in colour information
	shape->colour[0][0] = c1r;
	shape->colour[0][1] = c1g;
	shape->colour[0][2] = c1b;
	shape->colour[0][3] = c1a;

	shape->colour[1][0] = c2r;
	shape->colour[1][1] = c2g;
	shape->colour[1][2] = c2b;
	shape->colour[1][3] = c2a;

	shape->type = type;

	// this function must be explicitly set after creating a new shape
	shape->custom = NULL;

	return shape;
}

void render_square(Shape* s)
{
	// calculate radius of this polygon.
	float r = (s->scale/2) * sqrt(2);

	// load in colours
	// this function only uses one colour.
	glColor4fv(s->colour[0]);
	glBegin(GL_QUADS);

	printf("Colour: %f %f %f\n", s->colour[0][0], s->colour[0][1], s->colour[0][2], s->colour[0][3]);

	// position
	const float px = s->pos[0];
	const float py = s->pos[1];

	// x = r cos t
	// y = r sin t

	float t = s->rotation - 45; // angle theta

	for (int i = 0; i < 4; i++)
	{
		float x = r * cos(rad(t));
		float y = r * sin(rad(t));

		glVertex2f(x + px, y + py);

		t += 90;
	}

	glEnd();
}