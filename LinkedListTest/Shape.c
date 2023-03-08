#include "Shape.h"
#include <stdlib.h>

Shape* new_shape(const char* tag, unsigned int corners, 
    float x, float y, float scale, float rotation, 
    float r, float g, float b, float a, 
    ShapeType type)
{
    Shape* shape = (Shape*)malloc(sizeof(Shape));
    if (shape == NULL) 
    {
        return NULL; // Failed to allocate memory
    }

    strncpy(shape->tag, tag, TAGSIZE);

    shape->corners = corners;
    shape->pos[0] = x;
    shape->pos[1] = y;
    shape->scale = scale;
    shape->rotation = rotation;

    shape->colour[0] = r;
    shape->colour[1] = g;
    shape->colour[2] = b;
    shape->colour[3] = a;

    shape->type = type;

    return shape;
}