#define _CRT_SECURE_NO_WARNINGS

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

//#define MAX_SENTENCE_ARRAY_LENGTH 255
//#define MAX_WORD_ARRAY_LENGTH 20

struct _binaryTree {
	char word[20];
	int repeat;
	struct _binaryTree *left, *right;
};
typedef struct _binaryTree* tree;

void fromTextToTree(char*, tree*);
void addToTree(tree*, tree*);
void print(tree);

int main() {
	clock_t start, finish;
	char filename[] = "tree.txt";
	tree root = NULL;
	struct stat  file_stat;

	if (stat(filename, &file_stat) == -1) {
		puts("Can not get file statistics!");
	}
	else {
		printf("The size of \"%s\" is %u bytes.\n", filename, file_stat.st_size);
	}

	start = clock();
	fromTextToTree(filename, &root);
	finish = clock();
	printf("Simple concordance task was accomplished in %f sec.\n", ((double)(finish - start)) / CLOCKS_PER_SEC);
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
		(character != '!') && (character != '?')) {

			if (current == NULL) { // если это новое слово, выделим память, подготовим счетчики
				if (!(current = (tree)malloc(sizeof(struct _binaryTree)))) {
					puts("Can not allocate memory!");
					return;
				}
				current->repeat = 1;
				i = 0;
			}

			current->word[i++] = character;

			if ((symbol = fgetc(stream)) == EOF) { // необходимо проинспектировать следующий элемент 
				return;
			}
			else {
				character = symbol;
				if ((character == ' ') || (character == '.') || (character == '\n') ||
				(character == ',') || (character == '\0') ||
				(character == '!') || (character == '?')) { // если слово закончилось
				    current->word[i] = '\0';
					addToTree(&current, root);
					current = NULL;
				}
				fseek(stream, ftell(stream) - 1, SEEK_SET); // возвращаем каретку назад на одну позицию
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