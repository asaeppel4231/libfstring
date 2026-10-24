#include "fstring.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

#define FSTRING_IS_FLAT_COPY 0
#define FSTRING_IS_DEEP_COPY 1

#define FSTRING_IS_HEAP 1
#define FSTRING_NOT_HEAP 0

#define MAX_DATA_APPROXIMATED_SIZE 1 * 1024 * 1024 * 1024 /* = 1 GiB */

struct fstring {
  char* data;
  size_t length;
  size_t capacity;
  unsigned int readonly : 1;;
  unsigned int flat_copy: 1;
  unsigned int data_is_on_heap : 1;
  unsigned int : 5;
};

fstr_t* create_fstring(){
  fstr_t* fstring = malloc(sizeof(*fstring));

  if(fstring == NULL){
    return NULL;
  }

  memset(fstring, 0, sizeof(*fstring));

  return fstring;
}

char* destroy_fstring(fstr_t* fstr, int flags){
  if(fstr == NULL){
    return NULL;
  }

  if(flags & FSTRING_FREE_FLAG_SKIP_DATA){
    char* data = fstr->data;
    free(fstr);
    return data;
  }

  else if(fstr->data_is_on_heap && fstr->data != NULL){
    free(fstr->data);
  }

  free(fstr);

  return NULL;
}

int set_fstring_cstring(fstr_t *fstr, char *cstring, int flags){
  if(fstr == NULL || flags == 0){
    return 0;
  }

  if(fstr->data_is_on_heap && fstr->data != NULL && fstr->length > 0){
    free(fstr->data);
  }
  
  size_t length = strlen(cstring);

  if(flags & FSTRING_CSTRING_FLAG_FLAT_COPY){
    fstr->data = cstring;
    fstr->length = length;
    fstr->flat_copy = FSTRING_IS_FLAT_COPY;
    if(flags & FSTRING_CSTRING_FLAG_IS_WRITEABLE){
      fstr->readonly = FSTRING_NOT_RO;
    }else{
      fstr->readonly = FSTRING_IS_RO;
    }
    if(flags & FSTRING_CSTRING_FLAG_IS_HEAP){
      fstr->data_is_on_heap = FSTRING_IS_HEAP;
    }else{
      fstr->data_is_on_heap = FSTRING_NOT_HEAP;
    }

    fstr->capacity = 0;
  }
  else if(flags & FSTRING_CSTRING_FLAG_DEEP_COPY){
    char* data = malloc(length + 1);

    if(data == NULL){
      return 0;
    }

    memcpy(data, cstring, length + 1); /* Copy also nullbyte */

    fstr->data = data;
    fstr->length = length;
    fstr->capacity = length;
    fstr->flat_copy = FSTRING_IS_DEEP_COPY;
    fstr->readonly = FSTRING_NOT_RO;
    fstr->data_is_on_heap = FSTRING_IS_HEAP;
  }
  
  return 1;
}


int concat_fstr(fstr_t* src, fstr_t* dest, int flags){
  if(src == NULL || dest == NULL){
    return 0;
  }

  if(dest->readonly == FSTRING_IS_RO || dest->flat_copy == FSTRING_IS_FLAT_COPY){
    return -1;
  }

  size_t new_length = src->length + dest->length;

  if(new_length < 1){ 
    return -1;
  }

  if(new_length > dest->capacity && dest->data_is_on_heap == FSTRING_IS_HEAP){
    size_t new_size = (dest->capacity * 2 + 1);

    if(new_size > MAX_DATA_APPROXIMATED_SIZE){
      new_size = new_length + 1;
    }
#include <stdio.h>
    printf("Allocation %zu bytes of data\n", new_size);
    char* new_data = realloc(dest->data, new_size);

    if(new_data == NULL){
      return -2;
    }

    dest->data = new_data;
    dest->capacity = new_size;

    goto success;
  }
  else if(new_length <= dest->capacity){
    goto success;
  }
  else{
    return -3;
  }
  success:
    memcpy(dest->data + dest->length, src->data, src->length);
    *(dest->data + new_length) = '\0';
    dest->length = new_length;
  return 1;
}

int compare_fstr(fstr_t* fstr1, fstr_t* fstr2){
  if(fstr1 == NULL || fstr2 == NULL){
    return -2;
  }

  if(fstr1->length > fstr2->length){
    return 1;
  }

  if(fstr1->length < fstr2->length){
    return -1;
  }


  char* data = fstr1->data;
  char* data2 = fstr2->data;

  while(*data != '\0'){
    if(*data > *data2){
      return 1;
    }
    else if(*data < *data2){
      return -1;
    }
    data++;
    data2++;
  }

  return 0;
}

fstr_t* tokenize_fstr(fstr_t* fstr, char token, size_t offset){
  if(fstr == NULL){
    return NULL;
  }

  if(offset > fstr->length + 1){
    return NULL;
  }
  
  char* data = fstr->data + offset;
  char* temp = data;

  while(*temp != '\0'){
    if(*temp == token){
      break;
    }
    temp++;
  }

  if(temp == data){
    return NULL;
  }

  fstr_t* result = create_fstring();

  if(result == NULL){
    return NULL;
  }

  result->data = data;
  result->length = temp - data;
  result->capacity = 0;
  result->readonly = FSTRING_IS_RO;
  result->data_is_on_heap = FSTRING_NOT_HEAP;
  result->flat_copy = FSTRING_IS_FLAT_COPY;

  return result;
}


int make_fstring_data_unique(fstr_t* fstr){
  if(fstr == NULL){
    return 0;
  }

  if(fstr->data_is_on_heap != FSTRING_IS_HEAP){
    char* data = malloc(fstr->length + 1);

    if(data == NULL){
      return 0;
    }

    memcpy(data, fstr->data, fstr->length);
    *(data + fstr->length) = '\0';
    fstr->data = data;

    fstr->data_is_on_heap = FSTRING_IS_HEAP;
  }

  return 1;
}

char* get_fstring_cstring(fstr_t* fstr){
  return fstr == NULL ? NULL : fstr->data;  
}

size_t get_fstring_length(fstr_t *fstr){
  return fstr == NULL ? 0 : fstr->length;
}
