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
#include "Prototypes_and_Structures.h"

int main() {
	char filename[] = "text.txt"; // имя обрабатываемого файла с текстом
	tree root = NULL; // исходный указатель на бинарное дерево

	// обработка текста, создание бинарного дерева поиска
	if (fromTextToTree(filename, &root) == 1)
		puts("File \"text.txt\" has been processed.");
	else
		puts("Can not create a binary tree!");
	
	// вывод дерева в файл
	if (printInFile(root) == 1)
		puts("File \"concordance.txt\" has been created or overwritten.");
	else
		puts("Printing function crashed!");

	// нахождение и подсчет слов с заданной длиной
	if (wordsWithLength(root) == 1)
		puts("File \"words_by_length.txt\" has been created or overwritten.");
	else
		puts("Function \"wordsWithLength\" crashed");
	system("PAUSE");
}

/* Создает бинарное дерево, "пробегая" по каждому символу в файле
* Аргументы: имя исходного файла, адрес дерева
* Возвращает 1, если выполнена успешно, и 0, если ошибка.
*/
int fromTextToTree(char *filename, tree *root) {
	int symbol, i, k, letter;
	char character;
	// массив для знаков припинания и других символов,
	// потенциально завершающих слово
	char notLetter[] = { ' ', ' ', ',', '.', '?', '!', ';', ':', '"', '=',
		'[', ']', '{', '}', '…', '«', '»', '<', '>', '(', ')', '„', '“',
		'–', '—', '-', '*', '%', '&', '#', '+', '\\', '/', '\n', '\t', '\0' };
	tree current = NULL;
	FILE *stream;
	if (!(stream = fopen(filename, "r"))) {
		puts("Error opening file");
		return 0;
	}
	while ((symbol = fgetc(stream)) != EOF) { // читаем посимвольно до конца файла
		character = symbol; // symbol нужен лишь для корректного сравнения с EOF
		// проверка текущего символа на совпадение
		// с каким-либо символом из массива 'notLetter'
		for (k = 0, letter = 1; k < sizeof(notLetter); k++)
			if (character == notLetter[k]) {
				letter = 0;
				break;
			}
		if (letter == 1) { // если это буква
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
				current->word[i] = '\0'; // дописываем нулевой байт в конец слова
				addToTree(&current, root); // добавляем слово в дерево
				current = NULL;
			}
		}
	}
	if ((symbol == EOF) && (current)) { // для последнего слова в файле
		current->word[i] = '\0';
		addToTree(&current, root);
		current = NULL;
	}
	fclose(stream);
	return 1;
}

/* Добавляет слово в бинарное дерево поиска
* Вход: указатель на слово, указатель на корень дерева
*/
void addToTree(tree *current, tree *root) {
	if (!*root) { // если дерево пустое
		(*current)->left = (*current)->right = NULL;
		*root = *current;
	}
	else // если дерево уже существует
		/* Слово меньшее в лексографическом порядке добавляется
		* в левую ветвь, большее - в правую. Если слово идентично
		* уже добавленному, то у добавленного слова счетчик
		* повторений увеличивается на единицу.
		*/
		if ((strcmp((*current)->word, (*root)->word)) < 0)
			addToTree(current, &(*root)->left);
		else
			if ((strcmp((*current)->word, (*root)->word)) == 0)
				(*root)->repeat++;
			else
				addToTree(current, &(*root)->right);
}

/* Выводит в файл конкорданс в виде списка слов в три колонки
* с количествоим их повторений в квадратных скобках.
* Предлагает 4 варианта обхода дерева.
* Вход: исходный корень дерева.
* Возвращает 1 в случае успеха и 0 в случае ошибки.
*/
int printInFile(tree root) {
	FILE *stream;
	int col = 0, key;
	// создания файла для вывода конкорданса
	if (!(stream = fopen("concordance.txt", "w"))) {
		puts("Can not open file \"concordance.txt\"!");
		return 0;
	}
	do { // выбор пользователем варианта обхода дерева
		 // ввод защищен от случайных символов
		fflush(stdin);
		puts("Choose the way to workaround the tree (put any key from 1 to 4):\n"
			"    In depth:\n\t1 - Prefix\n\t2 - Intfix\n\t3 - Postfix\n4 - In width");
	} while ((scanf("%d", &key) != 1) || (key < 1) || (key > 4));
	switch (key)
	{
		// При выборе любого обхода в глубину вызывается одна и та же функция.
	case 1:
	case 2:
	case 3:
		printInFileDepth(root, &stream, &col, &key);
		break;
	case 4:
		printInFileWidth(root, &stream, &col);
		break;
	default:
		break;
	}
	fclose(stream);
	return 1;
}

/* Осуществляет вывод дерева одним из вариантов обхода в глубину
* в зависимости от выбора пользователя.
* Вход: исходный корень дерева, указатель на поток открытого файла
* 'concordance.txt', номер текущей колонки, номер варианта обхода.
*/
void printInFileDepth(tree root, FILE **stream, int *col, int *key) {
	if (root) {
		switch (*key)
		{
		case 1: // префискный обход
			printWord(root, stream, col);
			printInFileDepth(root->left, stream, col, key);
			printInFileDepth(root->right, stream, col, key);
			break;
		case 2: // инфиксный обход
			printInFileDepth(root->left, stream, col, key);
			printWord(root, stream, col);
			printInFileDepth(root->right, stream, col, key);
			break;
		case 3: // постфиксный обход
			printInFileDepth(root->left, stream, col, key);
			printInFileDepth(root->right, stream, col, key);
			printWord(root, stream, col);
			break;
		}
	}
}

/* Осуществляет вывод дерева используя обход в ширину.
* Вход: исходный корень дерева, указатель на поток открытого файла
* 'concordance.txt', номер текущей колонки.
*/
void printInFileWidth(tree root, FILE **stream, int *col) {
	DblLinkedList *queue = createDblLinkedList(); // создание очереди
	pushBack(&queue, root); // добавление исходного корня дерева в очередь
	while (queue->size) // пока очередь не пуста
	{
		tree tmp = popFront(&queue); // удаляем головной элемент из очереди
		printWord(tmp, stream, col); // печатаем слово в файл
		if (tmp->left) // при наличии потомка по левой ветке ставим его в очередь
			pushBack(&queue, tmp->left);
		if (tmp->right) // при наличии потомка по правой ветке ставим его в очередь
			pushBack(&queue, tmp->right);
	}
	deleteDblLinkedList(&queue); // удаление использованной очереди
}

/* Осуществляет непосредственно печать слова и
* количества его повторений в файл.
* Организует трехколоночный вывод.
* Вход: исходный корень дерева, указатель на поток открытого файла
* 'concordance.txt', номер текущей колонки.
*/
void printWord(tree root, FILE **stream, int *col) {
		if (root->repeat > 1) {
			// если слово повторяется больше одного раза,
			// то выводим количество его повторений
			fprintf(*stream, "%-30s", root->word);
			(*col)++;
			fprintf(*stream, "[%-4d]\t", root->repeat);
		}
		else {
			fprintf(*stream, "%-36s\t", root->word);
			(*col)++;
		}
		if (*col == 3) {
			// срабатывание счетчика для распределения по колонкам
			fprintf(*stream, "\n");
			*col = 0;
		}
}

/* Подготавливает данные для функции вывода всех
* листьев дерева с заданной длиной, после - вызывает функцию,
* осуществляющую вывод и дописывает в файл количество напечатанных слов.
* Вход: исходный корень дерева.
* Возвращает 1 в случае успеха и 0 в случае ошибки.
*/
int wordsWithLength(tree root) {
	int length = 0, amount = 0, col = 0;
	do {
		// ввод пользователем длины слова с защитой от ввода случайных значений
		fflush(stdin);
		puts("Put the word length");
	} while ((scanf("%d", &length) != 1) || (length < 1) || (length > 30));
	FILE *stream;
	// создание файла для вывода резульата
	if (!(stream = fopen("words_by_length.txt", "w"))) {
		puts("Can not open file \"concordance.txt\"!");
		return 0;
	}
	fprintf(stream, "Here is a list of words with length equal %d"
		" which are the leafs of the search binary tree:\n", length);
	// печать списка слов
	wordsWithLengthRecursion(root, &stream, length, &amount, &col);
	fprintf(stream, "\n----------------------------------------------"
		"--------------------------------------------\nFound %d word(s)", amount);
	fclose(stream);
	return 1;
}

/* Осуществляет непосредственную печать найденных слов в файл.
* Вход: исходный корень дерева, указатель на поток открытого файла
* 'concordance.txt', длина слова, счетчик количества найденных
* слов, номер текущей колонки.
*/
void wordsWithLengthRecursion(tree root, FILE **stream, int length, int *amount, int *col) {
	if (root) {
		wordsWithLengthRecursion(root->left, stream, length, amount, col);
		if ((strlen(root->word) == length) && !(root->left) && !(root->right)) {
			// если длина слова удовлетворяет условию и слово является
			// листом бинарного дерева поиска
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
