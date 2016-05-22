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
	char is_in_use; // is allocated?
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
	hdr->is_in_use = 0;
	hdr->next = hdr->prev = NULL;
	return 1;
}

void api_view() {
	if (g_heap_ptr == NULL) {
		printf("Heap uninitialized.\r\n");
		return;
	}
	HeapHeader* cur = (HeapHeader*)g_heap_ptr;
	while (cur != NULL) {
		printf("Addr: [%p] : Size: [%lu] : In Use: [%d] : Next: [%p] : Prev: [%p]\r\n",
				cur,
				cur->data_size,
				cur->is_in_use,
				cur->next,
				cur->prev);
		cur = cur->next;
	}
}

char* api_alloc(size_t size) {
	HeapHeader* cur = (HeapHeader*)g_heap_ptr;
	while ((cur->data_size < (size + sizeof(HeapHeader))) || cur->is_in_use == 1) {
		cur = cur->next;
		if (cur == NULL) {
			printf("No blocks large enough for %lu bytes.\r\n", size);
			return NULL;
		}
	}
	// Do fragment...
	size_t size_left_in_block_after_alloc = cur->data_size - (size + sizeof(HeapHeader));
	return NULL;
}

void api_free(const char* ptr) {
	return;
}

size_t api_get_size(const char* ptr) {
	return 0;
}


