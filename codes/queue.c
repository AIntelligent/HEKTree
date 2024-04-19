//
//	HEKTREE - 17/04/2024
//
// queue.c
//
// Author:
//       Hakan Emre Kartal <hek@nula.com.tr>
//
// Copyright (c) 2024 Hakan Emre Kartal
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#include "queue.h"

struct _node_t
{
	struct _node_t *Link;
};

struct _queue_t
{
	struct _node_t *Base;
	struct _node_t *Last;
};

typedef \
	struct _node_t *node_ptr;
	
typedef \
	struct _queue_t *queue_ptr;
	
#define _queue_ptr(b)((queue_ptr)(b))
#define _node_ptr(b)((node_ptr)(b))

queue_t new_queue( void )
{ 
	queue_ptr \
		l_ptrResult = (queue_ptr)malloc( sizeof(struct _queue_t) );
	
	l_ptrResult->Base = l_ptrResult->Last = (node_ptr)0;
	
	return (queue_t)l_ptrResult;
}

node_t queue_push( queue_t inQueuePtr, node_t inNodePtr )
{
	_node_ptr(inNodePtr)->Link = (node_ptr)0;
	
	if (_queue_ptr(inQueuePtr)->Base != (node_ptr)0)
	{ _queue_ptr(inQueuePtr)->Last->Link = _node_ptr(inNodePtr); }
	else
	{ _queue_ptr(inQueuePtr)->Base = _node_ptr(inNodePtr); }
	
	return ((node_t)(_queue_ptr(inQueuePtr)->Last = _node_ptr(inNodePtr)));
}

int queue_pop( queue_t inQueuePtr, node_t *outNode )
{
	int \
		l_iResult;
	node_ptr \
		l_ptrNode;
	
	if ((l_iResult = (int)(_queue_ptr(inQueuePtr)->Base != (node_ptr)0)))
	{
		*outNode = (node_t)(l_ptrNode = _queue_ptr(inQueuePtr)->Base);
		_queue_ptr(inQueuePtr)->Base = l_ptrNode->Link;
	}
	
	return l_iResult;
}

void *queue_first( const queue_t inQueuePtr )
{ return _queue_ptr(inQueuePtr)->Base; }

void queue_explore( const queue_t inQueuePtr, explore_handler_t inExploreHandler, void *inExtraParam )
{
	for																												\
	(																													\
		node_ptr l_ptrNode = _queue_ptr(inQueuePtr)->Base;												\
		(l_ptrNode != NULL) && inExploreHandler( (const node_t)l_ptrNode, inExtraParam );	\
		l_ptrNode = l_ptrNode->Link 																			\
	);
}

int queue_count( const queue_t inQueuePtr )
{
	int \
		l_iResult = 0;
		
	for	 																\
	( 																		\
		node_ptr l_ptrNode = _queue_ptr(inQueuePtr)->Base; \
		l_ptrNode != (node_ptr)0;									\
		l_ptrNode = l_ptrNode->Link, 								\
		l_iResult++ 													\
	);
		
	return l_iResult;
}

int queue_is_empty( const queue_t inQueuePtr )
{ return (inQueuePtr != NULL) ? (_queue_ptr(inQueuePtr)->Base == NULL) : !0; }

void delete_queue( queue_t *ioQueuePtr, free_handler_t inFreeHandler )
{
	queue_t \
		l_ptrQueue = _queue_ptr(*ioQueuePtr);
	node_ptr \
		l_ptrBase = _queue_ptr(l_ptrQueue)->Base, \
		l_ptrLink;
		
	*ioQueuePtr = (queue_t)0;
		
	_queue_ptr(l_ptrQueue)->Base = \
	_queue_ptr(l_ptrQueue)->Last = (node_ptr)0;
	
	free( l_ptrQueue );
	
	while (l_ptrBase != (node_ptr)0)
	{
		l_ptrLink = l_ptrBase->Link;
		
		if (inFreeHandler != NULL)
		{ inFreeHandler( l_ptrBase ); }
		
		l_ptrBase = l_ptrLink;
	}
}