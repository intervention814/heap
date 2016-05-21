/*
 * api_heap.c
 *
 *  Created on: May 21, 2016
 *      Author: user
 */

#include <stdio.h>
#include <stdlib.h>
#include "api_heap.h"

char* g_heap_ptr;

int api_init(size_t underlying_heap_size) {
	g_heap_ptr = malloc(underlying_heap_size);
	if (g_heap_ptr == NULL) {
		return 0;
	}
	memset(g_heap_ptr, 0, underlying_heap_size);
	return 1;
}

void api_view() {
	if (g_heap_ptr == NULL) {
		printf("Heap uninitialized.\r\n");
		return;
	}
}

char* api_alloc(size_t size) {
	return NULL;
}

void api_free(char* ptr) {
	return;
}


