%{
// yacc -d tema.y && lex tema.l && gcc lex.yy.c y.tab.c -o tema && ./tema test.txt
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "tema.h"

extern FILE* yyin;
extern char* yytext;
extern int yylineno;
%}

%union {
     int intVal;
     float floatVal;
     char charVal;
     char* strVal;
     bool boolVal;
     struct {
          char type[25];
          char name[101];
     } varDecl;
          struct {
          int type;
          char name[101];
     } var;
     struct {  
          struct {
               char type[25];
               char name[101];
          } params[15];
          int num;
     } params;
     struct {
          int type;
          char varName[101];
          int intVal;
          float floatVal;
          char charVal;
          char stringVal[101];
     } val;
}

%token BEGIN_PROGRAM END_PROGRAM 
%token <intVal>INT
%token <floatVal>FLOAT
%token <charVal>CHAR
%token <strVal>TIP <strVal>ID <strVal>STRING
%token <boolVal>BOOL
%token ASSIGN CLASS PRIVAT PUBLIC
%token EQ LT GT LTEQ GTEQ EQN
%token IF FOR THEN ELSE WHILE DO
%token AND OR
%token PLUS MINUS DIV MULT
%token RETURN
%token STRUCT MEMBER
%token TYPEOF <val>EVAL 

%type <varDecl>variabila <varDecl>param <varDecl>array
%type <params>lista_param
%type <val>expresie
%type <var>alg_exp

%left PLUS MINUS DIV MULT MOD

%right THEN ELSE

%nonassoc INT ID

%start progr
%%

progr: declaratii bloc {printVars(); printFuncs(); writeVarsToFile(); writeFuncsToFile(); printf("Program compiled successfully!\n");}
     ;

declaratii : declaratie 
	       | declaratie declaratii
	       ;

declaratie : variabila ';'
           | functie_def
           | STRUCT ID '{' variabile '}' ';'//definire
           | STRUCT ID '{' variabile '}' decl_struct ';'//definire+declarare
           | STRUCT ID ID //declarare daca a fost definit
           | CLASS ID '{' corp_clasa '}' ';'
           | variabila_cu_asignare
           | array ';'
           ;

array : TIP ID '[' INT ']' {addArray($1, $2, $4); strcpy($$.type, $1); strcpy($$.name, $2);}
      ;

decl_struct : ID
            | ID ',' decl_struct
            ;

variabile : variabila ';'
          | variabile variabila ';'
          ;

variabila : TIP ID {addVar($1, $2); strcpy($$.type, $1); strcpy($$.name, $2);}
          ;  

corp_clasa : PRIVAT ':' variabile
           | PRIVAT ':' variabile PUBLIC ':' public_class
           | PUBLIC ':' variabile
           | PUBLIC ':' public_class
           ;

public_class : variabila
             | variabila public_class
             | functie_def
             | functie_def public_class
             ;

functie_def : TIP ID '(' lista_param ')' ';' {
                    addFunction($1, $2);
                    int varIndex = funcNumber-1;
                    funcs[varIndex].paramsNumber = $4.num;
                    for(int i = 0; i < $4.num; i++) {
                         strcpy(funcs[varIndex].params[i].name, $4.params[i].name);
                         funcs[varIndex].params[i].type = nameTypeToNumber($4.params[i].type);
                    }
                    validateFunctionDefinitionParamsTypes(varIndex);
               }
            | TIP ID '(' lista_param ')' '{' corp_functie RETURN expresie ';' '}' {
                    addFunction($1, $2);
                    int varIndex = funcNumber-1;
                    funcs[varIndex].paramsNumber = $4.num;
                    for(int i = 0; i < $4.num; i++) {
                         strcpy(funcs[varIndex].params[i].name, $4.params[i].name);
                         funcs[varIndex].params[i].type = nameTypeToNumber($4.params[i].type);
                    }
                    validateFunctionDefinitionParamsTypes(varIndex);
               }
            | TIP ID '(' lista_param ')' '{' corp_functie RETURN FLOAT ';' '}' {
                    addFunction($1, $2);
                    int varIndex = funcNumber-1;
                    funcs[varIndex].paramsNumber = $4.num;
                    for(int i = 0; i < $4.num; i++) {
                         strcpy(funcs[varIndex].params[i].name, $4.params[i].name);
                         funcs[varIndex].params[i].type = nameTypeToNumber($4.params[i].type);
                    }
                    validateFunctionDefinitionParamsTypes(varIndex);
               }
            | TIP ID '(' lista_param ')' '{' corp_functie RETURN CHAR ';' '}' {
                    addFunction($1, $2);
                    int varIndex = funcNumber-1;
                    funcs[varIndex].paramsNumber = $4.num;
                    for(int i = 0; i < $4.num; i++) {
                         strcpy(funcs[varIndex].params[i].name, $4.params[i].name);
                         funcs[varIndex].params[i].type = nameTypeToNumber($4.params[i].type);
                    }
                    validateFunctionDefinitionParamsTypes(varIndex);
               }
            | TIP ID '(' lista_param ')' '{' corp_functie RETURN STRING ';' '}' {
                    addFunction($1, $2);
                    int varIndex = funcNumber-1;
                    funcs[varIndex].paramsNumber = $4.num;
                    for(int i = 0; i < $4.num; i++) {
                         strcpy(funcs[varIndex].params[i].name, $4.params[i].name);
                         funcs[varIndex].params[i].type = nameTypeToNumber($4.params[i].type);
                    }
                    validateFunctionDefinitionParamsTypes(varIndex);
               }
            | TIP ID '(' lista_param ')' '{' corp_functie RETURN BOOL ';' '}' {
                    addFunction($1, $2);
                    int varIndex = funcNumber-1;
                    funcs[varIndex].paramsNumber = $4.num;
                    for(int i = 0; i < $4.num; i++) {
                         strcpy(funcs[varIndex].params[i].name, $4.params[i].name);
                         funcs[varIndex].params[i].type = nameTypeToNumber($4.params[i].type);
                    }
                    validateFunctionDefinitionParamsTypes(varIndex);
               }
               
            ;

corp_functie : variabila ';'
             | variabila ';' corp_functie
             | variabila_cu_asignare
             | variabila_cu_asignare corp_functie
             | statement
             | statement corp_functie
             ;

variabila_cu_asignare : variabila ASSIGN ID ';' {initializeVarWithVar($1.name, $3);} 
                      | variabila ASSIGN INT ';' {initializeVarWithInt($1.name, $3);}
                      | variabila ASSIGN FLOAT ';' {initializeVarWithFloat($1.name, $3);}
                      | variabila ASSIGN STRING ';' {initializeVarWithString($1.name, $3);}
                      | variabila ASSIGN CHAR ';' {initializeVarWithChar($1.name, $3);}
                      | variabila ASSIGN BOOL ';' {initializeVarWithBool($1.name, $3);}
                      | variabila ASSIGN alg_exp ';'
                      | variabila ASSIGN bool_exp ';'
                      ;

lista_param : param {strcpy($$.params[0].type, $1.type); strcpy($$.params[0].name, $1.name); $$.num = 1;}
            | lista_param ','  param {strcpy($$.params[$$.num].type, $3.type); strcpy($$.params[$$.num].name, $3.name); $$.num++;}
            ;
            
param : TIP ID {strcpy($$.type, $1); strcpy($$.name, $2);}
      ; 
      
/* bloc */
bloc : BEGIN_PROGRAM list END_PROGRAM  
     ;
     
/* lista instructiuni */
list :  statement 
     |  statement list
     ;

/* instructiune */
statement : ID ASSIGN ID ';' {assignVarToVar($1, $3);}
          | ID ASSIGN INT ';' {assignIntToVar($1, $3);}
          | ID ASSIGN FLOAT ';' {assignFloatToVar($1, $3);}
          | ID ASSIGN CHAR ';' {assignCharToVar($1, $3);}
          | ID ASSIGN STRING ';' {assignStringToVar($1, $3);}
          | ID ASSIGN BOOL ';' {assignBoolToVar($1, $3);}
          | ID ASSIGN alg_exp ';' {validateVarExistance($1); validateExpressionTypes(varOrFuncNameToTypeNumber($1), $3.type);}
          | ID ASSIGN bool_exp ';' {validateVarExistance($1); validateExpressionTypes(varOrFuncNameToTypeNumber($1), BOOL_TYPE);}
          | ID '[' INT ']' ASSIGN INT ';'
          | ID MEMBER ID ASSIGN ID ';'
          | ID MEMBER ID ASSIGN INT ';'
          | ID MEMBER ID ASSIGN STRING ';'
          | ID MEMBER ID ASSIGN BOOL ';'
          | ID MEMBER ID ASSIGN alg_exp ';'
          | ID MEMBER ID ASSIGN bool_exp ';'
          | ID '(' lista_apel ')' ';'
          | ifthen_def
          | ifthenelse_def
          | while_def
          | dowhile_def
          | for_def 
          | TYPEOF '(' expresie ')' ';' {printf("%d:  %s%s%s type: %s.\n", yylineno, $3.varName[0] == '\0' ?  "Expression" : "Variable ", $3.varName[0] == '\0' ?  "" : $3.varName, " has", numberTypeToName($3.type));}
          | TYPEOF '(' FLOAT ')' ';' {printf("%d:  Expression has type: float.\n", yylineno);}
          | TYPEOF '(' CHAR ')' ';' {printf("%d:  Expression has type: char.\n", yylineno);}
          | TYPEOF '(' STRING ')' ';' {printf("%d:  Expression has type: string.\n", yylineno);}
          | TYPEOF '(' BOOL ')' ';' {printf("%d:  Expression has type: bool.\n", yylineno);}
          | EVAL '(' expresie ')' ';' {if($3.type == INT_TYPE) printf("%d:  Var has value of: %d\n", yylineno, $3.intVal);}
          | EVAL '(' FLOAT ')' ';' {printf("%d:  Var has value of: %f\n", yylineno, $3);}
          | EVAL '(' CHAR ')' ';' {printf("%d:  Var has value of: %c\n", yylineno, $3);}
          | EVAL '(' STRING ')' ';' {printf("%d:  Var has value of: %s\n", yylineno, $3);}
          | EVAL '(' BOOL ')' ';' {printf("%d:  Var has value of: %s\n", yylineno, $3 == 1 ? "True" : "False");}
          ;          
        
ifthen_def : IF '(' lista_cond ')' THEN '{' list '}' %prec THEN
           | IF '(' lista_cond ')' THEN statement
           ;

ifthenelse_def : IF '(' lista_cond ')' THEN '{' list '}' ELSE '{' list '}' 
               | IF '(' lista_cond ')' THEN '{' list '}' ELSE statement
               | IF '(' lista_cond ')' THEN statement ELSE '{' list '}'
               | IF '(' lista_cond ')' THEN statement ELSE statement
               ;

lista_cond : bool_exp AND bool_exp
           | bool_exp AND '(' lista_cond ')'
           | '(' lista_cond ')' AND bool_exp
           | '(' lista_cond ')' AND '(' lista_cond ')'
           | bool_exp OR bool_exp
           | bool_exp OR '(' lista_cond ')'
           | '(' lista_cond ')' OR bool_exp
           | '(' lista_cond ')' OR '(' lista_cond ')'
           | bool_exp
           ;

bool_exp : expresie EQ expresie
         | expresie EQN expresie
         | expresie GT expresie
         | expresie LT expresie
         | expresie GTEQ expresie
         | expresie LTEQ expresie
         ;   

while_def : WHILE '(' lista_cond ')' DO '{' list '}'
          | WHILE '(' lista_cond ')' DO statement
          ;

dowhile_def: DO '{' list '}' WHILE '(' lista_cond ')' ';'
           | DO statement WHILE '(' lista_cond ')' ';'
           ;

for_def : FOR '(' init_for ';' bool_exp ';' increment_for ')' '{' list '}'
        | FOR '(' init_for ';' bool_exp ';' increment_for ')' statement
        ;

init_for : ID ASSIGN INT {assignIntToVar($1, $3);}
         | variabila ASSIGN INT {initializeVarWithInt($1.name, $3);}
         ;

increment_for : ID PLUS PLUS
              | PLUS PLUS ID
              | ID MINUS MINUS
              | MINUS MINUS ID
              | ID ASSIGN expresie
              ;

expresie : INT {$$.type = INT_TYPE; $$.intVal = $1; $$.varName[0] = '\0';}
         | ID '[' INT ']' {$$.type = INT_TYPE; $$.intVal = $3; $$.varName[0] = '\0';}
         | ID {$$.type = varOrFuncNameToTypeNumber($1); strcpy($$.varName, $1); if($$.type == INT_TYPE) $$.intVal = vars[varNameToIndex($1)].intVal;}
         | ID '(' lista_apel ')' {$$.type = varOrFuncNameToTypeNumber($1); strcpy($$.varName, $1);}
         | alg_exp {$$.type = $1.type; $$.varName[0] = '\0';}
         ; 

alg_exp : expresie PLUS expresie {validateExpressionTypes($1.type, $3.type); $$.type = $1.type;}
        | expresie MINUS expresie {validateExpressionTypes($1.type, $3.type); $$.type = $1.type;}
        | expresie DIV expresie {validateExpressionTypes($1.type, $3.type); $$.type = $1.type;}
        | expresie MULT expresie {validateExpressionTypes($1.type, $3.type); $$.type = $1.type;}
        | expresie MOD expresie {validateExpressionTypes($1.type, $3.type); $$.type = $1.type;}
        | expresie PLUS FLOAT {validateExpressionTypes($1.type, FLOAT_TYPE); $$.type = FLOAT_TYPE;}
        | expresie MINUS FLOAT {validateExpressionTypes($1.type, FLOAT_TYPE); $$.type = FLOAT_TYPE;}
        | expresie DIV FLOAT {validateExpressionTypes($1.type, FLOAT_TYPE); $$.type = FLOAT_TYPE;}
        | expresie MULT FLOAT {validateExpressionTypes($1.type, FLOAT_TYPE); $$.type = FLOAT_TYPE;}
        | expresie MOD FLOAT {validateExpressionTypes($1.type, FLOAT_TYPE); $$.type = FLOAT_TYPE;}
        | '(' expresie ')' {$$.type = $2.type;}
        ;

lista_apel : ID 
           | INT
           | FLOAT
           | BOOL
           | CHAR
           | STRING
           | lista_apel ',' ID
           | lista_apel ',' INT
           | lista_apel ',' FLOAT
           | lista_apel ',' BOOL
           | lista_apel ',' STRING
           ;
%%
int yyerror(char * s){
    printf("Error: %s at line %d\n", s, yylineno);
}

int main(int argc, char** argv){
    yyin=fopen(argv[1],"r");
    yyparse();
} 
