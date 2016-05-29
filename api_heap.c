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

unsigned char* g_heap_ptr;

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
	//hdr->prev = NULL;
	return 1;
}

void api_view() {
	if (g_heap_ptr == NULL) {
		printf("Heap uninitialized.\r\n");
		return;
	}
	HeapHeader* cur = (HeapHeader*)g_heap_ptr;
	while (cur != NULL) {
		printf("Addr: [%p] : Size: [%lu] (header size [%lu]) : Next: [%p]\r\n",
				cur,
				cur->data_size,
				sizeof(HeapHeader),
				cur->next);
				//cur->prev);
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
			HeapHeader* newHeaderRet = (HeapHeader*)((char*)(cur + cur->data_size + sizeof(HeapHeader)));
			newHeaderRet->data_size = size;
			newHeaderRet->next = NULL;

			//if (newHeaderRet == (HeapHeader*)g_heap_ptr) {
			//	printf("Using entire free list block!\r\n");
			//	g_heap_ptr = NULL;
			//}

			printf("Created new 'from only block left' hdr at %p (size %lu)\r\n", (char*)newHeaderRet, newHeaderRet->data_size);
			return (char*)newHeaderRet + sizeof(HeapHeader);
		}
	}


	return NULL;
}

void api_free(void* ptr) {
	HeapHeader* hdr = (HeapHeader*)(ptr - sizeof(HeapHeader));
	if (g_heap_ptr == NULL) {
		printf("Free list is empty, using free'd ptr as head of freelist.\r\n");
		g_heap_ptr = (unsigned char*)hdr;
		return;
	}
	printf("Freeing block of size: [%lu] at [%p]\r\n", hdr->data_size, (unsigned char*)hdr);

	HeapHeader* prevContigousBlock = api_get_prev_coalesc_block(hdr);
	if (prevContigousBlock == NULL) {
		printf("Cannot coalesce free'd ptr with prev\r\n");
	}
	else {
		printf("Coalescing prev block at [%p] (size [%lu], hdr [%lu]) "
				"with free'd block hdr at [%p]\r\n",
				prevContigousBlock,
				prevContigousBlock->data_size,
				sizeof(HeapHeader),
				hdr);
		api_coallesce_prev(prevContigousBlock, hdr);
	}
	return;
}

void api_coallesce_prev(HeapHeader* prev, HeapHeader* hdr) {
	if (prev->next == NULL) {
		printf("Prev block's next is NULL\r\n");
		prev->data_size += sizeof(HeapHeader) + hdr->data_size;
		return;
	}
}

HeapHeader* api_get_prev_coalesc_block(HeapHeader* hdr) {
	if (g_heap_ptr == NULL) {
		printf("No blocks in free list\r\n");
		return NULL;
	}
	HeapHeader* cur = (HeapHeader*)g_heap_ptr;
	while (cur != NULL) {
		unsigned char* nextAddress = (unsigned char*)(cur + cur->data_size + sizeof(HeapHeader));
		if (nextAddress == (unsigned char*)hdr) {
			printf("Found heap block in free list contiguously "
					"before [%p] beginning at [%p], size [%lu]\r\n",
					hdr,
					cur,
					cur->data_size);
			return cur;
		}
		cur = cur->next;
		if (cur == (HeapHeader*)g_heap_ptr) {
			printf("We've come back to the start of the free list without finding a block to coalesce with.\r\n");
			break;
		}
	}
	return NULL;
}

size_t api_get_size(const unsigned char* ptr) {
	HeapHeader* hdr = (HeapHeader*)(ptr - sizeof(HeapHeader));
	return hdr->data_size;
}


