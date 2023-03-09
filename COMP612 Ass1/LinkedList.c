#include "LinkedList.h"
#include "Shape.h"
#include <stdlib.h>
#include <string.h> // dangerous territory, we are stepping into.

Shape* find(LinkedList* list, char* string)
{
	// iterate through mr linkedlist
	// for each node, check the tag
	for (Node* current = list->head; current != NULL; current = current->next)
	{
		if (!strcmp(current->shape_ptr->tag, string))
		{
			// equal
			return current->shape_ptr;
		}
	}

	return NULL;
}

// Initializes a new, empty linked list
LinkedList* new_ll(void)
{
	LinkedList* ll = (LinkedList*)malloc(sizeof(LinkedList));

	ll->head = NULL;
	ll->tail = NULL;

	ll->size = 0;

	return ll;
}

void insert_front(LinkedList* list, Shape* shape)
{
	// let's get a new node going
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->next = list->head;
	new_node->shape_ptr = shape;

	if (list->size == 0)
		list->tail = new_node;

	list->head = new_node;
	list->size++;
}

void insert_back(LinkedList* list, Shape* shape)
{
	// let's get a new node going
	Node* new_node = (Node*)malloc(sizeof(Node));
	new_node->next = NULL;
	new_node->shape_ptr = shape;

	if (list->size == 0) // Empty list, head and tail point to new node
	{
		list->head = new_node;
		list->tail = new_node;
	}
	else // add new node to tail and update the previous next pointer
	{
		list->tail->next = new_node;
		list->tail = new_node;
	}

	list->size++;
}