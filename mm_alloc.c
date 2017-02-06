/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines.
 */

#include "mm_alloc.h"
#include <stdlib.h>


static size_t bookkeeping = sizeof(struct block);
// static size_t bookkeeping = 40;

struct block *header = NULL;
struct block *footer;

void *mm_malloc(size_t size) {
    /* YOUR CODE HERE */
    if (!size)
    {
        return NULL;
    }
    if(header == NULL){
        if((header = sbrk(bookkeeping)) == -1) return NULL;
        header->prev = NULL;
        header->next = NULL;
        header->size = size;
        header->free = 0;
        if( (header->ptr = sbrk(size)) == -1) return NULL;
        memset(header->ptr, 0, size);
        footer = header;
        return header->ptr;
    }
    else {
        struct block *chunk = header;
        while(chunk){
            if(chunk->free && chunk->size >= size){
                size_t ss = chunk->size - size;
                chunk->free = 0;
                chunk->size = size;
                if (ss > 0){
                    struct block *temp;
                    if((temp = sbrk(bookkeeping)) == -1) return NULL;
                    temp->prev = footer;
                    footer->next = temp;
                    footer = temp;
                    temp->next = NULL;
                    temp->size = ss;
                    temp->ptr = chunk->ptr + size;
                    temp->free = 1;
                }
                memset(chunk->ptr, 0, size);
                return chunk->ptr;
            }
            chunk = chunk->next;
        }
        chunk = footer;
        struct block *end;
        if((end = sbrk(bookkeeping)) == -1) return NULL;
        end->prev = chunk;
        end->next = NULL;
        chunk->next = end;
        end->free = 0;
        end->size = size;
        if((end->ptr = sbrk(size)) == -1) return NULL;
        footer = end;
        chunk = end;
        memset(chunk->ptr, 0, size);
        return chunk->ptr;
    }

    // return NULL;
}

void *mm_realloc(void *ptr, size_t size) {
    /* YOUR CODE HERE */
    // return NULL;
    if(!size) return NULL;
    if(!ptr) return mm_malloc(size);
    struct block *cur = ptr - bookkeeping;
    if(cur->size >= size){
        void *new = mm_malloc(size);
        memcpy(new, ptr, size);
        mm_free(ptr);   
        return new;
    }
    else
    {   
        void *new = mm_malloc(size);
        if(new){
            memcpy(new, ptr, cur->size);
            mm_free(ptr);
            return new;
        }
        else return NULL;
    }
}

void mm_free(void *ptr) {
    /* YOUR CODE HERE */
    if (!ptr) return;
    struct block *temp = ptr - bookkeeping;
    temp->free = 1;
    memset(temp->ptr, 0, temp->size);
    if (temp->prev && temp->prev->free){
        struct block *prev = temp->prev;
        prev->size = prev->size + bookkeeping + temp->size;
        if(temp->next) prev->next = temp->next;
        if(footer == temp) footer = prev;
        temp = prev;
        memset(temp->ptr, 0, temp->size);

    }
    if (temp->next && temp->next->free){
        struct block *next = temp->next;
        temp->size = temp->size + bookkeeping + next->size;
        temp->next = next->next;
        next->prev = temp;
        if(footer == next) footer = temp;
        memset(temp->ptr, 0, temp->size);
    }
    return;

}




