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

void fromTextToTree(char*, tree*);
void addToTree(tree*, tree*);

void printInFile(tree);
void printInFileRecursion(tree, FILE**, int*);

void statistic(double, char*, long unsigned int);
long unsigned int file_size(char*);

void wordsWithLength(tree);
void wordsWithLengthRecursion(tree, int, int*);
void wordsWithLengthRecursionInFile(tree, FILE**, int, int*, int*);

int main() {
	setlocale(LC_ALL, "RUS");
	clock_t start, finish;
	char filename[] = "text.txt";
	tree root = NULL;

	start = clock();
	fromTextToTree(filename, &root);
	finish = clock();
	printf("The tree has been created in %f sec.\n", ((double)(finish - start)) / CLOCKS_PER_SEC);

	statistic(((double)(finish - start)) / CLOCKS_PER_SEC, filename, file_size(filename));
	
	start = clock();
	printInFile(root);
	finish = clock();
	printf("The tree has been printed in %f sec.\n", ((double)(finish - start)) / CLOCKS_PER_SEC);

	start = clock();
	wordsWithLength(root);
	finish = clock();
	printf("Words have been found in %f sec.\n", ((double)(finish - start)) / CLOCKS_PER_SEC);

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

void printInFile(tree root) {
	FILE *stream;
	int col = 0;
	if (!(stream = fopen("concordance.txt", "w"))) {
		puts("Can not open file \"concordance.txt\"!");
		return;
	}
	printInFileRecursion(root, &stream, &col);
}

void printInFileRecursion(tree root, FILE **stream, int *col) {
	if (root) {
		printInFileRecursion(root->left, stream, col);
		if (root->repeat > 1) {
			fprintf(*stream, "%-30s", root->word);
			(*col)++;
			fprintf(*stream, "[%d]\t", root->repeat);
		}
		else {
			fprintf(*stream, "%-36s", root->word);
			(*col)++;
		}
		if (*col == 3) {
		fprintf(*stream, "\n");
		*col = 0;
		}

		printInFileRecursion(root->right, stream, col);
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
	fprintf(timing, "[%02d:%02d:%02d] \t | \t \"%s\" [size: %u kb] \t | \t %f sec \t | \t %s\n",
		aTm->tm_hour, aTm->tm_min, aTm->tm_sec, filename, file_size / 1000, result, speed_comment);
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

void wordsWithLength(tree root) {
	int key = 0, length = 0, amount = 0, col = 0;
	do {
		fflush(stdin);
		puts("Choose the output (put 1 or 2 key number):\n1 - console\n2 - file");
	} while ((scanf("%d", &key) != 1) || ((key != 1) && (key != 2)));
	do {
		fflush(stdin);
		puts("Put the word length");
	} while ((scanf("%d", &length) != 1) || (length < 1) || (length > 30));
	if (key == 1) {
		printf("Here is a list of words with length equal %d:\n", length);
		wordsWithLengthRecursion(root, length, &amount);
		printf("------------------------------\nFound %d word(s)\n", amount);
	}
	else {
		FILE *stream;
		if (!(stream = fopen("words by length.txt", "w"))) {
			puts("Can not open file \"concordance.txt\"!");
			return;
		}
		fprintf(stream, "Here is a list of words with length equal %d:\n", length);
		wordsWithLengthRecursionInFile(root, &stream, length, &amount, &col);
		fprintf(stream, "\n----------------------------------------------"
			"--------------------------------------------\nFound %d word(s)", amount);
		fclose(stream);
	}
}

void wordsWithLengthRecursion(tree root, int length, int *amount) {
	if (root) {
		wordsWithLengthRecursion(root->left, length, amount);
		if (strlen(root->word) == length) {
			printf("%s\n", root->word);
			(*amount)++;
		}
		wordsWithLengthRecursion(root->right, length, amount);
	}
}

void wordsWithLengthRecursionInFile(tree root, FILE **stream, int length, int *amount, int *col) {
	if (root) {
		wordsWithLengthRecursionInFile(root->left, stream, length, amount, col);
		if (strlen(root->word) == length) {
			fprintf(*stream, "%-30s", root->word);
			(*col)++;
			if (*col == 3) {
				fprintf(*stream, "\n");
				*col = 0;
			}
			(*amount)++;
		}
		wordsWithLengthRecursionInFile(root->right, stream, length, amount, col);
	}
}
