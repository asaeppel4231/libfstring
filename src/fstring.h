#ifndef FSTRING_H
#define FSTRING_H

#include <stddef.h>

typedef struct fstring fstr_t;

#define FSTRING_NOT_RO 1
#define FSTRING_IS_RO 0

#define FSTRING_FREE_FLAG_SKIP_DATA 1 << 0

#define FSTRING_CSTRING_FLAG_FLAT_COPY    1 << 0
#define FSTRING_CSTRING_FLAG_DEEP_COPY    1 << 1
#define FSTRING_CSTRING_FLAG_IS_WRITEABLE 1 << 2
#define FSTRING_CSTRING_FLAG_IS_HEAP      1 << 3
#define FSTRING_CSTRING_FLAG_IS_VIEW      1 << 4

fstr_t* create_fstring();
char* destroy_fstring(fstr_t* fstr, int flags);

int set_fstring_cstring(fstr_t* fstr, char* cstring, int flags);


int concat_fstr(fstr_t* src, fstr_t* dest, int flags);
int compare_fstr(fstr_t* fstr1, fstr_t* fstr2);
fstr_t* tokenize_fstr(fstr_t* fstr, char token, size_t offset);


int make_fstring_data_unique(fstr_t* fstr);

char* get_fstring_cstring(fstr_t* fstr);
size_t get_fstring_length(fstr_t* fstr);

#endif
