#pragma once
#define TAGSIZE 100

typedef enum _shape_type
{
	SHAPE_SQUARE,
	SHAPE_CIRCLE,
	SHAPE_RECT,
	SHAPE_TRIANGLE,
	SHAPE_POLY
} ShapeType;

typedef struct _shape
{
	char tag[TAGSIZE]; // find by name
	unsigned int corners;

	float pos[2]; // x and y
	float scale;
	float rotation;

	float colour[4]; // including alpha

	ShapeType type;
} Shape;

Shape* new_shape(const char* tag, unsigned int corners,
	float x, float y, float scale,
	float r, float g, float b, float a,
	ShapeType type);