#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct NODE_s *NODE;
typedef struct NODE_s{
	NODE right;
	NODE left;
	int key;
	void *data;
} NODE_t[1];

typedef struct{
	NODE root;
} BST_t[1], *BST;

typedef struct LINKED_NODE_s
{
	struct LINKED_NODE_s *next;
	void * data;
} LINKED_NODE_t[1], *LINKED_NODE;

typedef struct LINKED_STACK_s
{
	LINKED_NODE head;
} LINKED_STACK_t[1], *LINKED_STACK;

LINKED_STACK linked_stack_init()
{
	LINKED_STACK stack = (LINKED_STACK)malloc(sizeof(LINKED_STACK_t));

	if(stack != NULL)
	{
		stack->head = NULL;
	}

	return stack;
}

LINKED_STACK push(LINKED_STACK stack, void* data)
{
	LINKED_NODE node = (LINKED_NODE)malloc(sizeof(LINKED_NODE_t));

	node->data = data;
	node->next = NULL;

	if(stack != NULL)
	{
		if(stack->head == NULL)
		{
			stack->head = node;
		}
		else{
			node->next = stack->head;
			stack->head = node;
		}
	}

	return stack;
}

void* pop(LINKED_STACK stack)
{
	void *data;

	if(stack != NULL)
	{
		if(stack->head != NULL)
		{
			LINKED_NODE tmp = stack->head;

			data = tmp->data;

			stack->head = tmp->next;

			free(tmp);
		}
		else printf("\nEmpty stack\n");
	}

	return data;
}

void inorder(NODE node){
	if(node != NULL){
		inorder(node->left);
		printf("%d ", node->key);
		inorder(node->right);
	}
}

void preorder(NODE node){
	if(node != NULL){
		preorder(node->right);
		printf("%d ", node->key);
		preorder(node->left);
	}
}

void inorder_iterative(NODE node)
{
	LINKED_STACK stack = linked_stack_init();

	NODE iter = node;

	push(stack, (NODE)node);

	while(stack->head != NULL)
	{
		iter = (NODE)pop(stack);

		printf("%d ", iter->key);

		if(iter->left != NULL)
			push(stack, (NODE)iter->left);
		
		if(iter->right != NULL)
			push(stack, (NODE)iter->right);	
			
		free(iter);
	}

	free(stack);
}

void bst_print(NODE node, int l){
	int i;
	if(node != NULL){
		bst_print(node->right, l+1);
		for (i = 0; i < l; ++i) {
			printf("     ");
		}
		printf("%d\n", node->key);
		bst_print(node->left, l+1);
	}
}

BST bst_init(){
	BST t = (BST)malloc(sizeof(BST_t));
	t->root = NULL;
	return t;
}

void bst_free(BST t){
	//Think about what happens to the nodes!?
	free(t);
}

NODE bst_init_node(int key, void *data){
	NODE node = (NODE)malloc(sizeof(NODE_t));
	node->left = NULL;
	node->right = NULL;
	node->key = key;
	node->data = data;
	return node;
}

void bst_free_node(NODE n){
	free(n);
}

/*
void bst_insert(BST t, int key, void *data){
	if(t == NULL){
		printf("ERROR. Uninitialized tree\n");
	}
	else{
		if(t->root == NULL){
			t->root = bst_init_node(key, data);
		}
		else{
			NODE node = t->root;
			while(node != NULL){
				if(key < node->key){
					if(node->left == NULL){
						node->left = bst_init_node(key, data);
						node = NULL;
					}
					else{
						node = node->left;
					}
				}
				else if(key > node->key){
					if(node->right == NULL){
						node->right = bst_init_node(key, data);
						node = NULL;
					}
					else{
						node = node->right;
					}
				}
				else{
					printf("Duplicate key: %d. Ignoring...\n\n\n", key);
					node = NULL;
				}
			}
		}
	}
}
*/

NODE bst_insert_rec(NODE node, int key, void *data){
	if(node == NULL){
		node = bst_init_node(key, data);
	}
	else{
		if(key < node->key){
			node->left = bst_insert_rec(node->left, key, data);
		}
		else if(key > node->key){
			node->right = bst_insert_rec(node->right, key, data);
		}
		else{
			printf("Duplicate key: %d. Ignoring...\n\n\n", key);
		}
	}
	return node;
}

void bst_insert(BST t, int key, void *data){
	if(t == NULL){
		printf("ERROR. Uninitialized tree\n");
	}
	else{
		if(t->root == NULL){
			t->root = bst_init_node(key, data);
		}
		else{
			t->root = bst_insert_rec(t->root, key, data);
		}
	}
}

void bst_delete_unbalanced_iterative(BST t, int key, void *data){
	if(t == NULL){
		printf("ERROR. Uninitialized tree\n");
	}
	else{

		NODE parent = (NODE)t;
		NODE curr = t->root;
		while((curr != NULL) && (key != curr->key)){
			if(key < curr->key){
				parent = curr;
				curr = curr->left;
			}
			else if(key > curr->key){
				parent = curr;
				curr = curr->right;
			}
		}

		if(curr == NULL){
			printf("The key is not found.\n");
		}
		else{
			/* NO CHILD */
			if( (curr->left == NULL) && (curr->right == NULL) ){
				if(curr == parent->left){
					parent->left = NULL;
				}
				else{
					parent->right = NULL;
				}
				//bst_free_node(curr);
				free(curr->data); /* !? */
				free(curr);

			}
			/* SINGLE CHILD ON THE RIGHT */
			else if( (curr->left == NULL) && (curr->right != NULL) ){

				if(curr == parent->left){
					parent->left = curr->right;
				}
				else{
					parent->right = curr->right;
				}
			}
			/* SINGLE CHILD ON THE LEFT */
			else if( (curr->left != NULL) && (curr->right == NULL) ){

				if(curr == parent->left){
					parent->left = curr->left;
				}
				else{
					parent->right = curr->left;
				}
			}
			/* TWO CHILDREN */
			else{

				NODE max = curr->left;
				while(max->right != NULL){
					max = max->right;
				}
				max->right = curr->right;

				if(curr == parent->left){
					parent->left = curr->left;
				}
				else{
					parent->right = curr->left;
				}
			}
		}
	}
}

void bst_delete_balanced_recursive(NODE curr, NODE parent, int key, void *data)
{
	if(curr == NULL){
		printf("ERROR. Uninitialized tree\n");
	}
	else{
		if(curr == NULL)
			return;

		if(key < curr->key){
			parent = curr;
			bst_delete_balanced_recursive(curr->left, parent, key, data);
		}
		else if(key > curr->key){
			parent = curr;
			bst_delete_balanced_recursive(curr->right, parent, key, data);
		}

		if(key == curr->key)	
		{
			/* NO CHILD */
			if( (curr->left == NULL) && (curr->right == NULL) ){

				if(curr == parent->left){
					parent->left = NULL;
				}
				else{
					parent->right = NULL;
				}

				//bst_free_node(curr);
				free(curr->data); /* !? */
				free(curr);
				return;
			}

			/* SINGLE CHILD ON THE RIGHT */
			else if( (curr->left == NULL) && (curr->right != NULL) ){

				if(curr == parent->left){
					parent->left = curr->right;
				}
				else{
					parent->right = curr->right;
				}

				return;
			}
			/* SINGLE CHILD ON THE LEFT */
			else if( (curr->left != NULL) && (curr->right == NULL) ){

				if(curr == parent->left){
					parent->left = curr->left;
				}
				else{
					parent->right = curr->left;
				}

				return;
			}

			/* TWO CHILDREN */
			else{
				NODE max = curr->left;
				NODE prevMax;

				while(max->right != NULL){
					prevMax = max;
					max = max->right;
				}
				prevMax->right = NULL;

				max->right = curr->right;

				if(curr == parent->left){
					parent->left = max;
				}
				else{
					parent->right = max;
				}

				max->left = curr->left;
				
			}
		}
	}
}

void backbone(BST tree)
{
    NODE gp = (NODE)tree;
    NODE parent = tree->root;

    while(parent != NULL)
    {
        if(parent->left != NULL)
        {
            NODE child = parent->left;
            
            parent->left = child->right;
            child->right = parent;

            gp->right = child; 
        }

        else
		{
        gp = parent; 
        }

        parent = gp->right;
    }
}

int powOf2(int n)
{
    int result = 1; 
    for (int i = 0; i < n; i++)
    {
        result *= 2;
    }
    
    return result;
}

int floorLogarithm(int n)
{
    int result = 2, i;
    for (i = 0; result <= n; i++)
    {
        result *= 2;
    }
    return i;
}

int getDataCount(BST tree)
{
    NODE iter = tree->root;
    int counter = 0;
    while(iter != NULL)
    {
        iter = iter->right;
        counter++;
    }
    return counter;
}

void leftRotation(NODE gp, NODE p , NODE c)
{
    NODE temp = c->left;
    c->left = p;
    p->right = temp;
    gp->right = c;
}

void makeLeftRotations(BST tree, int size)
{
    NODE gp = (NODE)tree;
    NODE p = tree->root;
    NODE c = p->right;

    while(size > 0)
    {
        leftRotation(gp, p ,c);


        gp = c;
        p = gp->right;
        c = p->right;

        size--;
    }
}

void balance(BST tree)
{
    int counter = getDataCount(tree);

    int m = powOf2(floorLogarithm(counter+1)) - 1;
    int k = counter - m;

    makeLeftRotations(tree, k);

	printf("k : %d \n", k);

    while(m > 1)
    {
        m /= 2;
		printf("m: %d \n", m);
        makeLeftRotations(tree, m);
    }
}

int main() {
	BST t1 = bst_init();
	
	bst_insert(t1, 1, NULL);
	bst_insert(t1, 3, NULL);
	bst_insert(t1, 10, NULL);
	bst_insert(t1, 15, NULL);
	bst_insert(t1, 23, NULL);
	bst_insert(t1, 35, NULL);
	bst_insert(t1, 38, NULL);
	bst_insert(t1, 39, NULL);
	bst_insert(t1, 38, NULL);
	bst_insert(t1, 40, NULL);

	bst_insert(t1, 42, NULL);
	bst_insert(t1, 63, NULL);
	bst_insert(t1, 67, NULL);
	bst_insert(t1, 82, NULL);

	printf("original : \n");	
    bst_print(t1->root, 0);

	int chosen = 40;
	//printf("\n\n%d deleted version: \n\n", chosen);

	// printf("backbone : \n");	
	// backbone(t1);
	// bst_print(t1->root, 0);

	printf("\n\n\nbalanced : \n");	
	balance(t1);
	bst_print(t1->root, 0);

	printf("\nPreorder: \n");
	preorder(t1->root);

	printf("\nInorder: \n");
	inorder(t1->root);

	//bst_delete_unbalanced_iterative(t1, chosen,  NULL);
	printf("\n\ninorder Iterative\n");
	inorder_iterative(t1->root);

	return 0;
}