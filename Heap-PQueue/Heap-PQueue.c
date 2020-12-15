// Heap-PQueue.c : This file contains the 'main' function. Program execution begins and ends there.
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"

#include "heap.h"
#include "pqueue.h"
#include "cqueue.h"

#include "parcel.h"
#include "parcels.h"

////////
// HEAP:
////////
// heap data structure
/////////////////////////////////////////////////////////////
//
// typedef struct Heap_ {
//
//     int size;
//
//     int (*compare)(const void* key1, const void* key2);
//     void (*destroy)(void* data);
//
//     void** tree;
//
// } Heap;
///////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//  Define private macros used by the heap implementation. 
////////////////////////////////////////////////////////////

#define heap_parent(npos) ((int)(((npos) - 1) / 2))

#define heap_left(npos) (((npos) * 2) + 1)

#define heap_right(npos) (((npos) * 2) + 2)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Public interface: Heap API
////////////////////////////////
// void heap_init(Heap* heap, int (*compare)(const void* key1, const void* key2),  void (*destroy)(void* data))
// void heap_destroy(Heap* heap)
// int  heap_insert(Heap* heap, const void* data)
// int  heap_extract(Heap* heap, void** data)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void heap_init(Heap* heap, int (*compare)(const void* key1, const void* key2),  void (*destroy)(void* data)) {

    heap->size = 0;
    heap->compare = compare;
    heap->destroy = destroy;
    heap->tree = NULL;

    return;
}

void heap_destroy(Heap* heap) {

    int i;
    if (heap->destroy != NULL) {

        for (i = 0; i < heap_size(heap); i++) {
            heap->destroy(heap->tree[i]);                                                                   // A user-defined function to free dynamically allocated data.
        }
    }

    free(heap->tree);                                                                                       // Free the storage allocated for the heap.

    memset(heap, 0, sizeof(Heap));                                                                          // Clear the structure to be on the safe side.

    return;
}

int heap_insert(Heap* heap, const void* data) {

    void* temp;

    int ipos;
    int  ppos;

    if ((temp = (void**)realloc(heap->tree, (heap_size(heap) + 1) * sizeof(void*))) == NULL) {
        return -1;
    } else {
        heap->tree = temp;
    }

    heap->tree[heap_size(heap)] = (void*)data;                                      //  Insert the node after the last node.       

    ipos = heap_size(heap);                                                         // Heapify the tree by pushing the contents of the new node upward.
    ppos = heap_parent(ipos);

    while (ipos > 0 && heap->compare(heap->tree[ppos], heap->tree[ipos]) < 0) {

        temp = heap->tree[ppos];                                                    // Swap the contents of the current node and its parent.
        heap->tree[ppos] = heap->tree[ipos];
        heap->tree[ipos] = temp;

        ipos = ppos;                                                                // Move up one level in the tree to continue heapifying.
        ppos = heap_parent(ipos);

    }

    heap->size++;                                                                   // Adjust the size of the heap to account for the inserted node.

    return 0;
}

int heap_extract(Heap* heap, void** data) {

    void* save;
    void* temp;

    int ipos;
    int lpos;
    int rpos;
    int mpos;

    if (heap_size(heap) == 0)                                                       //  Do not allow extraction from an empty heap.
        return -1;

    *data = heap->tree[0];                                                          //  Extract the node at the top of the heap.  

    save = heap->tree[heap_size(heap) - 1];                                         //  Adjust the storage used by the heap.

    if (heap_size(heap) - 1 > 0) {

        if ((temp = (void**)realloc(heap->tree, (heap_size(heap) - 1) * sizeof(void*))) == NULL) {
            return -1;
        } else {
            heap->tree = temp;
        }

        heap->size--;                                                               //  Adjust the size of the heap to account for the extracted node.

    } else {

        free(heap->tree);                                                           // Manage the heap when extracting the last node.
        heap->tree = NULL;
        heap->size = 0;
        return 0;
    }

    heap->tree[0] = save;                                                           // Copy the last node to the top.

    ipos = 0;                                                                       // Heapify the tree by pushing the contents of the new top downward.
    lpos = heap_left(ipos);
    rpos = heap_right(ipos);

    while (1) {

        lpos = heap_left(ipos);                                                     //  Select the child to swap with the current node.
        rpos = heap_right(ipos);

        if (lpos < heap_size(heap) && heap->compare(heap->tree[lpos], heap->tree[ipos]) > 0) {
            mpos = lpos;
        } else {
            mpos = ipos;
        }

        if (rpos < heap_size(heap) && heap->compare(heap->tree[rpos], heap->tree[mpos]) > 0) {
            mpos = rpos;
        }

        if (mpos == ipos) {
            break;
        } else {

            temp = heap->tree[mpos];                                                    // Swap the contents of the current node and the selected child.
            heap->tree[mpos] = heap->tree[ipos];
            heap->tree[ipos] = temp;

            ipos = mpos;                                                                //  Move down one level in the tree to continue heapifying.  
        }
    }
    return 0;
}



/////////////// end HEAP


///////////////////////
// Utility Functions
///////////////////////

static void print_heap(Heap* heap) {

    int i;

    fprintf(stdout, "Heap size is %d\n", heap_size(heap));

    for (i = 0; i < heap_size(heap); i++)
        fprintf(stdout, "Node=%03d\n", *(int*)heap->tree[i]);

    return;
}


static int compare_int(const void* int1, const void* int2) {

    if (*(const int*)int1 > * (const int*)int2)
        return 1;
    else if (*(const int*)int1 < *(const int*)int2)
        return -1;
    else
        return 0;
}

static void print_pqueue(PQueue* pqueue) {

    int i;

    fprintf(stdout, "Priority queue size is %d\n", pqueue_size(pqueue));

    for (i = 0; i < pqueue_size(pqueue); i++)
        fprintf(stdout, "Node=%03d\n", *(int*)pqueue->tree[i]);

    return;
}



////////////////////////////////////////////////////////////////////////////////////////////
// Circular Queue - Data Struct; cqueue.h
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
//
// struct Node {
//     int data;
//     struct Node* next;
// };
//
// struct Queue {
//     struct Node* front;
//     struct Node* rear;
// };
///////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////
// Public interface: Circular Queue API
////////////////////////////////////////////
// void enQueue(struct Queue* q, int value)
// int deQueue(struct Queue* q)
// void displayQueue(struct Queue* q)
////////////////////////////////////////////

// create circular queue
void
enQueue(struct Queue* q, int value) {

    struct Node* pnew = (struct Node*)malloc(sizeof(struct Node));       // actual memory alloc, pair malloc-free, new-delete
    pnew->data = value;

    if (q->front == NULL)       // empty queue, upd front
        q->front = pnew;
    else                        // queue not empty, upd back 
        q->rear->next = pnew;

    q->rear = pnew;             // upd back
    q->rear->next = q->front;   // upd front
}

int deQueue(struct Queue* q)
{
    if (q->front == NULL) {
        printf("Queue is empty");
        return INT_MIN;
    }

    int value; // Value to be dequeued 

    if (q->front == q->rear) {                // If this is the last node to be deleted

        value = q->front->data;
        free(q->front);
        q->front = NULL;
        q->rear = NULL;

    }
    else {                                  // more than one node in the queue

        struct Node* cur = q->front;
        value = cur->data;
        q->front = q->front->next;
        q->rear->next = q->front;
        free(cur);

    }

    return value;
}

// Function displaying the elements of Circular Queue 
void displayQueue(struct Queue* q)
{
    struct Node* cur = q->front;
    printf("\nElements in Circular Queue are: ");

    while (cur->next != q->front) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("%d", cur->data);
}

/////////////////////////////////////////
//
// PQueue Data Struct - pqueue.h
//
/////////////////////////////////////////
//
// typedef Heap PQueue;
//
//////////////////////////////////////////
//// Public Interface: Priority Queue API
//////////////////////////////////////////
//
//#define pqueue_init heap_init
//
//#define pqueue_destroy heap_destroy
//
//#define pqueue_insert heap_insert
//
//#define pqueue_extract heap_extract
//
//#define pqueue_peek(pqueue) ((pqueue)->tree == NULL ? NULL : (pqueue)->tree[0])
//
//#define pqueue_size heap_size
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Application of Priority Queue in Post Office Parcel Mgmt
// Publick Interface: Parcel API
// int get_parcel(PQueue *parcels, Parcel *parcel) 
// int put_parcel(PQueue *parcels, const Parcel *parcel)
///////////////////////////////////////////////////////////

int get_parcel(PQueue* parcels, Parcel* parcel) {

    Parcel* data;

    if (pqueue_size(parcels) == 0)
        return -1;
    else {       // That parcel could not be retrieved, return -1:

        if (pqueue_extract(parcels, (void**)&data) != 0)
            return -1;
        else {   // Pass back the highest-priority parcel:

            memcpy(parcel, data, sizeof(Parcel));
            free(data);
        }
    }

    return 0;
}

int put_parcel(PQueue* parcels, const Parcel* parcel) {

    Parcel* data;

    if ((data = (Parcel*)malloc(sizeof(Parcel))) == NULL)                       // Allocate storage for the parcel. 
        return -1;

    memcpy(data, parcel, sizeof(Parcel));

    if (pqueue_insert(parcels, data) != 0)                                      // Insert the parcel into the priority queue.
        return -1;

    return 0;
}


////////////////
// MAINLINE
////////////////
int main()
{
    printf("Entering Heap and PQueue implementation in C...\n");

    ///////////////
    // Heap Usage
    ///////////////

    Heap heap;
    void* data;

    int intval[30];
    int i;

    heap_init(&heap, compare_int, NULL);                                                    // init heap

    i = 0;                                                                                  // perform some insert heap operations:

    intval[i]= 5;
    fprintf(stdout, "Inserting %03d\n", intval[i]);
    if (heap_insert(&heap, &intval[i]) != 0)
        return 1;
    print_heap(&heap);
    i++;

    intval[i]= 10;
    fprintf(stdout, "Inserting %03d\n", intval[i]);
    if (heap_insert(&heap, &intval[i]) != 0)
        return 1;
    print_heap(&heap);
    i++;

    intval[i]= 20;
    fprintf(stdout, "Inserting %03d\n", intval[i]);
    if (heap_insert(&heap, &intval[i]) != 0)
        return 1;
    print_heap(&heap);
    i++;

    intval[i]= 1;
    fprintf(stdout, "Inserting %03d\n", intval[i]);
    if (heap_insert(&heap, &intval[i]) != 0)
        return 1;
    print_heap(&heap);
    i++;

    intval[i]= 25;
    fprintf(stdout, "Inserting %03d\n", intval[i]);
    if (heap_insert(&heap, &intval[i]) != 0)
        return 1;
    print_heap(&heap);
    i++;

    intval[i]= 22;
    fprintf(stdout, "Inserting %03d\n", intval[i]);
    if (heap_insert(&heap, &intval[i]) != 0)
        return 1;
    print_heap(&heap);
    i++;

    intval[i]= 9;
    fprintf(stdout, "Inserting %03d\n", intval[i]);
    if (heap_insert(&heap, &intval[i]) != 0)
        return 1;
    print_heap(&heap);
    i++;

    while (heap_size(&heap) > 0) {

        if (heap_extract(&heap, (void**)&data) != 0)
            return 1;
        fprintf(stdout, "Extracting %03d\n", *(int*)data);
        print_heap(&heap);

    }

    fprintf(stdout, "Destroying the heap\n");                                       
    heap_destroy(&heap);                                                                        // clean up heap.



    printf("\n------------------------------------------------------\n");




    ///////////////////////////////////////////////////////////////////////////////////////////
    // ## Circular queue usage
    ///////////////////////////////////////////////////////////////////////////////////////////
    // Create a queue and initialize front and rear 
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
    //                                     dequeue()/pop() (14) => 2 elementsleft 22-> -6
    ////////////////////////////////////////////////////////////////////////////////////////////


    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));;
    q->front = q->rear = NULL;

    // Inserting elements in Circular Queue 
    printf("enqueing 14, 22, 6 into circ. q\n");
    enQueue(q, 14);
    enQueue(q, 22);
    enQueue(q, 6);

    // Display elements present in Circular Queue 
    displayQueue(q);

    // Deleting elements from Circular Queue 
    printf("\nDeleted value = %d", deQueue(q));
    printf("\nDeleted value = %d", deQueue(q));

    // Remaining elements in Circular Queue 
    displayQueue(q);

    printf("\nequeueing 9, 20 into circ. q\n");
    enQueue(q, 9);
    enQueue(q, 20);
    displayQueue(q);

    printf("\nDeleted value = %d", deQueue(q));
    printf("\nDeleted value = %d", deQueue(q));
    printf("\nDeleted value = %d", deQueue(q));
    printf("\n------------------------------------------------------\n");


    ////////////////////////
    // Priority Queue Usage
    ////////////////////////
    PQueue pqueue;
    
    void* pq_data;
    
    int ival[30];
    int k;
    
    
    pqueue_init(&pqueue, compare_int, NULL);                                          // Init priority queue
    
 
                                                                                     // Perform seven inserts into priority queue:
    k = 0;
    
    ival[k]= 5;
    fprintf(stdout, "Inserting %03d\n", ival[k]);
    if (pqueue_insert(&pqueue, &ival[k]) != 0)
        return 1;
    print_pqueue(&pqueue);
    k++;
    
    ival[k]= 10;
    fprintf(stdout, "Inserting %03d\n", ival[k]);
    if (pqueue_insert(&pqueue, &ival[k]) != 0)
        return 1;
    print_pqueue(&pqueue);
    k++;
    
    ival[k]= 20;
    fprintf(stdout, "Inserting %03d\n", ival[k]);
    if (pqueue_insert(&pqueue, &ival[k]) != 0)
        return 1;
    print_pqueue(&pqueue);
    k++;
    
    ival[k]= 1;
    fprintf(stdout, "Inserting %03d\n", ival[k]);
    if (pqueue_insert(&pqueue, &ival[k]) != 0)
        return 1;
    print_pqueue(&pqueue);
    k++;
    
    ival[k]= 25;
    fprintf(stdout, "Inserting %03d\n", ival[i]);
    if (pqueue_insert(&pqueue, &ival[k]) != 0)
        return 1;
    print_pqueue(&pqueue);
    k++;
    
    ival[k]= 22;
    fprintf(stdout, "Inserting %03d\n", ival[k]);
    if (pqueue_insert(&pqueue, &ival[k]) != 0)
        return 1;
    print_pqueue(&pqueue);
    k++;
    
    ival[k]= 12;
    fprintf(stdout, "Inserting %03d\n", ival[k]);
    if (pqueue_insert(&pqueue, &ival[k]) != 0)
        return 1;
    print_pqueue(&pqueue);
    k++;
    
    while (pqueue_size(&pqueue) > 0) {                                                      // Extracting the highest priority element form the pqueue
    
        fprintf(stdout, "Peeking at the highest priority element..Value=%03d\n", *(int*)pqueue_peek(&pqueue));
        if (pqueue_extract(&pqueue, (void**)&pq_data) != 0)
            return 1;
        fprintf(stdout, "Extracting %03d\n", *(int*)pq_data);
        print_pqueue(&pqueue);
    
    }
    
    
    fprintf(stdout, "Destroying the pqueue\n");
    pqueue_destroy(&pqueue);                                                                // Clean up priority queue
    
    return 0;
    

} // END_MAINLINE

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
    ///////
    // OUT:
    /////////////////////////////////////////////////////////
    //
    //      Entering Heap and PQueue implementation in C...
    //      Inserting 005
    //      Heap size is 1
    //      Node = 005
    //      Inserting 010
    //      Heap size is 2
    //      Node = 010
    //      Node = 005
    //      Inserting 020
    //      Heap size is 3
    //      Node = 020
    //      Node = 005
    //      Node = 010
    //      Inserting 001
    //      Heap size is 4
    //      Node = 020
    //      Node = 005
    //      Node = 010
    //      Node = 001
    //      Inserting 025
    //      Heap size is 5
    //      Node = 025
    //      Node = 020
    //      Node = 010
    //      Node = 001
    //      Node = 005
    //      Inserting 022
    //      Heap size is 6
    //      Node = 025
    //      Node = 020
    //      Node = 022
    //      Node = 001
    //      Node = 005
    //      Node = 010
    //      Inserting 009
    //      Heap size is 7
    //      Node = 025
    //      Node = 020
    //      Node = 022
    //      Node = 001
    //      Node = 005
    //      Node = 010
    //      Node = 009
    //      Extracting 025
    //      Heap size is 6
    //      Node = 022
    //      Node = 020
    //      Node = 010
    //      Node = 001
    //      Node = 005
    //      Node = 009
    //      Extracting 022
    //      Heap size is 5
    //      Node = 020
    //      Node = 009
    //      Node = 010
    //      Node = 001
    //      Node = 005
    //      Extracting 020
    //      Heap size is 4
    //      Node = 010
    //      Node = 009
    //      Node = 005
    //      Node = 001
    //      Extracting 010
    //      Heap size is 3
    //      Node = 009
    //      Node = 001
    //      Node = 005
    //      Extracting 009
    //      Heap size is 2
    //      Node = 005
    //      Node = 001
    //      Extracting 005
    //      Heap size is 1
    //      Node = 001
    //      Extracting 001
    //      Heap size is 0
    //      Destroying the heap
    //      
    //      ------------------------------------------------------
    //      enqueing 14, 22, 6 into circ.q
    //      
    //      Elements in Circular Queue are : 14 22 6
    //      Deleted value = 14
    //      Deleted value = 22
    //      Elements in Circular Queue are : 6
    //      equeueing 9, 20 into circ.q
    //      
    //      Elements in Circular Queue are : 6 9 20
    //      Deleted value = 6
    //      Deleted value = 9
    //      Deleted value = 20
    //      ------------------------------------------------------
    //      Inserting 005
    //      Priority queue size is 1
    //      Node = 005
    //      Inserting 010
    //      Priority queue size is 2
    //      Node = 010
    //      Node = 005
    //      Inserting 020
    //      Priority queue size is 3
    //      Node = 020
    //      Node = 005
    //      Node = 010
    //      Inserting 001
    //      Priority queue size is 4
    //      Node = 020
    //      Node = 005
    //      Node = 010
    //      Node = 001
    //      Inserting - 858993460
    //      Priority queue size is 5
    //      Node = 025
    //      Node = 020
    //      Node = 010
    //      Node = 001
    //      Node = 005
    //      Inserting 022
    //      Priority queue size is 6
    //      Node = 025
    //      Node = 020
    //      Node = 022
    //      Node = 001
    //      Node = 005
    //      Node = 010
    //      Inserting 012
    //      Priority queue size is 7
    //      Node = 025
    //      Node = 020
    //      Node = 022
    //      Node = 001
    //      Node = 005
    //      Node = 010
    //      Node = 012
    //      Peeking at the highest priority element..Value = 025
    //      Extracting 025
    //      Priority queue size is 6
    //      Node = 022
    //      Node = 020
    //      Node = 012
    //      Node = 001
    //      Node = 005
    //      Node = 010
    //      Peeking at the highest priority element..Value = 022
    //      Extracting 022
    //      Priority queue size is 5
    //      Node = 020
    //      Node = 010
    //      Node = 012
    //      Node = 001
    //      Node = 005
    //      Peeking at the highest priority element..Value = 020
    //      Extracting 020
    //      Priority queue size is 4
    //      Node = 012
    //      Node = 010
    //      Node = 005
    //      Node = 001
    //      Peeking at the highest priority element..Value = 012
    //      Extracting 012
    //      Priority queue size is 3
    //      Node = 010
    //      Node = 001
    //      Node = 005
    //      Peeking at the highest priority element..Value = 010
    //      Extracting 010
    //      Priority queue size is 2
    //      Node = 005
    //      Node = 001
    //      Peeking at the highest priority element..Value = 005
    //      Extracting 005
    //      Priority queue size is 1
    //      Node = 001
    //      Peeking at the highest priority element..Value = 001
    //      Extracting 001
    //      Priority queue size is 0
    //      Destroying the pqueue
    //      
    //      C:\SRC\Heap-PQueue\Debug\Heap-PQueue.exe(process 6148) exited with code 0.



