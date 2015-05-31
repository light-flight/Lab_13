#define _CRT_SECURE_NO_WARNINGS

/*
 * ЛАБОРАТОРНАЯ РАБОТА 13. УКАЗАТЕЛИ. ДИНАМИЧЕСКИЕ СТРУКТУРЫ ДАННЫХ. ОБРАБОТКА БИНАРНЫХ ДЕРЕВЬЕВ
 *
 * ЗАДАНИЕ
 * Дан текст, хранящийся в файле. Текст состоит из слов, разделенных пробелами – 
 * длина слова не более 30 букв. Решить простейшую задачу создания конкорданса.
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
#include <locale.h>

struct _binaryTree {
	char word[31];
	int repeat;
	struct _binaryTree *left, *right;
};
typedef struct _binaryTree* tree;

int fromTextToTree(char*, tree*);
void addToTree(tree*, tree*);

int printInFile(tree);
void printInFileRecursion(tree, FILE**, int*);

int wordsWithLength(tree);
void wordsWithLengthRecursion(tree, FILE**, int, int*, int*);

int main() {
	setlocale(LC_ALL, "RUS");
	char filename[] = "text.txt";
	tree root = NULL;

	if (fromTextToTree(filename, &root) == 1)
		puts("File \"text.txt\" has been processed.");
	else
		puts("Can not create a binary tree!");
		
	if (printInFile(root) == 1)
		puts("File \"concordance.txt\" has been created or overwritten.");
	else
		puts("Printing function crashed!");
	if (wordsWithLength(root) == 1)
		puts("File \"words_by_length.txt\" has been created or overwritten.");
	else
		puts("Function \"wordsWithLength\" crashed");
	system("PAUSE");
}

int fromTextToTree(char *filename, tree *root) {
	int symbol, i;
	char character;
	tree current = NULL;
	FILE *stream;
	if (!(stream = fopen(filename, "r"))) {
		puts("Error opening file");
		return 0;
	}
	while ((symbol = fgetc(stream)) != EOF) { // читаем посимвольно до конца файла
		character = symbol;
		if (
		(character != ' ') && (character != '.') && (character != '\n') &&
		(character != ',') && (character != '\0') && (character != '!') && 
		(character != '?') && (character != '…') && (character != ':') &&
		(character != ';') && (character != '«') && (character != '»') &&
		(character != '"') && (character != '<') && (character != '>') &&
		(character != '=') && (character != '(') && (character != ')') &&
		(character != '[') && (character != ']') && (character != '{') &&
		(character != '}') && (character != '„') && (character != '“') && (character != '–')) { // если это буква
			if (!current) { // если это новое слово
				if (!(current = (tree)malloc(sizeof(struct _binaryTree)))) { // выделяем для него память
					puts("Can not allocate memory!");
					return 0;
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
	return 1;
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

int printInFile(tree root) {
	FILE *stream;
	int col = 0;
	if (!(stream = fopen("concordance.txt", "w"))) {
		puts("Can not open file \"concordance.txt\"!");
		return 0;
	}
	printInFileRecursion(root, &stream, &col);
	return 1;
}

void printInFileRecursion(tree root, FILE **stream, int *col) {
	if (root) {
		printInFileRecursion(root->left, stream, col);
		if (root->repeat > 1) {
			fprintf(*stream, "%-30s", root->word);
			(*col)++;
			fprintf(*stream, "[%-4d]\t", root->repeat);
		}
		else {
			fprintf(*stream, "%-36s\t", root->word);
			(*col)++;
		}
		if (*col == 3) {
		fprintf(*stream, "\n");
		*col = 0;
		}
		printInFileRecursion(root->right, stream, col);
	}
}

int wordsWithLength(tree root) {
	int length = 0, amount = 0, col = 0;
	do {
		fflush(stdin);
		puts("Put the word length");
	} while ((scanf("%d", &length) != 1) || (length < 1) || (length > 30));
	FILE *stream;
	if (!(stream = fopen("words_by_length.txt", "w"))) {
		puts("Can not open file \"concordance.txt\"!");
		return 0;
	}
	fprintf(stream, "Here is a list of words with length equal %d:\n", length);
	wordsWithLengthRecursion(root, &stream, length, &amount, &col);
	fprintf(stream, "\n----------------------------------------------"
		"--------------------------------------------\nFound %d word(s)", amount);
	fclose(stream);
	return 1;
}

void wordsWithLengthRecursion(tree root, FILE **stream, int length, int *amount, int *col) {
	if (root) {
		wordsWithLengthRecursion(root->left, stream, length, amount, col);
		if (strlen(root->word) == length) {
			fprintf(*stream, "%-30s", root->word);
			(*col)++;
			if (*col == 3) {
				fprintf(*stream, "\n");
				*col = 0;
			}
			(*amount)++;
		}
		wordsWithLengthRecursion(root->right, stream, length, amount, col);
	}
}
