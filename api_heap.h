/*
 * api_heap.h
 *
 *  Created on: May 21, 2016
 *      Author: user
 */

#ifndef API_HEAP_H_
#define API_HEAP_H_

typedef struct HeapHeader {
	size_t data_size;
	struct HeapHeader* next;
} HeapHeader;

int api_init(size_t underlying_heap_size);
void api_view();
void* api_alloc(size_t size);
void api_free(void* ptr);
size_t api_get_size(const unsigned char* ptr);
HeapHeader* api_get_prev_coalesc_block(HeapHeader* hdr);
void api_coallesce_prev(HeapHeader* prev, HeapHeader* hdr);

#endif /* API_HEAP_H_ */
