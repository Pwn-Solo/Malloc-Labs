#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

struct block{
	size_t size;
	int free;	
};

void *allocate(size_t size){
   	int allocate_size;
	allocate_size = size + 8-(size%8); //aligns size to be divisible by 8

	printf("%d\n",allocate_size);
	void *payload_ptr =NULL;
	int *header_ptr = NULL;

	struct block head;     //header 
	head.size = size;
	head.free = 1;

	printf("%p\n",sbrk(0));
	header_ptr = sbrk(sizeof(struct block));
	*header_ptr = head.size; 
	*(header_ptr+2) = head.free;

	printf("%p\n",sbrk(0));
	payload_ptr = sbrk(allocate_size);

	printf("%p\n",sbrk(0));
	if (!payload_ptr){
		printf("failed to allocate size %d\n", allocate_size);
	}

	return payload_ptr;
}

void main(){
	char *ptr,*ptr2=NULL;
	int size,size2;
	puts("enter size to allocate");
	scanf("%d",&size);
	ptr = (char*)allocate(size * sizeof(char));
	printf("Enter String ");
	scanf("%s",ptr);
}
