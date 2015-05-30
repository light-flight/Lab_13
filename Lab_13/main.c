﻿#define _CRT_SECURE_NO_WARNINGS

/*
 * ЛАБОРАТОРНАЯ РАБОТА 13. УКАЗАТЕЛИ. ДИНАМИЧЕСКИЕ СТРУКТУРЫ ДАННЫХ. ОБРАБОТКА БИНАРНЫХ ДЕРЕВЬЕВ
 *
 * ЗАДАНИЕ
 * Дан текст, хранящийся в файле. Текст состоит из слов, разделенных пробелами – 
 * длина слова не более 20 букв. Решить простейшую задачу создания конкорданса.
 * Реализовать функцию создания, упорядоченного (в алфавитном порядке) бинарного дерева,
 * где в узлах хранятся слова и количество их появления в указанном тексте.
 * Реализовать различные способы обхода дерева (в глубину, в ширину, в обратном порядке и т.д.)
 * для вывода элементов на экран. Реализовать задачу,
 * приведенную в варианте, в виде отдельной рекурсивной функции.
 *
 * 27 вариант
 * Посчитать количество листьев в дереве, имеющих заданную длину.
 *
 * Выполнил Юрьев Георгий, группа ПМ-13
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys\stat.h>

struct _binaryTree {
	char word[21];
	int repeat;
	struct _binaryTree *left, *right;
};
typedef struct _binaryTree* tree;

void fromTextToTree(char*, tree*);
void addToTree(tree*, tree*);
void print(tree);
void statistic(double, char*, long unsigned int);
long unsigned int file_size(char*);

int main() {
	clock_t start, finish;
	char filename[] = "tree.txt";
	tree root = NULL;

	start = clock();
	fromTextToTree(filename, &root);
	finish = clock();
	printf("Task was accomplished in %f sec.\n", ((double)(finish - start)) / CLOCKS_PER_SEC);

	statistic(((double)(finish - start)) / CLOCKS_PER_SEC, filename, file_size(filename));
	print(root);

	system("PAUSE");
}

void fromTextToTree(char *filename, tree *root) {
	int symbol, i;
	char character;
	tree current = NULL;
	FILE *stream;
	if (!(stream = fopen(filename, "r"))) {
		puts("Error opening file");
		return;
	}
	while ((symbol = fgetc(stream)) != EOF) { // читаем посимвольно до конца файла
		character = symbol;
		if ((character != ' ') && (character != '.') && (character != '\n') &&
		(character != ',') && (character != '\0') &&
		(character != '!') && (character != '?')) { // если это буква
			if (!current) { // если это новое слово
				if (!(current = (tree)malloc(sizeof(struct _binaryTree)))) { // выделяем для него память
					puts("Can not allocate memory!");
					return;
				}
				current->repeat = 1; // инициализируем количество повторений этого слова
				i = 0; // инициализируем счетчик для массива букв
			}
			current->word[i++] = character; // заносим букву в массив
		}
		else {
			if (current) { // если слово закончилось
				current->word[i] = '\0';
				addToTree(&current, root);
				current = NULL;
			}
		}
	}
	fclose(stream);
}

void addToTree(tree *current, tree *root) {
	if (!*root) {
		(*current)->left = (*current)->right = NULL;
		*root = *current;
	}
	else
		if ((strcmp((*current)->word, (*root)->word)) < 0)
			addToTree(current, &(*root)->left);
		else
			if ((strcmp((*current)->word, (*root)->word)) == 0)
				(*root)->repeat++;
			else
				addToTree(current, &(*root)->right);
}

void print(tree root) {
	if (root) {
		print(root->left);
		printf("%s", root->word);
		if (root->repeat > 1)
			printf(" [%d]", root->repeat);
		printf("\n");
		print(root->right);
	}
}

void statistic(double result, char *filename, long unsigned int file_size) {
	char speed_comment[80];
	time_t t = time(NULL);
	struct tm* aTm = localtime(&t);
	FILE *timing;

	if (file_size == 0) {
		puts("File is empty!");
		return;
	}

	printf("The size of \"%s\" is %u bytes.\n", filename, file_size);

	if (!(timing = fopen("timing.txt", "a"))) {
		puts("Can not open file \"timing.txt\"!");
		return;
	}
	puts("Add a comment about optimisation:");
	gets(speed_comment);
	fprintf(timing, "[%02d:%02d:%02d] \t | \t \"%s\" [size: %u Mb] \t | \t %f sec \t | \t %s\n",
		aTm->tm_hour, aTm->tm_min, aTm->tm_sec, filename, file_size / 1000 / 1000, result, speed_comment);
}

long unsigned int file_size(char* filename) {
	struct stat file_stat;

	if (stat(filename, &file_stat) == -1) {
		puts("Can not get file statistics!");
		return 0;
	}
	else
		return file_stat.st_size;
}