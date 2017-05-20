/***********************************************************************
 *
 * Author:     Ake Jacksen
 * Filename:   fifo.h
 * Date:       930525
 * Modified:   010127
 *
 *
 ***********************************************************************/

#ifndef FIFO_H
#define FIFO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define FALSE 0
// #define TRUE  1

typedef struct queue QUEUE_T;

struct QueueNodeStruct{
	double data;
	struct QueueNodeStruct *link;
	};

struct queue{
       struct QueueNodeStruct *front;
       struct QueueNodeStruct *rear;
       };

bool EmptyQ(struct queue *queue);
void Enqueue(struct queue *queue, double data );
void Dequeue( struct queue *queue, double &data);
void New_node(struct QueueNodeStruct **newnode);
void initializeQ(struct queue *queue);

#endif
