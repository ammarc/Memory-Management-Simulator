/* * * * * * *
 * Module for creating and manipulating singly-linked lists of integers
 *
 * created for COMP20007 Design of Algorithms 2017
 * by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "list.h"

typedef struct node Node;

// a list node points to the next node in the list, and to some data
struct node {
	Node* next;
	Node* prev;
    void* data;
};

// a list points to its first and last nodes, and stores its size (num. nodes)
struct list {
	Node *head;
	Node *last;
	int size;
};

// helper function to create a new node and return its address
Node *new_node();

// helper function to clear memory of a node (does not free the node's data)
void free_node(Node *node);


/* * * *
 * FUNCTION DEFINITIONS
 */

// create a new list and return a pointer to it
List *new_list() {
	List *list = malloc(sizeof *list);
	assert(list);
	
	list->head = NULL;
	list->last = NULL;
	list->size = 0;

	return list;
}

// destroy a list and free its memory
void free_list(List *list) {
	assert(list != NULL);
	// free each node
	Node *node = list->head;
	Node *next;
	while (node) {
		next = node->next;
		free_node(node);
		node = next;
	}
	// free the list struct itself
	free(list);
}

// helper function to create a new node and return its address
Node *new_node()
{
	Node *node = malloc(sizeof *node);
	assert(node);
	
	return node;
}

// helper function to clear memory of a node
void free_node(Node *node)
{
	free(node);
}

// add an element to the front of a list
// this operation is O(1)
/*
void list_add_start(List *list, void* data)
{
	assert(list);
    
	// create and initialise a new list node
	Node *node = new_node();
    
	node->data = data;

	node->next = list->head;
    
    node->prev = NULL;
    
    // if the list is empty then add a new node to the head
    if (list->head == NULL)
    {
        list->head = new_node();
    }

    // seg fault down here:
    // fprintf(stderr, "Head's prev is: %p\n\n", list->head);
    // list->head->prev = node;
    // fprintf(stderr, "Good uptill now ...\n\n");
    // next will be the old first node (may be null)

	// place it at the start of the list
	list->head = node;

	// if list was empty, this new node is also the last node now
    // modified for the doubly linked list
	if(list->size == 0)
    {
		list->last = node;
	}
    else
    {
        list->head->prev = node;
    }

	// and we need to keep size updated!
	list->size++;
}
*/

// add an element to the back of a list
// this operation is O(1)
void list_add_end(List* list, void* data)
{
	assert(list != NULL);

	// we'll need a new list node to store this data
	Node* node = new_node();

	node->data = data;
	node->next = NULL;  // as the last node, there's no next node
    //node->prev = list->last;
    

	if(list->size == 0)
    {
		// if the list was empty, new node is now the first node
		list->head = node;
        list->head->prev = NULL;
	}
    else
    {
		// otherwise, it goes after the current last node
		list->last->next = node;
        node->prev = list->last;
	}
	
	// place this new node at the end of the list
	list->last = node;
	
	// and keep size updated too
	list->size++;
}

// remove and return the front data element from a list
// this operation is O(1)
// error if the list is empty (so first ensure list_size() > 0)
void* list_remove_start(List *list)
{
	assert(list != NULL);
	assert(list->size > 0);
	
    // we'll need to save the data to return it
	Node *start_node = list->head;
	// int *list_data = malloc(sizeof(int) * 4);
    void* list_data = malloc(sizeof(list->head->data));
	
	list_data = list->head->data;

	list->head = list->head->next;
    

	// if this was the last node in the list, the last also needs to be cleared
	if(list->size == 1)
    {
		list->last = NULL;
	}
    else
    {
        list->head->prev = NULL;
    }

	// the list is now one node smaller
	list->size--;
    
	// and we're finished with the node holding this data
	free_node(start_node);

	// done!
	return list_data;
}

// remove and return the final data element in a list
// this operation is O(n), where n is the number of elements in the list
// error if the list is empty (so first ensure list_size() > 0)
/*
void* list_remove_end(List *list)
{
	assert(list != NULL);
	assert(list->size > 0);

    // making a variable to store int array to return
    void* list_data = malloc(sizeof(list->last->data));;
	
	// we'll need to save the data to return it
	Node *end_node = list->last;

	list_data = end_node->data;
	
	// then replace the last with the second-last node (may be null)
	// (to find this replacement, we'll need to walk the list --- the O(n) bit
	Node *node = list->head;

	Node *prev = NULL;
	while (node->next) {
		prev = node;
		node = node->next;
	}
	list->last = prev;
	
	if(list->size == 1) {
		// if we're removing the last node, the head also needs clearing
		list->head = NULL;
	} else {
		// otherwise, the second-last node needs to drop the removed last node
		prev->next = NULL;
	}

	// the list just got one element shorter
	list->size--;

	// we're finished with the list node holding this data
	free_node(end_node);

	// done!
	return list_data;
}
*/
// return the number of elements contained in a list
int list_size(List *list) {
	assert(list != NULL);
	return list->size;
}

// returns whether the list contains no elements (true) or some elements (false)
bool list_is_empty(List *list) {
	assert(list != NULL);
	return (list->size==0);
}