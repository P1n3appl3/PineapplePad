#ifndef __FIFO_H__
#define __FIFO_H__
#include <stdint.h>

#define FIFOSIZE 32  // maximum storage is FIFO_SIZE-1 elements
class Queue{
private:
	char buf[FIFOSIZE];
	int front; // index to an empty place, next place to put
    int back; // index to oldest data, next to get

public:
  Queue();            // initialize queue
  bool IsEmpty(void); // true if empty
  bool IsFull(void);  // true if full
  bool Put(char x);   // enter data into queue
  bool Get(char *pt); // remove data from queue
  void Print(void);   // display element of queue on LCD
};


#endif
