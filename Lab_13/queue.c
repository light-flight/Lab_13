#include <stdio.h>
#include "Prototypes_and_Structures.h"

DblLinkedList* createDblLinkedList() {
	DblLinkedList *tmp = (DblLinkedList*)malloc(sizeof(DblLinkedList));
	tmp->size = 0;
	tmp->head = tmp->tail = NULL;
	return tmp;
}

void deleteDblLinkedList(DblLinkedList **head) {
	list tmp = (*head)->head;
	list next = NULL;
	while (tmp) {
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
	free(*head);
	(*head) = NULL;
}

tree popFront(DblLinkedList **dblist) {
	list prev;
	tree tmp;
	if (!(*dblist)->head)
		return NULL;
	prev = (*dblist)->head;
	(*dblist)->head = (*dblist)->head->next;
	if ((*dblist)->head)
		(*dblist)->head->prev = NULL;
	if (prev == (*dblist)->tail)
		(*dblist)->tail = NULL;
	tmp = prev->tree_pointer;
	(*dblist)->size--;
	return tmp;
}

void pushBack(DblLinkedList **dblist, tree root) {
	list current;
	if (!(current = (list)malloc(sizeof(list)))) {
		puts("Can not allocate memory!");
		return;
	}
	current->tree_pointer = root;
	if ((*dblist)->size == 0) { // если список пуст
		(*dblist)->head = (*dblist)->tail = current;
		current->next = current->prev = NULL;
	}
	else { // добавляем элемент в уже существующий список в конец
		current->next = NULL;
		current->prev = (*dblist)->tail;
		(*dblist)->tail->next = current;
		(*dblist)->tail = current;
	}
	(*dblist)->size++;
	current = NULL;
}