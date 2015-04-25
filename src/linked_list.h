#pragma once

struct _Node {
	struct _Node *next;
	void *value;
};
typedef struct _Node *Node;

Node ll_create(void *value);
void ll_destroy(Node head);
Node ll_tail(Node head);
	// insert new Node at the end of the list
Node ll_append(Node head, void *newValue);
Node ll_remove(Node head, Node pos);
