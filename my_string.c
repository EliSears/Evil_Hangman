#include <stdio.h>
#include <stdlib.h>
#include "my_string.h"

//structure = {SIZE,CAPACITY,STRING}
//SIZE = number of characters contained
//CAPACITY = total possible number of characters
//STRING = stored data of length SIZE

struct string{
  int size;
  int capacity;
  char* data;
};
typedef struct string String;

Status my_string_realloc_memory(MY_STRING hMy_string, int mincap);   //internal function for memory reallocation

MY_STRING my_string_init_default(void){
  String* pMy_string = NULL;
  pMy_string = (String*)malloc(sizeof(String));
  
  if(pMy_string != NULL){
    pMy_string->size = 0;
    pMy_string->capacity = 7;
    pMy_string->data = (char*)malloc(sizeof(char)*pMy_string->capacity);

    if(pMy_string->data != NULL){
      return (MY_STRING)pMy_string;
    }
    return NULL;
   
  }
  return NULL;
}

MY_STRING my_string_init_c_string(const char* c_string){
  String* pMy_string = NULL;
  pMy_string = (String*)malloc(sizeof(String));
  int str_size = 0;
  
  while (c_string[str_size] != '\0'){
    ++str_size;
  }
  if(pMy_string != NULL){
    pMy_string->size = str_size;
    pMy_string->capacity = str_size+1;
    pMy_string->data = NULL;

    pMy_string->data = (char*)malloc(sizeof(char)*pMy_string->capacity);

    if(pMy_string->data != NULL){
      for(int i = 0; i < pMy_string->size; ++i){
	pMy_string->data[i] = c_string[i];
      }
      return (MY_STRING)pMy_string;
    }
    return NULL;
  }
  return NULL;
}

Status my_string_realloc_memory(MY_STRING hMy_string, int mincap){
  String* pMy_string = (String*)hMy_string;
  char* temp = NULL;
  
  mincap = mincap*2;

  temp = (char*)malloc(sizeof(char)*mincap);
  if(temp != NULL){
    for(int i = 0; i < pMy_string->size; ++i){
      temp[i] = pMy_string->data[i];
    }
    free(pMy_string->data);
    pMy_string->data = temp;
    pMy_string->capacity = mincap;
    return SUCCESS;
  }
  return FAILURE;
}

int my_string_get_capacity(MY_STRING hMy_string){
  String* pMy_string = (String*)hMy_string;
  return pMy_string->capacity;
}

int my_string_get_size(MY_STRING hMy_string){
  String* pMy_string = (String*)hMy_string;
  return pMy_string->size;
}

int my_string_compare(MY_STRING hLeft_string, MY_STRING hRight_string){
  String* pLstring = (String*)hLeft_string;
  String* pRstring = (String*)hRight_string;
  //printf("%s\t%s\n",pRstring->data, pLstring->data);
  int i = 0;
  while((pRstring->size)-1 >= i && (pLstring->size)-1 >= i){
    if(pRstring->data[i] > pLstring->data[i])       // R is greater
      return -1;
    else if(pRstring->data[i] < pLstring->data[i])  // L is greater
      return 1;
    else                           //character are equal
      ++i;
  }
  if(pRstring->size > pLstring->size)  //strings are equal but R is longer
    return -1;
  else if(pRstring->size < pLstring->size)  // '' '' but L is longer
    return 1;
  else                                 //R==L
  return 0;
}

Status my_string_extraction(MY_STRING hMy_string, FILE* fp){
  char word[100];
  Status status = FAILURE;
  if(fscanf(fp,"%s",word) && !feof(fp)){
    for(int i = 0; word[i] != '\0'; ++i){
		if(word[i] >= 0 && word[i] <= 127) //make sure its ascii characters
			my_string_push_back(hMy_string,word[i]);
    }
    status = SUCCESS;
  }
  return status;
}

Status my_string_insertion(MY_STRING hMy_string, FILE* fp){
  String* pMy_string = (String*)hMy_string;
  if(fprintf(fp,"%s", pMy_string->data))
    return SUCCESS;
  return FAILURE;
}

void my_string_destroy(Item* pItem){
  String* pMy_string = (String*)*pItem;
  free(pMy_string->data);
  free(pMy_string);
  *pItem = NULL;
}

Status my_string_push_back(MY_STRING hMy_string, char item){
  String* pMy_string = (String*)hMy_string;
  if (pMy_string->size >= pMy_string->capacity){  //if not enough space allocate more memory
    if(my_string_realloc_memory(hMy_string, pMy_string->size) == FAILURE)
      return FAILURE;
  }
  pMy_string->data[pMy_string->size] = item;
  ++pMy_string->size;
  return SUCCESS;
}

Status my_string_pop_back(MY_STRING hMy_string){
  String* pMy_string = (String*)hMy_string;
  if(pMy_string->size > 0){
    --pMy_string->size;
    return SUCCESS;
  }
  return FAILURE;
}

char* my_string_at(MY_STRING hMy_string, int index){
  String* pMy_string = (String*)hMy_string;
  if(index >= pMy_string->size){  //if out of bounds return NULL
    return NULL;
  }
  return &pMy_string->data[index];   //location of character
}

char* my_string_c_str(MY_STRING hMy_string){
  String* pMy_string = (String*)hMy_string;
  if (pMy_string->size >= pMy_string->capacity){  //if not enough space allocate more memory
    if(my_string_realloc_memory(hMy_string, pMy_string->size) == FAILURE)
      return NULL;
  }
  pMy_string->data[pMy_string->size] = '\0';   //add null terminator

  return pMy_string->data;    //return data
}

Status my_string_concat(MY_STRING hResult, MY_STRING hAppend){
    String* pResult = (String*)hResult;
    String* pAppend = (String*)hAppend;
    int i = pResult->size;
    int j;

    if((pResult->size + pAppend->size)>= pResult->capacity){//if not enough space allocate more memory
      if(my_string_realloc_memory(hResult, (pResult->size + pAppend->size)) == FAILURE)
	return FAILURE;
    }
    for(j=0; j < pAppend->size; ++i, ++j){//add append to result
      pResult->data[i] = pAppend->data[j];
      pResult->size = i+1;
    }
   
    return SUCCESS;
}

Boolean my_string_empty(MY_STRING hMy_string){
  String* pMy_string = (String*)hMy_string;
  if (pMy_string->size < 1)   //if string size in 0 or less -> string is empty
    return TRUE;

  return FALSE;
}

void my_string_assignment(Item* pLeft, Item Right){
  String* temp = NULL;
  String* pRString = (String*)Right;
  if (*pLeft != NULL)
	  my_string_destroy(pLeft);

  temp = my_string_init_c_string(my_string_c_str(pRString));

  if(my_string_compare(temp, pRString) == 0)
   *pLeft = temp;
}

