#include <stdio.h> 
#include <stdlib.h> 
#include <limits.h> 
  
// A structure to represent a queue 
struct MsgQueue {
    int First, Last, MsgSize; 
    unsigned MsgCapacity; 
    int* array; 
}; 
  
  
struct MsgQueue* createQueue(unsigned MsgCapacity) { 
    struct MsgQueue* queue = (struct MsgQueue*) malloc(sizeof(struct MsgQueue)); 
    queue->MsgCapacity = MsgCapacity; 
    queue->First = queue->MsgSize = 0;  
    queue->Last = MsgCapacity - 1;  // This is important, see the enqueue 
    queue->array = (int*) malloc(queue->MsgCapacity * sizeof(int)); 
    return queue; 
} 
  
// MsgQueue is full when MsgSize becomes equal to the MsgCapacity  
int isFull(struct MsgQueue* queue) {  
    return (queue->MsgSize == queue->MsgCapacity);  
} 
  
// MsgQueue is empty when MsgSize is 0 
int isEmpty(struct MsgQueue* queue) {
    return (queue->MsgSize == 0);
} 
  
// Function to add an item to the queue.   

void enqueue(struct MsgQueue* queue, int item) { 
    if (isFull(queue)) 
        return; 
    queue->Last = (queue->Last + 1)%queue->MsgCapacity; 
    queue->array[queue->Last] = item; 
    queue->MsgSize = queue->MsgSize + 1; 
    // printf("%d enqueued to queue\n", item); 
} 
  
// Function to remove an item from queue.  
// It changes First and MsgSize 
int dequeue(struct MsgQueue* queue) { 
    if (isEmpty(queue)) 
        return INT_MIN; 
    int item = queue->array[queue->First]; 
    queue->First = (queue->First + 1)%queue->MsgCapacity; 
    queue->MsgSize = queue->MsgSize - 1; 
    return item; 
} 
  
// Function to get First of queue 
int First(struct MsgQueue* queue) { 
    if (isEmpty(queue)) 
        return INT_MIN; 
    return queue->array[queue->First]; 
} 
  
// Function to get Last of queue 
int Last(struct MsgQueue* queue) { 
    if (isEmpty(queue)) 
        return INT_MIN; 
    return queue->array[queue->Last]; 
}