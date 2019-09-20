#include "gen_vector.h"
#include <stdio.h>
#include <stdlib.h>

struct vector{
  Item* data;
  int capacity;
  int size;
};

typedef struct vector Vector;

Status vector_realloc_memory(VECTOR hVector, int mincap);

VECTOR vector_init_default(void){
  Vector* pVector = NULL;
  pVector = (Vector*)malloc(sizeof(Vector));
  if(pVector != NULL){
    pVector->size = 0;
    pVector->capacity = 5;
    pVector->data = (Item*)malloc(sizeof(Item)*pVector->capacity);

    if(pVector->data != NULL){
      return (VECTOR)pVector;
    }
    return NULL;
  }
  return NULL;
}

Status vector_realloc_memory(VECTOR hVector, int mincap){
  Vector* pVector = (Vector*)hVector;
  Item* temp = NULL;

  mincap = mincap*2; //new capcity for vector
  temp = (Item*)malloc(sizeof(Item)*mincap);
  if(temp != NULL){
    for(int i = 0; i < pVector->size; ++i){
      temp[i] = pVector->data[i];
    }
    free(pVector->data);
    pVector->data = temp;
    pVector->capacity = mincap;
    return SUCCESS;
  }
  return FAILURE;
}

int vector_get_capacity(VECTOR hVector){
  Vector* pVector = (Vector*)hVector;
  return pVector->capacity;
}

int vector_get_size(VECTOR hVector){
  Vector* pVector = (Vector*)hVector;
  return pVector->size;
}

Item vector_get_data(VECTOR hVector, int index){
  Vector* pVector = (Vector*)hVector;
  if(index >= pVector->size){
    printf("Index: %d\t\tSize: %d\n",index, pVector->size);
    printf("TOOBIG\n");
    return NULL;
  }
  else{
    // printf("ROIGHTSIZE\n");
    return pVector->data[index];
  }
}

Status vector_push_back(VECTOR hVector, Item item ){
  Vector* pVector = (Vector*)hVector;
  if (pVector->size >= pVector->capacity){  //if not enough space allocate more memory
    if(vector_realloc_memory(hVector, pVector->size) == FAILURE)
      return FAILURE;
  }
  pVector->data[pVector->size] = item;
  pVector->size++;
  return SUCCESS;
}

Status vector_pop_back(VECTOR hVector){
  Vector* pVector = (Vector*)hVector;
  if(pVector->size > 0){
    pVector->size--;
    return SUCCESS;
  }
  return FAILURE;
}

void vector_destroy(VECTOR* phVector){
  Vector* pVector = (Vector*)*phVector;
  if(pVector == NULL){
    printf("cannot dealloc\n");
    return;
  }
  free(pVector->data);
  free(pVector);
  phVector = NULL;
}

