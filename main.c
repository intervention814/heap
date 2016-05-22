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
void handle_view_api();
void list_add(const char* ptr);
void list_delete(int block_to_delete);
void list_view();

// Head of the heap list...
AllocHeapListElement* allocated_heap_blocks= NULL;
static int allocd_heap_block_id = 0;

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
		else if (strcmp(command, "alloc") == 0)
		{
			printf("Handling alloc...\r\n");
			handle_alloc();
		}
		else if (strcmp(command, "free") == 0) {
			printf("Handling free...\r\n");
			handle_free();
		}
		else if (strcmp(command, "viewlist") == 0) {
			printf("Handling view...\r\n");
			handle_view();
		}
		else if (strcmp(command, "viewapi") == 0) {
			printf("Handling view API...\r\n");
			handle_view_api();
		}
		else if (strcmp(command, "q") == 0) {
			break;
		} else {
			printf("Available commands are: viewapi, viewlist, alloc, free, q\r\n");
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
	if (ptr == NULL) {
		printf("Unable to alloc %lu bytes.\r\n", len_to_alloc);
		return;
	}

	// Add the ptr to the heap list...
	list_add(ptr);
}

void handle_view()
{
	list_view();
}

void handle_view_api() {
	api_view();
}

void list_add(const char* ptr) {
	// Insert ptr into heap list
	AllocHeapListElement* cur = allocated_heap_blocks;
	if (allocated_heap_blocks == NULL) {
		allocated_heap_blocks = malloc(sizeof(AllocHeapListElement));
		allocated_heap_blocks->next = NULL;
		allocated_heap_blocks->ptr_heap = ptr;
		allocated_heap_blocks->block_id = allocd_heap_block_id++;
		return;
	}

	// Advance list until we find position for new element
	while (cur->next != NULL) {
		cur = cur->next;
	}

	cur->next = malloc(sizeof(AllocHeapListElement));
	cur = cur->next;
	cur->next = NULL;
	cur->ptr_heap = ptr;
	cur->block_id = allocd_heap_block_id++;
}

void list_delete(int block_to_delete) {
	AllocHeapListElement* cur = allocated_heap_blocks;
	AllocHeapListElement* prev = NULL;

	// If we have no list, ptr won't be in it...
	if (allocated_heap_blocks == NULL) {
		printf("No list yet.\r\n");
		return;
	}

	// Find the ptr in our list
	while (cur->block_id != block_to_delete){
		if (cur->next != NULL) {
			prev = cur;
			cur = cur->next;
		} else {
			printf("Block id %d not in list.\r\n", block_to_delete);
			return;
		}
	}

	// Fix our prev ptr to point to our next, since 'we' are being freed.
	if (prev != NULL) {
		prev->next = cur->next;
	}

	// Call the api_free function...
	api_free(cur->ptr_heap);

	free(cur);
}

void list_view() {
	AllocHeapListElement* cur = allocated_heap_blocks;

		printf("Viewing heap...\r\n");
		if (cur == NULL) {
			printf("No items allocated.\r\n");
			return;
		}

		while (cur != NULL) {
			printf("Block ID: %d\r\n", cur->block_id);
			printf("Size: %lu\r\n", api_get_size(cur->ptr_heap));
			cur = cur->next;
		}
}
