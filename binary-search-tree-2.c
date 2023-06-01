/*
 * Binary Search Tree #2
 *
 * Data Structures
 *
 * School of Computer Science
 * at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int key;
	struct node *left;
	struct node *right;
} Node;

/* for stack */
#define MAX_STACK_SIZE 20
Node *stack[MAX_STACK_SIZE];
int top = -1;

/* for queue */
#define MAX_QUEUE_SIZE 20
Node *queue[MAX_QUEUE_SIZE];
int front = -1;
int rear = -1;

int initializeBST(Node **h);
void recursiveInorder(Node *ptr); /* recursive inorder traversal */
int insert(Node *head, int key);  /* insert a node to the tree */
int freeBST(Node *head);		  /* free all memories allocated to the tree */

/* functions that you have to implement */
void iterativeInorder(Node *ptr);	 /* iterative inorder traversal */
void levelOrder(Node *ptr);			 /* level order traversal */
int deleteNode(Node *head, int key); /* delete the node for the key */
Node *pop();
void push(Node *aNode);
Node *deQueue();
void enQueue(Node *aNode);

/* you may add your own defined functions if necessary */

void printStack();

int main()
{
	char command;
	int key;
	Node *head = NULL;

	do
	{
		printf("\n\n[----- [Hyaejun mun] [2019015035] -----]\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z      Print Stack                  = p \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch (command)
		{
		case 'z':
		case 'Z':
			initializeBST(&head);
			break;
		case 'q':
		case 'Q':
			freeBST(head);
			break;
		case 'i':
		case 'I':
			printf("Your Key = ");
			scanf("%d", &key);
			insert(head, key);
			break;
		case 'd':
		case 'D':
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(head, key);
			break;

		case 'r':
		case 'R':
			recursiveInorder(head->left);
			break;
		case 't':
		case 'T':
			iterativeInorder(head->left);
			break;

		case 'l':
		case 'L':
			levelOrder(head->left);
			break;

		case 'p':
		case 'P':
			printStack();
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');

	return 1;
}

int initializeBST(Node **h)
{

	/* if the tree is not empty, then remove all allocated nodes from the tree*/
	if (*h != NULL)
		freeBST(*h);

	/* create a head node */
	*h = (Node *)malloc(sizeof(Node));
	(*h)->left = NULL; /* root */
	(*h)->right = *h;
	(*h)->key = -9999;

	top = -1;

	front = rear = -1;

	return 1;
}

void recursiveInorder(Node *ptr)
{
	if (ptr)
	{
		recursiveInorder(ptr->left);  // 재귀로 구현하고, inorder 방식이므로 좌->중앙->오른 순으로
		printf(" [%d] ", ptr->key);	  // 확인한다.
		recursiveInorder(ptr->right); // ptr이 NULL이면 중단한다.
	}
}

/**
 * textbook: p 224
 */
void iterativeInorder(Node *node)
{
	while (1) // 더이상 탐색할 노드가 없으면 종료한다.
	{
		for (; node; node = node->left) // 왼쪽 자식노드로 이동하면서 경로를 스택에 저장한다.
			push(node);
		node = pop();				 // 다음 확인할 노드로 이동한다.
									 // 왼쪽 노드가 모두 비어 있으면, 맨 마지막 왼쪽 노드일 것이고,
									 // 이 구간에서 탐색을 모두 완료한 것이면 부모 노드가 될 것이다.
		if (node == NULL)			 // 만일 node가 NULL이라면, pop에 실패했다는 것이므로
			return;					 // 더 이상 조사할 노드가 없음을 의미한다.
		printf(" [%d] ", node->key); // 확인한 노드를 출력한다.
		node = node->right;			 // 오른쪽 서브트리를 확인한다. 비어 있으면 NULL이 되어서
									 // 다음 반복 시 push 없이 pop만 하여 부모 노드를 탐색하게 될 것이고,
									 // 비어 있지 않으면 다시 왼쪽 노드들을 스택에 추가해 가며 연산할 것이다.
	}
}

/**
 * textbook: p 225
 */
void levelOrder(Node *ptr) // level order 방식: 큐에 추가-> 큐에서 제거하는 노드를 확인하고, 그 동시에
{						   // 제거하는 노드의 자식 노드들 추가. preorder 방식과 약간 유사.
	if (ptr == NULL)
	{
		printf("There is no tree.\n");
		return;
	}
	enQueue(ptr); // 헤드를 큐에 넣는다.
	while (1)
	{
		ptr = deQueue(); // 확인할 노드를 얻는다.
		if (ptr)
		{
			printf(" [%d] ", ptr->key);
			if (ptr->left) // 나중에 실행할 노드들을 큐에 추가한다.
				enQueue(ptr->left);
			if (ptr->right)
				enQueue(ptr->right);
		}
		else
			break; // 더 이상 꺼낼 큐의 원소가 없으면, 종료를 의미한다.
	}
}

int insert(Node *head, int key)
{
	Node *newNode = (Node *)malloc(sizeof(Node));
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;

	if (head->left == NULL)
	{
		head->left = newNode;
		return 1;
	}

	/* head->left is the root */
	Node *ptr = head->left;

	Node *parentNode = NULL;
	while (ptr != NULL)
	{

		/* if there is a node for the key, then just return */
		if (ptr->key == key)
			return 1;

		/* we have to move onto children nodes,
		 * keep tracking the parent using parentNode */
		parentNode = ptr;

		/* key comparison, if current node's key is greater than input key
		 * then the new node has to be inserted into the right subtree;
		 * otherwise the left subtree.
		 */
		if (ptr->key < key)
			ptr = ptr->right;
		else
			ptr = ptr->left;
	}

	/* linking the new node to the parent */
	if (parentNode->key > key)
		parentNode->left = newNode;
	else
		parentNode->right = newNode;
	return 1;
}

int deleteNode(Node *head, int key) // 말단이면 그냥 제거, 중간이면 그 서브트리 원소들을 유지하면서 올림.
{
	if (head == NULL) // 트리가 없을 때
	{
		printf("There is no tree.\n");
		return 1;
	}
	if (head->left == NULL) // 트리가 비었을 때
	{
		printf("Tree is empty.\n");
		return 1;
	}
	Node *find = head->left;
	Node *parent = NULL;
	while ((find != NULL) && (find->key != key)) // 다 찾아서 NULL이 되거나, 맞는 노드를 찾거나.
	{
		if (find->key != key) // 찾지 못한 경우
		{
			parent = find;			// 부모 위치 보관(나중에 노드 제거할 때 부모노드 링크 NULL로 바꿈.).
			if (find->key > key)	// 입력받은 값이 더 작으면,
				find = find->left;	// 왼쪽에 있을 것이라 예상.
			else					// 입력받으 값이 더 크면,
				find = find->right; // 오른쪽에 있을 것이라 예상.
		}
	}				  // 맞는 노드를 찾거나, 모든 노드를 찾으면 종료.
	if (find == NULL) // 못찾았으면
	{
		printf("There is no that key.\n");
		return 1;
	}
	if ((find->left == NULL) && (find->right == NULL)) // 리프노드인 경우
	{
		if (parent != NULL) // find가 루트 노드가 아님을 의미.
		{
			if (parent->left == find) // find 가리키는 부분 삭제.
				parent->left == NULL;
			else
				parent->right == NULL;
		}
		else
		{
			head->left == NULL;
		}
		free(find);
		return 0;
	}
	if ((find->left == NULL) || (find->right == NULL)) // 자식이 1개. 위에서 둘다 NULL인 경우는 제외.
	{
		Node *child;
		if (find->left != NULL)	 // 왼쪽에 노드를 가지면
			child = find->left;	 // 왼쪽 노드를 옮기기로 함.
		else					 // 오른쪽에 노드를 가지면
			child = find->right; // 오른쪽 노드를 옮기기로 함.
		if (parent != NULL)		 // 헤드노드가 아니면,
		{
			if (parent->left == find) // 왼쪽, 오른쪽에 삽입한다.
				parent->left = child;
			else
				parent->right = child;
		}
		else // 헤드노드이면,
		{
			head->left = child; // 헤드에 삽입해준다.
		}
		free(find); // 찾은 노드를 버린다.
		return 0;
	}
	// 자식이 2개인 경우
	Node *change = find->left; // 왼쪽 서브트리의 가장 큰 원소를 삽입하기로 한다.
	parent = find;
	while (change->right != NULL)
	{
		parent = change;
		change = change->right; // 왼쪽 서브트리에서 가장 큰 노드를 가리키게 한다.
	}
	if (change->left == NULL)		  // change가 왼쪽 서브트리를 가질 경우를 고려한다.
		parent->right = NULL;		  // 없으면, 그냥 NULL 처리해 버려도 된다.
	else							  // 하지만 있으면,
		parent->right = change->left; // 나머지들을 부모 노드의 오른쪽에 추가해 주어야 한다.
	find->key = change->key;		  // find를 지우고 모두 당기는 것보다,
	free(change);					  // 맨 뒤의 노드와 정보교환한 후 맨 뒤를 지우는 게 편하다.
	return 0;
}

void freeNode(Node *ptr)
{
	if (ptr)
	{
		freeNode(ptr->left);
		freeNode(ptr->right);
		free(ptr);
	}
}

int freeBST(Node *head)
{

	if (head->left == head)
	{
		free(head);
		return 1;
	}

	Node *p = head->left;

	freeNode(p);

	free(head);
	return 1;
}

Node *pop()
{
	if (top < 0)	 // 스택이 비어있으면, 뺄 수 없음.
		return NULL; // NULL을 출력
	return stack[top--];
} // 스택이 비어있지 않으면, 맨 위 값을 빼고 스택 크기를 하나 줄임.

void push(Node *aNode)
{
	if (top >= MAX_STACK_SIZE - 1) // 스택이 가득 차있으면(top이 19보다 크면)
		return;					   // 스택에 넣을 수 없다.
	stack[++top] = aNode;		   // 그게 아니면, 스택에 값을 넣는다.
}

Node *deQueue()
{
	if (front == rear) // 큐가 비어 있으면, 값을 빼낼 수 없음.
		return NULL;
	front = (front + 1) % MAX_QUEUE_SIZE; // front는 0~19 사이임을 기억하기.
	return queue[front];				  // 값을 빼내고, front값이 1 앞당겨졌다.
}

void enQueue(Node *aNode)
{
	rear = (rear + 1) % MAX_QUEUE_SIZE; // rear이 1 증가한다.(추가)
										// rear은 0~19 사이임을 기억하기.
	if (front == rear)					// 큐가 가득 차있으면(rear에서 1 더 가면 front이면)
		return;							// 스택에 넣을 수 없다.
	queue[rear] = aNode;
}

void printStack()
{
	if (top == -1) // 스택이 비었으면 출력하지 않는다.
	{
		printf("stack is empty.");
		return;
	}
	int i = 0;
	printf("stack\n");
	while (i <= top) // while문을 이용해 하나하나 프린트한다.
	{
		printf("stack[%d] = %d\n", i, stack[i]->key);
		i++;
	}
}