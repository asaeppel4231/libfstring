#include <stdio.h>
#include "fstring.h"

int main(){
  fstr_t* fstr = create_fstring();

  printf("Set cstring: %d\n", set_fstring_cstring(fstr, "Hello World\n", FSTRING_CSTRING_FLAG_DEEP_COPY));
  printf("Content: %s\n", get_fstring_cstring(fstr));

  printf("10 self-concat operations...\n");
  
  for(int i=0; i < 10; i++){
//    printf("i = %d\n", i);
    int result = concat_fstr(fstr, fstr, 0);
//    printf("Concat result: %d\n", result);
  }
//  printf("Result: %s\n", get_fstring_cstring(fstr));
  printf("Creating view...\n");
  fstr_t* view;

  size_t pos = 0;
//  printf("Fstr length: %zu", get_fstring_length(fstr));
  while(pos < get_fstring_length(fstr)){
    view = tokenize_fstr(fstr, '\n', pos);
    make_fstring_data_unique(view);
    printf("Token: %s\n", get_fstring_cstring(view));
    pos += get_fstring_length(view);
//    printf("View length: %zu\n", get_fstring_length(view));
    pos += 1;
    destroy_fstring(view, 0);
  }
  
  destroy_fstring(fstr, 0);
}
