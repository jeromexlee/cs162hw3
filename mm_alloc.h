/*
 * mm_alloc.h
 *
 * Exports a clone of the interface documented in "man 3 malloc".
 */

#pragma once

#include <stdlib.h>


void *mm_malloc(size_t size);
void *mm_realloc(void *ptr, size_t size);
void mm_free(void *ptr);

/*struct of block*/
struct block {
    struct block *prev;
    struct block *next;
    int free;
    void *ptr;
    size_t size;
    char data [0];
};