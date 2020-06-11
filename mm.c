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
    size_t data;  //(size + free)
    struct block* next; 
};

int *start_list,*end_list=NULL;

int set_allocated(int used_chunks) //no of chunks + 0x1 (used)
{
    return used_chunks*0x10 + 0x1;
}

void set_free(int *head_ptr)       //no of chunks + 0x0 (free)
{
    *head_ptr = *head_ptr - 0x1;
}

void *traverse_free_list(int newsize) //search for space in the free list
{
    int *temp = start_list;
     
    while((void *)temp != NULL){
        int data = *temp;
        if (data%2==0)  //if block is free
            printf("data = %p\n",data); // block data 
        
        temp = temp+1;
        int next = (void *)temp;
        printf("next = %p\n",next); // ptr to next block 
        temp = *temp;

    }
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    start_list = mem_sbrk(0);  //pts to the start of the allocated chunk list
    end_list = NULL; 
    return 0;
}
/* 
 * mm_malloc - Allocate a block by incr0xf60x000000819bd010ementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{

    int newsize = ALIGN(size);
    struct block* head = NULL;

    int used_chunks = newsize/4;        //1 chunk = 4 bytes (32-bit), 8 bytes (64-bit) 

    traverse_free_list(10);

    head = mem_sbrk(sizeof(struct block));
    
    head->data = set_allocated(used_chunks);
    
    void *payload = mem_sbrk(newsize);
    head->next = mem_sbrk(0);          // pts to the header of the next block7/

    if (payload == (void *)-1)
        return NULL;
    else{
        end_list = mem_sbrk(0);
        return payload;
    }
    
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