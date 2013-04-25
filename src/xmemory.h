/*
 * xmemory.h
 *
 *  Created on: Apr 25, 2013
 *      Author: liyajie1209
 */

#ifndef XMEMORY_H_
#define XMEMORY_H_


#include <stdio.h>

void * xmalloc(size_t size);

void xfree(void * ptr);

#endif /* XMEMORY_H_ */
