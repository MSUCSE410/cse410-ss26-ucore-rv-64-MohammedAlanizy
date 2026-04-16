#include "queue.h"
#include "defs.h"

extern struct proc pool[];

void init_queue(struct queue *q)
{
	q->front = q->tail = 0;
	q->empty = 1;
}

void push_queue(struct queue *q, int value)
{
	if (!q->empty && q->front == q->tail) {
		panic("queue shouldn't be overflow");
	}
	q->empty = 0;
	q->data[q->tail] = value;
	q->tail = (q->tail + 1) % QUEUE_SIZE;
}

int pop_queue(struct queue *q)
{
	if (q->empty) return -1;
		
	int min_q_idx = q->front;
	uint64 min_stride = (uint64)-1; // Set to max possible uint64
	
	// Scan the circular buffer for the process with the smallest stride
	int curr = q->front;
	while (curr != q->tail) {
		int p_idx = q->data[curr];
		if (pool[p_idx].stride < min_stride) {
			min_stride = pool[p_idx].stride;
			min_q_idx = curr;
		}
		curr = (curr + 1) % QUEUE_SIZE;
	}
	
	int value = q->data[min_q_idx];
	
	// Shift elements backwards to close the gap left by the popped element
	curr = min_q_idx;
	while (curr != q->front) {
		int prev = (curr - 1 + QUEUE_SIZE) % QUEUE_SIZE;
		q->data[curr] = q->data[prev];
		curr = prev;
	}
	
	q->front = (q->front + 1) % QUEUE_SIZE;
	if (q->front == q->tail) q->empty = 1;
		
	return value;
}