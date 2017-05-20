/***********************************************************************
 *
 * Author:     Ake Jacksen
 * Filename:   fifo.cpp
 * Date:       930525
 * Modified:   010127
 *
 * Description: fifo.cpp is an ADT that implements a dynamic queue
 *
 *
 ***********************************************************************/                                                                      
#include "fifo.h"


// ajn 131211
#define AfxMessageBox

/*********************************************************************
 * 
 * Function EmptyQ()
 *
 * Description: Checks if queue is empty
 *
 *********************************************************************/

bool EmptyQ(struct queue *queue)
{
    if(queue->rear==NULL)
    {
        return(true);
    }

    else
    {
        return(false);
    }
}


/*********************************************************************
 * 
 * Function Enqueue()
 *
 * Description: Add new data to queue
 *
 *********************************************************************/

void Enqueue(struct queue *queue, double data)
{
    struct QueueNodeStruct *newnode;
    
    New_node(&newnode);
    newnode->data = data;
    newnode->link = NULL;

    if(EmptyQ(queue))
        queue->front=newnode;
    else
        (queue->rear)->link=newnode;
    
    queue->rear=newnode;
}



/*********************************************************************
 * 
 * Function Dequeue()
 *
 * Description: Remove data from queue
 *
 *********************************************************************/

void Dequeue( struct queue *queue, double &data)
{
    struct QueueNodeStruct *pointer;
    
    
    if( EmptyQ(queue))
	{
       // AfxMessageBox("Error: Queue is empty\n");
        exit(1);
    }
    pointer = queue->front;
    data = queue->front->data;
    queue->front = queue->front->link;
    free(pointer);
    
	if(queue->front==NULL)                              /* modiferad */
        queue->rear=NULL;
}




#if 0
/*********************************************************************
 * 
 * Function Enqueue()
 *
 * Description: Remove data from queue
 *
 *********************************************************************/

void GetNextQueueData(struct queue *queue, double data )
{
        
    
      
    if( EmptyQ(queue))
	{
       // AfxMessageBox("Error: Queue is empty\n");
        exit(1);
    }
    data = queue->front->data;
    queue->front = queue->front->link;
        
	if(queue->front==NULL)                             
        queue->rear=NULL;

}
#endif




/*********************************************************************
 * 
 * Function New_node()
 *
 * Description: Create a new node
 *
 *********************************************************************/

void New_node(struct QueueNodeStruct **newnode)
{
    *newnode=(struct QueueNodeStruct *) malloc(sizeof(struct QueueNodeStruct));
    
	if(*newnode==NULL)
	{
        //AfxMessageBox("Error: Queue report out of memory\n");
        exit(1);
    }
}


/*********************************************************************
 * 
 * Function: initializeQ()
 *
 * Description: 
 *
 *********************************************************************/
 void initializeQ(struct queue *queue)
 {
 queue->rear=NULL;
 }





