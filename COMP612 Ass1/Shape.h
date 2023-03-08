#pragma once

typedef struct _shape
{
	char tag[100]; // find by name
	unsigned int corners;

	float pos[2]; // x and y
	float scale; 
	float rotation;

	float colour[4]; // including alpha

	ShapeType type;
} Shape;

typedef enum _shape_type
{
	SHAPE_SQUARE,
	SHAPE_CIRCLE,
	SHAPE_RECT,
	SHAPE_TRIANGLE,
	SHAPE_POLY
} ShapeType;