#include "Shape.h"
#include <math.h>
#include <stdio.h>
#include "freeglut.h"

// Radian convertinator 4000
inline double rad(double deg)
{
	return deg * (3.14159 / 180.0);
}

// filthy, dirty, disgusting pointer arithmetic
void rtoiv(float* rgb, float* out)
{
	*out = *rgb / 255.0;
	*(out + 1) = *(rgb + 1) / 255.0;
	*(out + 2) = *(rgb + 2) / 255.0;
	*(out + 3) = 1.0f;
}
// but the bounds of my genius find no limits here


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


Shape* new_custom_shape(char* tag, void(*custom)(struct _shape* self))
{
	// initialize with default values
		Shape* shape = new_shape(tag, 4, 0, 0, 1.0f, 0,
		1.0f, 1.0f, 1.0f, 1.0f, // colour 1
		1.0f, 1.0f, 1.0f, 1.0f, // colour 2
		SHAPE_CUSTOM); // custom shape, so the custom function is in play here

		shape->custom = custom;

		return shape;
}

// GENERIC RENDER FUNCTIONS
void render_square(Shape* s)
{
	// calculate radius of this polygon.
	float r = (s->scale/2) * sqrt(2);

	// load in colours
	// this function only uses one colour.
	glColor4fv(s->colour[0]);
	glBegin(GL_QUADS);


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

void render_triangle(Shape* s)
{
	// starting at the center
	// x = r cos t
	// y = r sin t
	float t = s->rotation + 90;
	float x = s->scale * cos(rad(t));
	float y = s->scale * sin(rad(t));

	glBegin(GL_TRIANGLES);
	glColor4fv(s->colour[0]);
	glVertex2f(x + s->pos[0], y + s->pos[1]);

	glColor4fv(s->colour[1]);
	t += 120;
	x = s->scale * cos(rad(t));
	y = s->scale * sin(rad(t));
	glVertex2f(x + s->pos[0], y + s->pos[1]);
	t += 120;
	x = s->scale * cos(rad(t));
	y = s->scale * sin(rad(t));
	glVertex2f(x + s->pos[0], y + s->pos[1]);
	glEnd();
}

void render_circle(Shape* s)
{
	// colour 1 is inner, colour 2 is outer
	// x = r cos t
	// y = r sin t
	float r = s->scale;

	glBegin(GL_TRIANGLE_FAN);

	glColor4fv(s->colour[0]);
	glVertex2f(s->pos[0], s->pos[1]);

	glColor4fv(s->colour[1]);
	for (int i = 0; i < 720; i++)
	{
		float x = r * cos(rad(i));
		float y = r * sin(rad(i));

		glVertex2f(x + s->pos[0], y + s->pos[1]);
	}

	glEnd();
}

void render_poly(Shape* s)
{
	float r = s->scale;

	glBegin(GL_TRIANGLE_FAN);

	glColor4fv(s->colour[0]);
	glVertex2f(s->pos[0], s->pos[1]);

	glColor4fv(s->colour[1]);

	float theta = s->rotation;

	for (int i = 0; i < s->corners + 1; i++) // one more time to close the loop
	{
		float x = r * cos(rad(theta));
		float y = r * sin(rad(theta));

		glVertex2f(x + s->pos[0], y + s->pos[1]);

		theta += 360.0f / s->corners;
	}

	glEnd();
}