// структура для бинарного дерева
struct _binaryTree {
	char word[31];
	int repeat;
	struct _binaryTree *left, *right;
};
typedef struct _binaryTree* tree;

// стуктура для двухсвязного списка,
// который используется при обходе дерева в ширину
struct queue {
	tree tree_pointer;
	struct list *prev, *next;
};
typedef struct queue* list;

// структура, необходимая для удобной навигации
// и оценке размера двухсвязного списка
typedef struct DblLinkedList {
	size_t size;
	list head;
	list tail;
} DblLinkedList;

//*************************************************************

int fromTextToTree(char*, tree*);
void addToTree(tree*, tree*);

int printInFile(tree);
void printInFileDepth(tree, FILE**, int*, int*);
void printWord(tree, FILE**, int*);
void printInFileWidth(tree, FILE**, int*);

int wordsWithLength(tree);
void wordsWithLengthRecursion(tree, FILE**, int, int*, int*);

DblLinkedList* createDblLinkedList();
void deleteDblLinkedList(DblLinkedList**);
void pushBack(DblLinkedList**, tree);
tree popFront(DblLinkedList**);
