#include "LinkedList.h"
#include "Shape.h"
#include <stdlib.h>

typedef struct _node
{
	Shape* shape_ptr;
	Node* next;
} Node;