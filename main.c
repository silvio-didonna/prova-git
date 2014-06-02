#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "globali.h"


int main(int argc, char *argv[])
{
  char* filename = argv[1];
  FILE* gram_file;
  Grammar grammar;
  
  // controlla se è stato inserito il nome del file
  
  if (filename == 0)
  {
     printf("nome file non specificato \n");
     return -1;
  }
  
  // apertura del file contenente la grammatica
  
  gram_file = fopen(filename,"r");
  if (gram_file == NULL)
  {
     printf("nome di file errato\n");
     return -1;
  }
  
  print_grammar(load_grammar(gram_file,&grammar));
  
  check_s(&grammar);
  
  check_cf(&grammar);
  
  check_monotona(&grammar);
  
  check_ld(&grammar);
  
  fclose(gram_file);
  
  inserimento_prod(&grammar);
  
  print_grammar(&grammar);
  
  
  system("PAUSE");	
  return 0;
}


