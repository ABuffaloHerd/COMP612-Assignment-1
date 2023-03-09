#include <freeglut.h>
#include "Shape.h"
#include "RenderFunctions.h"
#include <stdio.h>

// CUSTOM RENDER FUNCTIONS
// MUST USE THIS SIGNATURE
// void <name>(struct _shape* self)

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