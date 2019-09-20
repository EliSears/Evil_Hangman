#include "AssociativeArray.h"
#include <stdlib.h>



typedef struct node Node;
struct node{  //node for avl tree
  int height; //balance factor
  unsigned int key; //key to match strings to and sort by
  VECTOR family; //VECTOR holding all matched strings
  Node* left;  //ptr to left branch
  Node* right;  //ptr to right branch
};
typedef struct arr AArr;
struct arr{
  Node* root;
  VECTOR nodes;
  char key_char;
};

A_ARRAY array_init_default(void);
Node* array_create_node(unsigned int key);
Status array_insert_string(Node* pRoot, MY_STRING hString, unsigned int key);
Node* array_AVL_insert_node(Node* pRoot, Node* new_node);
int AVL_update_height(Node* pRoot);
Node* AVL_rotate_left(Node* pRoot);
Node* AVL_rotate_right(Node* pRoot);
Node* AVL_balance(Node* pRoot);
int AVL_height(Node* pRoot);
Status array_sort(VECTOR* nodes_arr);
void array_quicksort(Node** nodes_array, int size);
void swap(Node** a, Node** b);
VECTOR lookup_by_key(Node* pRoot, unsigned int key);
void array_node_destroy(Node* pNode);
char* array_key_string_convert(unsigned int bitKey, AArr* pArr);
unsigned int array_bit_key(MY_STRING word, char key_char);


A_ARRAY array_init_default(void){  //create a_array object
  AArr* pArr = NULL;
  pArr = (AArr*)malloc(sizeof(AArr));
  if(pArr == NULL)
    exit(1);
  pArr->root = NULL;
  pArr->key_char;
  pArr->nodes = vector_init_default();
  return pArr;
}

Status array_create_families(VECTOR dictionary, A_ARRAY* phArr, char key_char){
	AArr* pArr = (AArr*)*phArr; //convert from void*
	Node* temp_node = NULL;
	unsigned int temp_key = 0; 
	  
	if (pArr != NULL) {//if a_array is nonNULL
		array_destroy(phArr); //start from scratch (because its faster to anyway)
	}
	*phArr = array_init_default();
	pArr = (AArr*)*phArr; //convert from void*
	pArr->key_char = key_char;
	
  
  
  for (int i = 0; i < vector_get_size(dictionary); ++i) {  //sort dictionary into subfamilies

	  temp_key = 0;
	  temp_key = array_bit_key(vector_get_data(dictionary, i), key_char);

	  
	  //if subfamily doesn't exist yet->make it and add it to vector
	  if (array_insert_string(pArr->root, vector_get_data(dictionary, i), temp_key) == FAILURE) {
		  temp_node = array_create_node(temp_key);		//make node for AVL tree
		  vector_push_back(temp_node->family, vector_get_data(dictionary, i)); //add string to node's array
		  pArr->root = array_AVL_insert_node(pArr->root, temp_node);	//insert node into tree
		  vector_push_back(pArr->nodes, temp_node); //add node to nodes vector for future sorting
	  }
	  else
		  temp_key = 0; //if key not used in a new node->reset it
  }

  
  if (array_sort(&(pArr->nodes)) == SUCCESS) {
	  return SUCCESS;
  }
  else
	  return FAILURE;
}

Node* array_create_node(unsigned int key){  //creates node for avl tree
  Node* pNode = NULL;
  pNode = (Node*)malloc(sizeof(Node));
  if(pNode == NULL)
    exit(1);
  pNode->key = key;
  pNode->height = 1;
  pNode->left = NULL;
  pNode->right = NULL;
  pNode->family = vector_init_default();
  return pNode;
}

Status array_insert_string(Node* pRoot, MY_STRING hString, unsigned int key){
	if(pRoot == NULL){
		return FAILURE;
	}
	else{
		if (key == pRoot->key) {
			my_string_get_size(hString);
			vector_push_back(pRoot->family, hString);
		}
		else if(key < pRoot->key){
			return array_insert_string(pRoot->left, hString, key);
    }
		else if(key > pRoot->key){
			return array_insert_string(pRoot->right, hString, key);
		}
	}
	return SUCCESS;
}

Node* array_AVL_insert_node(Node* pRoot, Node* new_node){
	if(pRoot == NULL)
		pRoot = new_node;
	else{
		if(new_node->key < pRoot->key){
			pRoot->left = array_AVL_insert_node(pRoot->left, new_node);      
		}
		else if(new_node->key > pRoot->key){
			pRoot->right = array_AVL_insert_node(pRoot->right, new_node);
		}
		else {
			printf("ERROR: NEW NODE WITHOUT UNIQUE KEY");
			exit(1);
		}
	}
  return AVL_balance(pRoot);
}

int AVL_update_height(Node* pRoot){
	if (AVL_height(pRoot->left) > AVL_height(pRoot->right))
		return 1 + AVL_height(pRoot->left);
	else
		return 1 + AVL_height(pRoot->right);
}


Node* AVL_rotate_left(Node* pRoot){
	Node* temp_right = pRoot->right;
	pRoot->right = temp_right->left;
	temp_right->left = pRoot;
	pRoot->height = AVL_update_height(pRoot);
	temp_right->height = AVL_update_height(temp_right);

	return temp_right;
}

Node* AVL_rotate_right(Node* pRoot){ 
  Node* temp_left = pRoot->left; 
  pRoot->left = temp_left->right;
  temp_left->right = pRoot;
  pRoot->height = AVL_update_height(pRoot);
  temp_left->height = AVL_update_height(temp_left);
  
  return temp_left;
}

Node* AVL_balance(Node* pRoot) {
	pRoot->height = AVL_update_height(pRoot);

	if (AVL_height(pRoot->left) - AVL_height(pRoot->right) == 2)
	{
		if (AVL_height(pRoot->left->right) > AVL_height(pRoot->left->left))
			pRoot->left = AVL_rotate_left(pRoot->left);
		return AVL_rotate_right(pRoot);
	}
	else if (AVL_height(pRoot->right) - AVL_height(pRoot->left) == 2)
	{
		if (AVL_height(pRoot->right->left) > AVL_height(pRoot->right->right))
			pRoot->right = AVL_rotate_right(pRoot->right);
		return AVL_rotate_left(pRoot);
	}
	return pRoot;
}

int AVL_height(Node* pRoot) {
	if (pRoot == NULL)
		return 0;
	return pRoot->height;
}

Status array_sort(VECTOR* nodes_arr){
  Node** pArr = NULL;
  int size = vector_get_size(*nodes_arr);
  pArr = (Node**)malloc(sizeof(Node*)*size);
  if(pArr ==  NULL)
    exit(1);
  for(int i = 0; i < size; ++i){
    pArr[i] = vector_get_data(*nodes_arr,i);
  }

  vector_destroy(nodes_arr);		//reset vector
  *nodes_arr = vector_init_default();
  
  array_quicksort(pArr, size);

  for(int i = 0; i < size; ++i){
    vector_push_back(*nodes_arr, pArr[i]);
  }
  free(pArr);
  if(vector_get_size(*nodes_arr) == size)
    return SUCCESS;
  
  return FAILURE;
}

void array_quicksort(Node** nodes_array, int size){
  Node** pivot = &(nodes_array[0]);
  Node** left = pivot;
  Node** right = &(nodes_array[size-1]);
  if(size < 2)
    return;
  while(left < right){
      while(left < right && vector_get_size((*pivot)->family) < vector_get_size((*right)->family)){
		right--;
      }
      while(left < right && vector_get_size((*left)->family) <= vector_get_size((*pivot)->family)){
		left++;
      }
      if(left != right)
	swap(left, right);
  }
  swap(pivot, left);

  array_quicksort(nodes_array, left - nodes_array);     //recursive left side
  array_quicksort(&nodes_array[left - nodes_array + 1], &nodes_array[size - 1] - right);// recursive right side
}

void swap(Node** a, Node** b){
  Node* temp = *a;
  *a = *b;
  *b = temp;
}
  

Status array_list_keys(A_ARRAY hAArray){
  AArr* pArr = (AArr*)hAArray;
  for(int i = 0; i < vector_get_size(pArr->nodes); ++i){
    printf("[%s] %d members\n", array_key_string_convert(((Node*)vector_get_data(pArr->nodes,i))->key, pArr),
	   vector_get_size(((Node*)vector_get_data(pArr->nodes,i))->family));
  }
  return SUCCESS;
}


VECTOR array_lookup_by_key(A_ARRAY hAArray, MY_STRING key){
  AArr* pArr = (AArr*)hAArray;
  unsigned int bitKey = array_bit_key(key, pArr->key_char);
  return lookup_by_key(pArr->root, bitKey);
}

VECTOR lookup_by_key(Node* pRoot, unsigned int key){
  if (pRoot == NULL)
	  return NULL;
  if(key == pRoot->key)
    return pRoot->family;
  if(key < pRoot->key)
    return lookup_by_key(pRoot->left, key);
  if(key > pRoot->key)
    return lookup_by_key(pRoot->right, key);
  
  return NULL;
}

char* array_max_family_key(A_ARRAY hAArray){
  AArr* pArr = (AArr*)hAArray;
  unsigned int maxKey = ((Node*)vector_get_data(pArr->nodes, vector_get_size(pArr->nodes) - 1))->key;
  char* sKey = array_key_string_convert(maxKey, pArr);
  return sKey;
}

VECTOR array_max_family(A_ARRAY hAArray){  
  AArr* pArr = (AArr*)hAArray;
  int size = vector_get_size(pArr->nodes);
  Node* largest_node = vector_get_data(pArr->nodes, (size-1));
  return largest_node->family;
}

void array_node_destroy(Node* pNode){
  vector_destroy(&(pNode->family));
  free(pNode);
}

void array_destroy(A_ARRAY* phAArray){
  AArr* pArr = (AArr*)*phAArray;
  Node* temp = NULL;
  for(int i=0; i < vector_get_size(pArr->nodes); ++i){
    temp = vector_get_data(pArr->nodes, i);
    array_node_destroy(temp);
  }
  vector_destroy(&(pArr->nodes));
  free(pArr);
  *phAArray = NULL;
}

char* array_key_string_convert(unsigned int bitKey, AArr* pArr) {
	unsigned int checker;
	char key_char = pArr->key_char;
	MY_STRING word = vector_get_data(lookup_by_key(pArr->root, bitKey), 0);
	int size = my_string_get_size(word);
	char* sKey = my_string_c_str(word);

	for (int i = 0; i < size; ++i) {
		checker = 1 << i;
		if (!(bitKey & checker))
			sKey[i] = '-';
	}

	return sKey;
}

unsigned int array_bit_key(MY_STRING word, char key_char) {
	char* temp = my_string_c_str(word);
	int size = my_string_get_size(word);
	unsigned int bit_key, insert;
	bit_key = 0;
	for(int i = 0; i < size; ++i){
		if(key_char == temp[i]){
			insert = 1 << i;
			bit_key = bit_key | insert;
		}
	}
	return bit_key;
}