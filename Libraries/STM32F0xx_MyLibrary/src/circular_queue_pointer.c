#include "circular_queue.h"
#include "rtt_log.h"
#include <string.h>

#define LOCK_FREE 0

#define PAGE_SIZE   	2
#define PAGE_NUMBER 	4
#define QbufferSize 	PAGE_SIZE*PAGE_NUMBER

signed long int Qbuffer[QbufferSize];		// Qbuffer storage
signed long int *EnQptr;	// enqueue pointer
signed long int *DeQptr;	// dequeue pointer
signed long int *SOBptr;    // pointer to start of the buffer
signed long int *EOBptr;	// pointer to end of the buffer

signed short int inPAGE = 0;

// Initialization function does not return any value.
void InitQueue(void)
// Initializes enqueue and dequeue pointers.
{
	EnQptr = &Qbuffer[0];
	DeQptr = &Qbuffer[0];
	SOBptr = &Qbuffer[0];
	EOBptr = SOBptr + QbufferSize;
}

unsigned char Enqueue(signed long int *pDataIn)
/*	Stores an item into the queue buffer.
		pDataIn points to the data to be queued.
		Return values are:
		0 => enqueue operation is sucessful.
		1 => buffer overrun - no space available in the buffer
*/
{
	signed long int *TempQptr;	// Temporary pointer storage
	
	TempQptr = EnQptr;	// Save current enqueue pointer
	EnQptr++; 	// Increment enqueue pointer
	if(EnQptr == EOBptr) // Check if reached end of buffer
	{
		EnQptr = SOBptr;		// Go back to the first location (SOBptr)
	}
	
	#if LOCK_FREE
	if(EnQptr == (SOBptr + (inPAGE*PAGE_SIZE)))
	{
		logi("Buffer overflow!");
		memset(EnQptr, 0xFFFFFFFF, PAGE_SIZE*sizeof(signed long int)); // Clear Qbuffer

		if( (DeQptr >= (SOBptr + inPAGE*PAGE_SIZE)) &&
		    (DeQptr < (SOBptr + ((inPAGE+1)*PAGE_SIZE))) )
		{
			DeQptr = SOBptr + ((inPAGE+1)*(PAGE_SIZE));
			if(DeQptr == EOBptr)
				DeQptr = SOBptr;
		}
		inPAGE++;
		if(inPAGE == PAGE_NUMBER)
			inPAGE = 0;
	}	

	*TempQptr = *pDataIn; 	// Copy input data to buffer
	return (unsigned char) 0; 	// Enqueue operation is successful

	#else
	if(EnQptr == DeQptr)	// Check for buffer overrun
	{
		EnQptr = TempQptr;		// Go back to the first location
		return (unsigned char) 1;		// buffer overrun
	}
	else
	{
		*TempQptr = *pDataIn; 	// Copy input data to buffer
		return (unsigned char) 0; 	// Enqueue operation is successful
	}
	#endif
} // End of function EnQueue

unsigned char Dequeue(signed long int *pDataOut)
/*	Retrieves an item from the queue buffer.
		Retrieved item is strored at the location pointed to by pDataOut.
		Return values are:
		0 => dequeue operation is successful
		1 => there are no queue items
*/
{
	if(DeQptr == EnQptr)
	{
		*pDataOut = 0xFFFFFFFF;
		return (unsigned char) 1;		// no items available in the queue
	}
	else
	{
		*pDataOut = *DeQptr;		// copy data to the output location
		DeQptr++;								// increment dequeue pointer
		if(DeQptr == EOBptr)		// check if reached end of buffer
			DeQptr = &Qbuffer[0];		// go back to the first location

		return (unsigned char) 0;		// dequeue operation is successful
	}
}	// End of function DeQueue
