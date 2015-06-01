#include <stdio.h>
#include <malloc.h>
#include "Prototypes_and_Structures.h"

/* Создает двухсвязный список и возвращает
* указатель на него
*/
DblLinkedList* createDblLinkedList() {
	DblLinkedList *tmp = (DblLinkedList*)malloc(sizeof(DblLinkedList));
	tmp->size = 0;
	tmp->head = tmp->tail = NULL;
	return tmp;
}

/* Удаляет все узлы двухсвязного списка.
* Вход: адрес указателя на список.
*/
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

/* Удаляет элемент из головы списка и
* возвращает указатель на него.
* Если список пуст, возвращает NULL.
* Вход: адрес указателя на список.
*/
tree popFront(DblLinkedList **dblist) {
	list prev;
	tree tmp;
	// если список пуст
	if (!(*dblist)->head)
		return NULL;
	// запоминаем головной узел и переводим
	// указатель головы списка на его следующий элемент
	prev = (*dblist)->head;
	(*dblist)->head = (*dblist)->head->next;
	// если такой элемент существует, то
	// обнуляем его указатель на предыдущий узел
	if ((*dblist)->head)
		(*dblist)->head->prev = NULL;
	// если список был с единственным узлом,
	// обнуляем указатель на конечный элемент
	if (prev == (*dblist)->tail)
		(*dblist)->tail = NULL;
	// сохраняем необходимое содержимое удаляемого элемента
	// и освобождаем память
	tmp = prev->tree_pointer;
	(*dblist)->size--;
	return tmp;
}

/* Добавляет в конец очереди переданный элемент дерева.
* Вход: адрес указателя на список и адрес на элемент дерева.
*/
void pushBack(DblLinkedList **dblist, tree root) {
	list current;
	if (!(current = (list)malloc(sizeof(list)))) {
		puts("Can not allocate memory!");
		return;
	}
	//передаем адрес элемента дерева в узел списка
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