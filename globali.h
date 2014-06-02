#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#define MAX_WORD_LENGTH 100
#define MAX_PRODUCTIONS 100

// Definizione dei tipi ------------------------------------------------------*/

typedef char Symbol;

typedef enum controlli {VERO,FALSO} controlli;

typedef struct
{
        Symbol word [MAX_WORD_LENGTH];
        unsigned length;
} Word;

typedef struct
{
        Word left;
        Word right;
        int error;
        
} Production;

typedef struct
{
        Production productions[MAX_PRODUCTIONS];
        unsigned numprod;
} Grammar;

// Definizione dei prototipi delle funzioni

int is_terminal(Symbol s);
int is_nonterminal(Symbol s);
int is_prodsym(Symbol s);
int is_prodsep(Symbol s);
Symbol read_sym(FILE* file);
Production* add_new_production(Grammar *g);
void add_symbol(Word *w, Symbol s);
Grammar* load_grammar(FILE* file, Grammar* g);
void print_sym (Symbol s);
void print_word (Word* w);
void print_production (Production* p);
void print_grammar(Grammar* g);
void check_s(Grammar* g);
void check_monotona(Grammar* g);
void check_ld(Grammar* g);
int is_single_sym_left(Production* p);
void check_cf(Grammar* g);
int left_length(Production* p);
int right_length(Production* p);
controlli check_nt_left(Production* p);
void acapo(FILE* file);
controlli ask_new();
Symbol read_sym_input();
Grammar* inserimento_prod(Grammar* g);







