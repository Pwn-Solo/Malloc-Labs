#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

team_t team = {
    /* Team name */
    " ",
    /* First member's full name */
    "PwnSolo",
    /* First member's email address */
    " ",
    /* Second member's full name (leave blank if none) */
    " ",
    /* Second member's email address (leave blank if none) */
    " "
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t))) 

struct block{
    size_t data; //(size + free)
    struct block* next; 

};

int set_allocated(int used_chunks) //no of chunks + 0x1 (used)
{
    return used_chunks*0x10 + 0x1;
}

void set_free(int *head_ptr)       //no of chunks + 0x0 (free)
{
    *head_ptr = *head_ptr - 0x1;
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */\

void *mm_malloc(size_t size)
{
    int newsize = ALIGN(size);
    struct block* head = NULL;

    int used_chunks = newsize/4;        //1 chunk = 4 bytes (32-bit), 8 bytes (64-bit) 

    head = mem_sbrk(sizeof(struct block));
    
    head->data = set_allocated(used_chunks);
    
    void *payload = mem_sbrk(newsize);
    head->next = mem_sbrk(0);           // pts to the header of the next block

    if (payload == (void *)-1)
        return NULL;
    else 
        return payload;
    
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    int *head_ptr = ptr - 8;
    if ((*head_ptr)%2==0)
        printf("Block is not allcated yet"); // if block is not allocated ; dont free
    
    else{
        set_free(head_ptr);
    }
}
/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}














