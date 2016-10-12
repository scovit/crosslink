
%{

// echo bison; bison infotest.y; echo gcc; gcc -c infotest.tab.c; echo bison; bison -d infotest.y; echo flex; flex lexer.l; echo gcc; gcc -c lex.yy.c; echo link ; gcc -o calc infotest.tab.o lex.yy.o
// this is a lexer of a keyword rich simple custom configuration file

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../infofile/infofile.h"
#include "../build/parse.tab.h"
#define MAX_STR_CONST 4096

int yyerror(char *s);

char string_buf[MAX_STR_CONST];
char *string_buf_ptr;

%}

%option noyywrap

delim         [ \t]
whitesp       {delim}+

digit         [0-9]
integer       {digit}+
real          ({digit}+[.]{digit}*)|({digit}*[.]{digit}+)
exp           ({integer}|{real})[eE]-?{integer}

letter        [a-zA-Z_]
word          {letter}+

%x strlit

%%

{integer}                { yylval.data.s = strdup(yytext); return INTEGER; }
{real}                   { yylval.data.s = strdup(yytext); return REAL; }
{exp}                    { yylval.data.s = strdup(yytext); return EXP; }
"nan"                    { yylval.data.s = strdup(yytext); return REAL; }

"d"                      return INTKW;
"g"                      return DBLKW;
"s"                      return STRKW;
"u"                      return INUKW;
"llu"                    return LLUJW;

{word}                   { yylval.data.s = strdup(yytext);
                           return WORD; }

{whitesp}                return WHITESP;

"\n"                     return NEWLINE;
"="                      return EQUAL;
":"                      return COLON;
"["                      return LEFTBRACE;
"]"                      return RIGHTBRACE;
","                      return COMMA;
";"                      return SEMICOLON;

"\""     { string_buf_ptr = string_buf; BEGIN(strlit); }

<strlit>\"        { /* saw closing quote - all done */
  BEGIN(INITIAL);
  *string_buf_ptr = '\0';

  yylval.data.s = strdup(string_buf);
  return STRING;
}
     
<strlit>\n        {
   /* error - unterminated string constant */
   /* generate error message */
  BEGIN(INITIAL);

  *string_buf_ptr = '\0';
  yyerror(string_buf);
}
     
<strlit>\\[0-7]{1,3} {
/* octal escape sequence */
   int result;
     
   (void) sscanf( yytext + 1, "%o", &result );

   if ( result > 0xff ) 
      /* error, constant is out-of-bounds */
   {
      BEGIN(INITIAL);
      *string_buf_ptr = '\0';
      yyerror(string_buf);
   }

   *string_buf_ptr++ = result;
}

<strlit>\\[0-9]+ {
    /* generate error - bad escape sequence; something
    * like '\48' or '\0777777'
    */
  BEGIN(INITIAL);
  *string_buf_ptr = '\0';
  yyerror(string_buf);
}
     
<strlit>\\n  *string_buf_ptr++ = '\n';
<strlit>\\t  *string_buf_ptr++ = '\t';
<strlit>\\r  *string_buf_ptr++ = '\r';
<strlit>\\b  *string_buf_ptr++ = '\b';
<strlit>\\f  *string_buf_ptr++ = '\f';

<strlit>\\(.|\n)  *string_buf_ptr++ = yytext[1];

<strlit>[^\\\n\"]+        {
  char *yptr = yytext;

  while ( *yptr )
    *string_buf_ptr++ = *yptr++;
}


<strlit><<EOF>>   {
  yyerror( "unterminated quote" );
  yyterminate();
}

<<EOF>>  {
  yyterminate();
}

%%
