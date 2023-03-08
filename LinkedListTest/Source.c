#include "LinkedList.h"
#include "Shape.h"
#include <stdio.h>

int main(void)
{
	Shape* s = new_shape("a", 4, 1, 1, 1, 1, 1, 1, 1, 1);
	Shape* s2 = new_shape("b", 4, 1, 1, 1, 1, 1, 1, 1, 1);
	Shape* s3 = new_shape("c", 4, 1, 1, 1, 1, 1, 1, 1, 1);
	LinkedList* ll = new_ll();

	insert_back(ll, s);
	insert_back(ll, s2);
	insert_back(ll, s3);

	Node* current = ll->head;
	while (current != NULL)
	{
		printf("%s\n", current->shape_ptr->tag);

		current = current->next;
	}

	printf("Shape s2 at address %p\n", s2);
	printf("LinkedList found address for shape s2: %p", find(ll, "b"));

	return 0;
}