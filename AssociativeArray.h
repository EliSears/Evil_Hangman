#pragma once

#include "my_string.h"
#include "gen_vector.h"
#include "generic.h"
#include <stdio.h>

typedef void* A_ARRAY;




//Precondition: dictionary is a valid VECTOR of MY_STRINGs, phArr is a vaild pointer to 
//				a NULL pointer or A_ARRAY object, and key_char is a valid char
//Postcondition: returns void ptr to associative array
//               of keys to subfamilies
Status array_create_families(VECTOR dictionary, A_ARRAY* phArr, char key_char);

//Precondition: hAArray is a valid A_ARRAY object
//Postcondition:prints keys and sizes of each array
//              returns SUCCESS if successful, failure otherwise
Status array_list_keys(A_ARRAY hAArray);

//Precondition: hAArray is a valid A_ARRAY object and
//              key is a valid MY_STRING object
//Postcondition:returns VECTOR of MY_STRINGS with given key
//              or NULL if none found
VECTOR array_lookup_by_key(A_ARRAY hAArray, MY_STRING key);

//Precondition: hAArray is a valid A_ARRAY object
//Postcondition: returns string of key of most MY_STRING objects
char* array_max_family_key(A_ARRAY hAArray);

//Precondition: hAArray is a valid A_ARRAY object
//Postcondition: returns largest VECTOR of MY_STRINGS
VECTOR array_max_family(A_ARRAY hAArray);

//Precondition: hpAArray is a valid A_ARRAY object
//Postcondition: phAArray = NULL, object is freed
void array_destroy(A_ARRAY* phAArray);








