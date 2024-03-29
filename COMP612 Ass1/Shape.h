#pragma once
#define TAGSIZE 100
#include <stdarg.h>

typedef enum _shape_type
{
	SHAPE_SQUARE,
	SHAPE_CIRCLE,
	SHAPE_RECT,
	SHAPE_TRIANGLE,
	SHAPE_POLY,
	SHAPE_CUSTOM = 99
} ShapeType;

typedef struct _shape
{
	char tag[TAGSIZE]; // find by name
	unsigned int corners; // only used with SHAPE_POLY or SHAPE_CUSTOM

	float pos[2]; // x and y
	float scale;
	float rotation;

	float colour[2][4]; // two colour slots including alpha

	ShapeType type;

	void(*custom)(struct _shape* self); // custom function for when none of the premade shapes are useful
	void(*update)(struct _shape* self); // update function to keep everything tidy

	unsigned int enabled;
} Shape;

// Shape Constr- I mean factory.
Shape* new_shape(char* tag, unsigned int corners,
	float x, float y, float scale, float rotation,
	float c1r, float c1g, float c1b, float c1a,
	float c2r, float c2g, float c2b, float c2a,
	ShapeType type);

Shape* new_custom_shape(const char* tag, void(*custom)(struct _shape* self));

// Utils
// rgb transmogrifier 6900
inline float rtoi(int rgb)
{
	return (float)rgb / 255.0;
}

void rtoiv(float* rgb, float* out);
// Draw square
void render_square(Shape* s);

// draw a perfect circle
void render_circle(Shape* s);

// DRAW PYRAMID
void render_triangle(Shape* s);
// Draw rectangle

// draw polygon
void render_poly(Shape* s);