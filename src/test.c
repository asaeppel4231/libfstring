#include <stdio.h>
#include "fstring.h"

int main(){
  fstr_t* fstr = create_fstring();

  printf("Set cstring: %d\n", set_fstring_cstring(fstr, "Hello World !!!", FSTRING_CSTRING_FLAG_DEEP_COPY));
  printf("Content: %s\n", get_fstring_cstring(fstr));  
 
  printf("This time again, but with readonly literal data\n");
  fstr_t* fstr2 = create_fstring();
  printf("Set cstring: %d\n", set_fstring_cstring(fstr2, "Again!!!", FSTRING_CSTRING_FLAG_FLAT_COPY));

  printf("Content: %s\n", get_fstring_cstring(fstr2));

  printf("Comparision of both strings: %d\n", compare_fstr(fstr, fstr2));
  printf("Concat of strings: %d\n", concat_fstr(fstr2, fstr, 0));
  printf("Concat result: %s\n", get_fstring_cstring(fstr));

  fstr_t* view;
  printf("Tokenizing: %p\n", view = tokenize_fstr(fstr, ' ', 0));
  printf("Make string unique: %d\n", make_fstring_data_unique(view));
  printf("First token: %s\n", get_fstring_cstring(view));

  destroy_fstring(view, 0);    
  destroy_fstring(fstr2, FSTRING_FREE_FLAG_SKIP_DATA);
  destroy_fstring(fstr, 0);
 
}
