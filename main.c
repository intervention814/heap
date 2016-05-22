/*
 * main.c
 *
 *  Created on: May 21, 2016
 *      Author: Freeman
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api_heap.h"

// Struct defining what each heap element looks like in the manipulate list view...
typedef struct HeapListElement {
	int block_id;
	const char* ptr_heap;
	struct HeapListElement* next;
} AllocHeapListElement;

// Prototypes for functions
void handle_alloc();
void handle_free();
void handle_view();
void list_add(const char* ptr);
void list_delete(const char* ptr);

// Head of the heap list...
AllocHeapListElement* allocdHeapList = NULL;

int main(int argc, char** argv)
{
	char command[256];

	// Init the heap
	if (api_init(1000)) {
		printf("Heap initialized.\r\n");
	} else {
		printf("Unable to init heap.\r\n");
		return 1;
	}

	while (1)
	{
		printf("Enter command: ");
		if (scanf("%s", command) != 1) {
			printf("Unable to read command.\r\n");
			continue;
		}
		if (strcmp(command, "alloc") == 0)
		{
			printf("Handling alloc...\r\n");
			handle_alloc();
		}
		if (strcmp(command, "free") == 0) {
			printf("Handling free...\r\n");
			handle_free();
		}
		if (strcmp(command, "view") == 0) {
			printf("Handling view...\r\n");
			handle_view();
		}
		if (strcmp(command, "q") == 0) {
			break;
		}
	}
}

void handle_free()
{
	int block_id_to_free;
	if (scanf("%d", &block_id_to_free) != 1) {
		printf("Unable to read block ID to free.\r\n");
		return;
	}
	printf("Freeing %d...\r\n", block_id_to_free);
	list_delete(block_id_to_free);
}

void handle_alloc()
{
	char* ptr = NULL;
	size_t len_to_alloc;

	if (scanf("%lu", &len_to_alloc) != 1) {
		printf("Unable to read len_to_alloc\r\n");
		return;
	}

	printf("Allocating %lu bytes...\r\n", len_to_alloc);
	ptr = api_alloc(len_to_alloc);

	// Add the ptr to the heap list...
	list_add(ptr);
}

void handle_view()
{
	printf("Viewing heap...\r\n");
	api_view();
}

void list_add(const char* ptr) {
	// Insert ptr into heap list
	if (allocdHeapList == NULL) {
		allocdHeapList = malloc(sizeof(AllocHeapListElement));
		allocdHeapList->next = NULL;
		allocdHeapList->ptr_heap = ptr;
		return;
	}

	AllocHeapListElement* cur = allocdHeapList;
	while (cur->next != NULL) {
		cur = cur->next;
	}

	cur->next = malloc(sizeof(AllocHeapListElement));
	cur = cur->next;
	cur->next = NULL;
	cur->ptr_heap = ptr;
}

void list_delete(const char* ptr) {
	AllocHeapListElement* cur = allocdHeapList;
	AllocHeapListElement* prev = NULL;

	// If we have no list, ptr won't be in it...
	if (allocdHeapList == NULL) {
		printf("No list yet.\r\n");
		return;
	}

	// Find the ptr in our list
	while (cur->ptr_heap != ptr){
		if (cur->next != NULL) {
			prev = cur;
			cur = cur->next;
		} else {
			printf("Ptr %p not in list.\r\n", ptr);
			return;
		}
	}

	// Fix our prev ptr to point to our next, since 'we' are being freed.
	if (prev != NULL) {
		prev->next = cur->next;
	}

	free(cur);
}
