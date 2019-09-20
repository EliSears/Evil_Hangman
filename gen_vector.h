#include "generic.h"

typedef void* VECTOR;

//creates generic vector (vector of void pointers)
//returns void pointer to vector
VECTOR vector_init_default(void);

int vector_get_capacity(VECTOR hVector);

int vector_get_size(VECTOR hVector);

Item vector_get_data(VECTOR hVector, int index);

Status vector_push_back(VECTOR hVector, Item item);

Status vector_pop_back(VECTOR hVector);

void vector_destroy(Item* phVector);
