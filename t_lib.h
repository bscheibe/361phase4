/*
 * types used by thread library
 */
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>

//A TCB structure containing the thread context and additional info
struct tcb
{
	int thread_id;
	int thread_priority;
	ucontext_t *thread_context;
};

typedef struct tcb tcb;

//Semaphore structure. Uses a FIFO queue which makes it a STRONG Semaphore
struct sem_t
{
	int count;
	struct Queue *q;
};

typedef struct sem_t sem_t;

//A structure symbolizing a node in the linked list representing the queue
struct QNode
{
    tcb *tcb; //The thread control block associated with the node
    struct QNode *next; //Pointer to next node in the queue
};
typedef struct QNode QNode;

//Simple Queue data structure made up of QNodes
//Implemented using a linked list of QNodes
struct Queue
{
	struct QNode *front, *rear; //Pointers to the head and tail of the queue
};
typedef struct Queue Queue;

struct messageNode {
         char *message;     // copy of the message
         int  len;          // length of the message
         int  sender;       // TID of sender thread
         int  receiver;     // TID of receiver thread
         struct messageNode *next; // pointer to next node
       };
typedef struct messageNode messageNode;

struct mbox {
	  struct messageNode  *msg;       // message queue
	  sem_t               *mbox_sem;
       };
typedef struct mbox mbox;


struct QNode *newNode(tcb *tcb);
struct Queue *createQueue();
void enQueue(struct Queue *q, tcb *tcb);
struct QNode *deQueue(struct Queue *q);
void t_yield();
void t_init();
void t_create(void (*fct)(int), int id, int pri);
void t_shutdown();
void t_terminate();
int sem_init(sem_t **sp, int sem_count);
void sem_wait(sem_t *s);
void sem_signal(sem_t *s);
void sem_destroy(sem_t **s);
int mbox_create(mbox **mb); /* Create a mailbox pointed to by mb. */
void mbox_destroy(mbox **mb); /* Destroy any state related to the mailbox pointed to by mb. */
void mbox_deposit(mbox *mb, char *msg, int len); /* Deposit message msg of length len into the mailbox pointed to by mb. */
void mbox_withdraw(mbox *mb, char *msg, int *len); /* Withdraw the first message from the mailbox pointed to by mb into msg and set the message's length in len accordingly. The caller of mbox_withdraw() is responsible for allocating the space in which the received message is stored. If there is no message in the mailbox, len is set to 0. mbox_withdraw() is not blocking. Even if more than one message awaits the caller, only one message is returned per call to mbox_withdraw(). Messages are withdrew in the order in which they were deposited. */
void send(int tid, char *msg, int len); /* Send a message to the thread whose tid is tid. msg is the pointer to the start of the message, and len specifies the length of the message in bytes. In your implementation, all messages are character strings. */
void receive(int *tid, char *msg, int *len); /* Wait for and receive a message from another thread. The caller has to specify the sender's tid in tid, or sets tid to 0 if it intends to receive a message sent by any thread. If there is no "matching" message to receive, the calling thread waits (i.e., blocks itself). [A sending thread is responsible for waking up a waiting, receiving thread.] Upon returning, the message is stored starting at msg. The tid of the thread that sent the message is stored in tid, and the length of the message is stored in len. The caller of receive() is responsible for allocating the space in which the message is stored. Even if more than one message awaits the caller, only one message is returned per call to receive(). Messages are received in the order in which they were sent. The caller will not resume execution until it has received a message (blocking receive). */
