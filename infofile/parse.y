/* Configure file grammar
    cd ..; scl enable devtoolset-4 "make -j 34"; cd -; gcc -g -O0 -o infotest infotest.c ../build/lex.yy.o ../build/parse.tab.o 
   */
%{
#include <stdlib.h>
#include <stdio.h>
#include "../infofile/infofile.h"
extern FILE *yyin;
static struct records_infofile *content;
static struct records_infofile *current;

int yyerror(char *s);
int yylex();

%}
/* BISON Declarations */

%union{
  struct data_infofile data;
  char *str;
}

%token NEWLINE
%token EQUAL
%token COLON
%token LEFTBRACE
%token RIGHTBRACE
%token COMMA
%token WHITESP
%token SEMICOLON

%token INTKW
%token DBLKW
%token STRKW
%token INUKW
%token LLUJW
%token <data> INTEGER
%token <data> REAL
%token <data> EXP
%token <data> STRING   
%token <str> WORD

%type <data> object
%type <data> list
%type <data> listobject
%type <data> intobject
%type <data> uintobject
%type <data> lluintobject
%type <data> doubleobject
%type <data> strobject
%type <str> word

/* Grammar follows */
%%
start: input
    | WHITESP input
    ;
input:
    | input statement
    ;
statement: expr newline
    | expr semicolon
    | newline
    ;
semicolon: SEMICOLON
    | semicolon WHITESP
    ;
newline: NEWLINE
    | newline WHITESP
    ;
expr: word equal object {
         struct records_infofile *new =
            malloc(sizeof(struct records_infofile));
         new -> what = $1;
         new -> data = $3;
	 new -> next = NULL;
         if (current) {
	    current -> next = new;
            current = current -> next;
         } else {
            content = new;
	    current = new;
         }
      }
    ;
word: WORD  { $$ = $1; }
    | word WHITESP
    ;
equal: EQUAL
    | equal WHITESP
    ;
object: listobject   { $$ = $1; }
    | intobject      { $$ = $1; }
    | uintobject     { $$ = $1; }
    | lluintobject   { $$ = $1; }
    | doubleobject   { $$ = $1; }
    | strobject      { $$ = $1; }
    | object WHITESP
    ;
listobject: leftbrace list RIGHTBRACE { $$ = $2; }
    ;
leftbrace: LEFTBRACE
    | leftbrace WHITESP
    ;
list: {                           /* void list */
         $$.type = is_list_infofile;
	 $$.list = NULL;
      }
    | object {                    /* one element */
         $$.type = is_list_infofile;
	 $$.list = malloc(sizeof(struct list_infofile));
         $$.list -> data = $1;
      }
    | list comma object {
         $$ = $1;
	 struct list_infofile *curr = $1.list;
         while (curr -> next) curr = curr -> next;
	 curr -> next = malloc(sizeof(struct list_infofile));
         curr -> next -> data = $3;
	 curr -> next -> next = NULL;
      }
    ;
comma: COMMA
    | comma WHITESP
    ;
intobject: INTKW COLON INTEGER { $$.d=strtol($3.s, NULL, 10); free($3.s); $$.type = is_d_infofile; }
    ;
uintobject: INUKW COLON INTEGER { $$.u=strtoul($3.s, NULL, 10); free($3.s); $$.type = is_u_infofile; }
    ;
lluintobject: LLUJW COLON INTEGER { $$.llu=strtoull($3.s, NULL, 10); free($3.s); $$.type = is_llu_infofile; }
    ;
doubleobject: DBLKW COLON EXP  { $$.g=strtod($3.s, NULL); free($3.s); $$.type = is_g_infofile; }
    | DBLKW COLON REAL         { $$.g=strtod($3.s, NULL); free($3.s); $$.type = is_g_infofile; }
    | DBLKW COLON INTEGER      { $$.g=strtod($3.s, NULL); free($3.s); $$.type = is_g_infofile; }
    ;
strobject: STRKW COLON STRING  { $$.s = $3.s; $$.type = is_s_infofile; }
    ;

%%
/*--------------------------------------------------------*/
/* Additional C code */
/* Error processor for yyparse */
int yyerror(char *s)        /* called by yyparse on error */
{
  printf("Error: %s\n",s);
  return(0);
}

/*--------------------------------------------------------*/
/* The controlling function */
struct records_infofile *parse_infofile(FILE *fh, int *error) {
  yyin = fh;
  content = current = NULL;

  *error = yyparse();

  return content;
}
