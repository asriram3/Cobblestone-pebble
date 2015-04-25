#include <pebble.h>
#include "linked_list.h"


Node ll_create(void *value) {
	Node node = (Node)malloc(sizeof(struct _Node));
	if (node) {
		node->value	= value;
		node->next	= NULL;
	}
	return node;
}

void ll_destroy(Node head) {
	if (head->next)
		ll_destroy(head->next);
	free(head->value);
	free(head);
}

Node ll_prev(Node head, Node target) {
	Node curr = head;
	while (curr && curr->next != target)
		curr = curr->next;
	return curr;
}

Node ll_tail(Node head) {
	Node node = head;
	while (node->next)
		node = node->next;
	return node;
}

/// Insert new Node at the end of the list
Node ll_append(Node head, void *newValue) {
	Node tail = ll_tail(head);
	Node newNode = ll_create(newValue);
	tail->next = newNode;
	return newNode;
}

Node ll_remove(Node head, Node pos) {
	Node prev = ll_prev(head, pos);
	if (prev)
		prev->next = pos->next;
	Node newHead;
	if (pos == head)
		newHead = pos->next;
	else
		newHead = head;
	pos->next = NULL;
	ll_destroy(pos);
	return newHead;
}
