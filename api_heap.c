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
		printf("Addr: [%p] : Size: [%lu] (header size [%lu]) : Next: [%p] : Prev: [%p]\r\n",
				cur,
				cur->data_size,
				sizeof(HeapHeader),
				cur->next,
				cur->prev);
		cur = cur->next;
	}
}

void* api_alloc(size_t size) {
	HeapHeader* cur = (HeapHeader*)g_heap_ptr;

	// If we only have one block...
	if (cur->next == NULL) {
		if (cur->data_size >= (size + sizeof(HeapHeader))) {
			printf("One block available of right size...\r\n");
			// Update the size of this chunk taking into consideration that
			// there is a new header, for the new block being allocated, taking up space now.
			cur->data_size -= (size + sizeof(HeapHeader));

			// Create a new header for the return
			HeapHeader* newHeaderRet = (HeapHeader*)((char*)(cur + size + sizeof(HeapHeader)));
			newHeaderRet->data_size = size;
			newHeaderRet->next = NULL;
			newHeaderRet->prev = NULL;
			//g_heap_ptr = (char*)newHeaderRet;

			printf("Created new 'only block left' at %p (size %lu)\r\n", g_heap_ptr, newHeaderRet->data_size);
			return (char*)newHeaderRet + sizeof(HeapHeader);
		}
	}


	return NULL;
}

void api_free(void* ptr) {
	return;
}

size_t api_get_size(const unsigned char* ptr) {
	HeapHeader* hdr = (HeapHeader*)(ptr - sizeof(HeapHeader));
	return hdr->data_size;
}


