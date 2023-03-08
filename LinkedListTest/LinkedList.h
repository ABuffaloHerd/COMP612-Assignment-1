// Linkedlist v1
#pragma once
#include "Shape.h"
typedef struct _node
{
	Shape* shape_ptr;
	struct _node* next;
} Node;

typedef struct _ll
{
	struct _node* head;
	struct _node* tail; // only because this needs to be insertable at the ass end

	unsigned int size;
} LinkedList;

LinkedList* new_ll(void);
void insert_front(LinkedList* list, Shape* shape);
void insert_back(LinkedList* list, Shape* shape);
Shape* find(LinkedList* list, char* string);