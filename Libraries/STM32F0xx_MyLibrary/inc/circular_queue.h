#ifndef CIRCULAR_QUEUE
#define CIRCULAR_QUEUE

void InitQueue(void);
signed short int QueueElementSize(void);
unsigned char Enqueue(signed long int *pDataIn);
unsigned char Dequeue(signed long int *pDataOut);

#endif /* CIRCULAR_QUEUE */
