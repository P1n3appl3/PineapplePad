#include <stdint.h>
#include "../inc/FIFO.h"
#include "../inc/ST7735.h"

// A class named Queue that defines a FIFO
Queue::Queue() {
    front = 0;
    back = 0;
}

// To check whether Queue is empty or not
bool Queue::IsEmpty(void) {
    return front == back;
}

// To check whether Queue is full or not
bool Queue::IsFull(void) {
    return (FIFOSIZE + back - front) % FIFOSIZE == 1;
}

// Inserts an element in queue at rear end
bool Queue::Put(char x) {
    if (IsFull()) {
        return false;
    }
    buf[front] = x;
    front = (front + 1) % FIFOSIZE;
    return true;
}

// Removes an element in Queue from front end.
bool Queue::Get(char* pt) {
    if (IsEmpty()) {
        return false;
    }
    *pt = buf[back];
    back = (back + 1) % FIFOSIZE;
    return true;
}

/*
   Printing the elements in queue from front to rear.
   This function is only to test the code.
   This is not a standard function for Queue implementation.
 */
void Queue::Print(void) {
    int y = 0;
    for (int i = back; i != front; i = (i + 1) % FIFOSIZE) {
        ST7735_SetCursor(0, y++);
        // ST7735_OutUDec(buf[i]);
        ST7735_OutChar(buf[i]);
    }
}
