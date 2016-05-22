/*
 * api_heap.c
 *
 *  Created on: May 21, 2016
 *      Author: user
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api_heap.h"

char* g_heap_ptr;

typedef struct HeapHeader {
	size_t data_size;
	struct HeapHeader* next;
	struct HeapHeader* prev;
} HeapHeader;

int api_init(size_t underlying_heap_size) {
	printf("Initializing heap with %lu bytes\r\n", underlying_heap_size);
	g_heap_ptr = malloc(underlying_heap_size + sizeof(HeapHeader));
	if (g_heap_ptr == NULL) {
		return 0;
	}
	memset(g_heap_ptr, 0, underlying_heap_size);
	HeapHeader* hdr = (HeapHeader*)g_heap_ptr;
	hdr->data_size = underlying_heap_size;
	hdr->next = NULL;
	hdr->prev = NULL;
	return 1;
}

void api_view() {
	if (g_heap_ptr == NULL) {
		printf("Heap uninitialized.\r\n");
		return;
	}
	HeapHeader* cur = (HeapHeader*)g_heap_ptr;
	while (cur != NULL) {
		printf("Addr: [%p] : Size: [%lu] : Next: [%p] : Prev: [%p]\r\n",
				cur,
				cur->data_size,
				cur->next,
				cur->prev);
		cur = cur->next;
	}
}

char* api_alloc(size_t size) {
	HeapHeader* cur = (HeapHeader*)g_heap_ptr;

	// If we only have one block...
	if (cur->next == NULL) {
		if (cur->data_size >= (size + sizeof(HeapHeader))) {
			printf("One block available of right size...\r\n");
			HeapHeader* newHeader = (HeapHeader*)((char*)(cur + size + sizeof(HeapHeader)));
		}
	}

	while (cur->data_size < (size + sizeof(HeapHeader))) {
		cur = cur->next;
		if (cur == NULL) {
			printf("No blocks large enough for %lu bytes.\r\n", size);
			return NULL;
		}
	}
	// Do fragment...
	return NULL;
}

void api_free(const char* ptr) {
	return;
}

size_t api_get_size(const char* ptr) {
	return 0;
}


