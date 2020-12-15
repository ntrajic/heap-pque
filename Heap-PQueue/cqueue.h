// cqueue.h - circular queue
////////////////////////////
#ifndef CQUEUE_H
#define CQUEUE_H

////////////////////////////////////////////////////////////////////////////////////////////
// Circular Queue - Data Struct
///////////////////////////////
//
//    <------   queue   <------  enque(val)/push_back()
//<--getfront()   x delqueue
//   dequeue()
// front rear
//    f  r           f    r              f            r
//    |  |           |    |              |            |
//    v  v           v    v              v            v
//   -> 14 -      -> 14 -> 22 -         ->14 -> 22 -> -6-
//  | ---- |     |-------------|       | ----------------|
//  enqueue(14)       enqueue(22)         enqueue(-6)
// 
// 
//                                     dequeue()/pop() (14) => like middle drawing
///////////////////////////////////////////////////////////////////////////////////////////
// circular queue data structure
/////////////////////////////////
struct Node {
    int data;
    struct Node* next;
};

struct Queue {
    struct Node* front;
    struct Node* rear;
};
/////////////////////////////////////////
// Public interface: Circular Queue API
/////////////////////////////////////////

void enQueue(struct Queue* q, int value);
int deQueue(struct Queue* q);
void displayQueue(struct Queue* q);

#endif
