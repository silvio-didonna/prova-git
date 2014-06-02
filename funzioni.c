#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include "globali.h"


// Procedure di riconoscimento dei simboli -----------------------------------*/

int is_terminal(Symbol s)
{
    return (islower(s));
    //return (s >= 'a') && (s <= 'z');
}

int is_nonterminal(Symbol s)
{
    return (isupper(s));
    //return (s >= 'A') && (s <= 'Z');
}

int is_prodsym(Symbol s)
{
    return (s == '>');
}

int is_prodsep(Symbol s)
{
    return (s == '\n');
}


/* Lettura di simboli da file ------------------------------------------------*/

Symbol read_sym(FILE* file)
{
       Symbol s;
       
       // fscanf(file,"%c",&s); 
       
       //questo ciclo permette di saltare la lettura di spazi tra i simboli
       do 
          s = getc(file);
       while (s==' ');
       
       return s;
}

Symbol read_sym_input(){
	Symbol s;
	
	do
		s=getc(stdin);
	while (s==' ');
	
	return s;
}

Production* add_new_production(Grammar *g)
{
  Production* p;
  p = &(g->productions[g->numprod++]);
  p->left.length = 0;
  p->right.length = 0;
  p->error=0;
  
  return p;
}
                       

void add_symbol(Word *w, Symbol s)
{
  w->word[w->length++] = s;  
}


// Procedura di acquisizione di una grammatica da un file --------------------*/
                       
Grammar* load_grammar(FILE* file, Grammar* g)
{
			 
         enum States {START,LEFT,RIGHT,ERROR};
         /*   START  = Scansione di una nuova produzione [F]
              LEFT   = Scansione della parte sinistra
              RIGHT  = Scansione della parte destra [F]
              ERROR  = Errore di scansione
         */
         enum States current_state = START;  // Stato iniziale
         Symbol s;
         Production* p;
         
         g->numprod = 0; // Inizializza la grammatica
                  
         while (!feof(file)) 
         {
               s = read_sym(file);
               if (feof(file)) break;
               switch(current_state)
               {
               case START:
                    if (is_terminal(s) || is_nonterminal(s))
                    {
                       current_state = LEFT;
                       
                       //p = &(g->productions[g->numprod++]);
                       //p->left.length = 0;
                       p = add_new_production(g);
                       add_symbol(&p->left,s);
                       //L'istruzione precedente corrisponde a p->left.word[p->left.length++] = s;
                    }
                    else if (is_prodsep(s))
                    {
                       current_state = START;
                    }
                    else{ 
                         //current_state = ERROR;
                         current_state = ERROR;
                       	 p = add_new_production(g);
                       	 if(is_prodsym(s))
                       	 	p->error=4;						//TESTA DELLA PRODUZIONE VUOTA
                       	 	else p->error=1;				//SIMBOLO NON RICONOSCIUTO
                         //acapo(file);
                     }
                    break;
               case LEFT:
                    if (is_terminal(s) || is_nonterminal(s))
                    {
                       current_state = LEFT;
                       add_symbol(&p->left,s);
                    }
                    else if (is_prodsym(s))
                    {
                       current_state = RIGHT;
                    }
                    else{
                        //current_state = ERROR;
                        current_state = ERROR;
                        p->error=1;					//SIMBOLO NON RICONOSCIUTO
                        //acapo(file);
                    }
                    break;
               case RIGHT:
                    if (is_terminal(s) || is_nonterminal(s))
                    {
                       current_state = RIGHT;
                       add_symbol(&p->right,s);
                    }
                    else if (is_prodsep(s))
                    {
                       current_state = START;
                    }
                    else{
                         //current_state = ERROR;
                         current_state = ERROR;
                         if(is_prodsym(s))
                         	p->error=2;				//TROPPI SEPARATORI >
                         	else p->error=1;		//SIMBOLO NON RICONOSCIUTO
                         //acapo(file);
                     }
                    break;
                case ERROR:
                	if(is_prodsep(s))
                		current_state = START;
                	break;
               }
         }
         
         
            return g;

}       

// Procedure di stampa

void print_sym (Symbol s)
{
     printf("%c ",s);
}


void print_word (Word* w)
{
     int i;
     
     for (i=0; i<w->length; i++){
         print_sym(w->word[i]);
     }
}

void print_production (Production* p) 
{
	 int i;
	 
	 controlli controllo=FALSO;
	 
     print_word(&p->left);
     printf (" --> ");
     print_word(&p->right);
     
    if(check_nt_left(p)==VERO)
    printf("\t\tE' PRESENTE ALMENO UN NONTERMINALE");
     
     switch(p->error){
     	case 1: printf("\tATTENZIONE: SIMBOLO NON RICONOSCIUTO!");
     			break;
     	case 2: printf("\tATTENZIONE: TROPPI SEPARATORI --> !");
     			break;
     	case 3: printf("\tATTENZIONE: PRODUZIONE NON VALIDA!");
     			break;
     	case 4: printf("\tATTENZIONE: TESTA DELLA PRODUZIONE VUOTA!");
		 		break;		
     			
     }
     

}

void print_grammar(Grammar* g)
{
     int i;
     
     if (g == NULL)
        printf ("Errore! Grammatica non valida! \n");
     else
     {
        printf ("Numero di produzioni: %d\n", g->numprod);
        for (i=0; i<g->numprod; i++)
        {
            print_production(&g->productions[i]);
            printf ("\n");
        }
     }
}

//MIE FUNZIONI------------------------------------------------------------------

int is_single_sym_left(Production* p){
	
	return (p->left.length==1);
}

int left_length(Production* p){
	
	return (p->left.length);
}


int right_length(Production* p){
	
	return (p->right.length);
}


controlli check_nt_left(Production* p){
	int i;
	controlli controllo=FALSO;
	
	for(i=0; i<left_length(p); i++){
     	if(is_nonterminal(p->left.word[i])){
        	controllo=VERO;
        	break;
        	}
        }
        if((controllo==FALSO) && (p->error==0))
        	p->error=3;								//PRODUZIONE NON VALIDA
        return controllo;
}


void check_s(Grammar* g){
	int i=0;
	
	controlli esito=FALSO;
	
	for (i=0; i<g->numprod; i++)
	{
		if(is_single_sym_left(&(g->productions[i])))
			if(strcmp(g->productions[i].left.word,"S")==0){
				esito=VERO;
				break;
				}
	}
	if(esito==VERO)
		printf("\nLA GRAMMATICA CARICATA PRESENTA IL SIMBOLO INIZIALE\n\n");
		else 
		printf("\nATTENZIONE: NON E' PRESENTE IL SIMBOLO INIZIALE!\n\n");
	return;
}


void check_cf(Grammar* g){
	
	int i=0;
	
	controlli esito=FALSO;
	
	for (i=0; i<g->numprod; i++)
	{
		if(is_single_sym_left(&(g->productions[i]))){
			if(is_nonterminal(g->productions[i].left.word[0]))
				esito=VERO;
				
			}
			else break;
		}
		
		if(esito==VERO)
			printf("\nLA GRAMMATICA E' C.F.\n\n");
			else printf("\nLA GRAMMATICA NON E' C.F.\n\n");
			
	return;
}



void check_monotona(Grammar* g){
	int i=0;
	
	controlli esito=FALSO;
	
	for (i=0; i<g->numprod; i++)
	{
		if((is_single_sym_left) && (check_nt_left(&(g->productions[i]))==VERO))
			if(left_length(&(g->productions[i]))<=right_length(&(g->productions[i])))
				esito=VERO;
				else esito=FALSO;
			
		if (esito==FALSO)
			break;
	}
	
	if(esito==VERO)
		printf("\nLA GRAMMATICA E' MONOTONA\n\n");
		else printf("\nLA GRAMMATICA NON E' MONOTONA\n\n");
	
	return;
}


void check_ld(Grammar* g){
	int i=0;
	
	controlli esito=FALSO;
	
	if((is_single_sym_left) && (check_nt_left(&(g->productions[i]))==VERO))
	for (i=0; i<g->numprod; i++)
	{
		switch(right_length(&(g->productions[i]))){
			case 0: esito=VERO; break;
			case 1: if(is_terminal(g->productions->right.word[0]))
						esito=VERO;
						else esito=FALSO;
					break;
			case 2: if((is_terminal(g->productions->right.word[0])) && (is_nonterminal(g->productions->right.word[1])))
						esito=VERO;
						else esito=FALSO;
					break;
			default: 	esito=FALSO;
			}
			if(esito==FALSO)
				break;
	
	}
	
	if(esito==VERO)
		printf("\nLA GRAMMATICA E' LINEARE DESTRA\n\n");
		else printf("\nLA GRAMMATICA NON E' LINEARE DESTRA\n\n");
	
	return;
}


void acapo(FILE* file){
	
	Symbol k;
	
	do
		k=fgetc(file);
	while((k != '\n') || (!feof(file)));
	
	return;
}


controlli ask_new(){
	controlli esito;
	
	Symbol s;
	
	esito=FALSO;
	
	printf("\nVUOI INSERIRE UNA NUOVA PRODUZIONE DA TASTIERA? (Y/N)    ");
	scanf("%c",&s);
	
	
	if(s == 'Y'){
		esito=VERO;
		
		
	}
		
	return esito;
}


Grammar* inserimento_prod(Grammar* g){
	
	enum States {START,LEFT,RIGHT,ERROR};
         /*   START  = Scansione di una nuova produzione [F]
              LEFT   = Scansione della parte sinistra
              RIGHT  = Scansione della parte destra [F]
              ERROR  = Errore di scansione
         */
         enum States current_state = START;  // Stato iniziale
         
    Symbol s;
    Production* p;
         
	
	while(ask_new()==VERO){
		if(g->numprod<(MAX_PRODUCTIONS-1)){
			printf("\n");
			do{
				
				s = read_sym_input();
				
				
				switch(current_state)
               {
               case START:
                    if (is_terminal(s) || is_nonterminal(s))
                    {
                       current_state = LEFT;
                       
                       //p = &(g->productions[g->numprod++]);
                       //p->left.length = 0;
                       p = add_new_production(g);
                       add_symbol(&p->left,s);
                       //L'istruzione precedente corrisponde a p->left.word[p->left.length++] = s;
                    }
                    else if (is_prodsep(s))
                    {
                       current_state = START;
                    }
                    else{ 
                         //current_state = ERROR;
                         current_state = ERROR;
                       	 p = add_new_production(g);
                       	 if(is_prodsym(s))
                       	 	p->error=4;						//TESTA DELLA PRODUZIONE VUOTA
                       	 	else p->error=1;				//SIMBOLO NON RICONOSCIUTO
                         //acapo(file);
                    }
                    break;
               case LEFT:
                    if (is_terminal(s) || is_nonterminal(s))
                    {
                       current_state = LEFT;
                       add_symbol(&p->left,s);
                    }
                    else if (is_prodsym(s))
                    {
                       current_state = RIGHT;
                    }
                    else{
                        //current_state = ERROR;
                        current_state = ERROR;
                        p->error=1;					//SIMBOLO NON RICONOSCIUTO
                        //acapo(file);
                    }
                    break;
               case RIGHT:
                    if (is_terminal(s) || is_nonterminal(s))
                    {
                       current_state = RIGHT;
                       add_symbol(&p->right,s);
                    }
                    else if (is_prodsep(s))
                    {
                       current_state = START;
                       //
                    }
                    else{
                         //current_state = ERROR;
                         current_state = ERROR;
                         if(is_prodsym(s))
                         	p->error=2;				//TROPPI SEPARATORI >
                         	else p->error=1;		//SIMBOLO NON RICONOSCIUTO
                         //acapo(file);
                    }
                    break;
                case ERROR:
                	if(is_prodsep(s))
                		current_state = START;
                	break;
               }
				//
			} while(current_state != START);
			
		}
		else printf("\nSPIACENTE, NUMERO MASSIMO DI PRODUZIONI RAGGIUNTO.\n\n");
	}
			
	
	return g;
}











