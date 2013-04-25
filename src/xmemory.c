/*
 * xmemory.c
 *
 *  Created on: Apr 25, 2013
 *      Author: liyajie1209
 */

#include <stdlib.h>

#include "xmemory.h"


void * xmalloc(size_t size) {
	void * ptr = NULL;
	if (size > 0) {
		ptr = malloc(size);
	}
	return ptr;
}

void xfree(void * ptr) {
	if(ptr == NULL)
		return;
	free(ptr);
	ptr = NULL;
}
