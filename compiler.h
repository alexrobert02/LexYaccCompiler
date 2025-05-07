#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
extern int yylineno;

#define INT_TYPE 0
#define FLOAT_TYPE 1
#define CHAR_TYPE 2
#define STRING_TYPE 3
#define BOOL_TYPE 4

struct vars{
    int type;
    bool isConst;
    char name[101];
    int intVal;
    float floatVal;
    char charVal;
    char stringVal[101];
    bool boolVal;
} vars[501];
int varNumber = 0;

struct funcs{
    int type;
    char name[101];
    int paramsNumber;
    struct {
        int type;
        char name[101];
    } params[15];
} funcs[101];
int funcNumber = 0;

//------- HELPERS -------

int varOrFuncNameToTypeNumber(char *name) {
    for(int i = 0; i < varNumber; i++) {
        if(strcmp(name, vars[i].name) == 0) {
            return vars[i].type;
        }
    }
    for(int i = 0; i < funcNumber; i++) {
        if(strcmp(name, funcs[i].name) == 0) {
            return funcs[i].type;
        }
    }
}

bool isConstVar(char *varName) {
    char constType[] = "const ";
    if(strlen(varName) < strlen(constType)) {
        return false;
    }
    for(int i = 0; i < strlen(constType); i++) {
        if(constType[i] != varName[i])
            return false;
    }
    return true;
}

bool doesVarExist(char *varName) {
    for(int i = 0; i < varNumber; i++) {
        if(strcmp(varName, vars[i].name) == 0) {
            return true;
        }
    }
    return false;
}

bool doesFuncExist(char *funcName) {
    for(int i = 0; i < funcNumber; i++) {
        if(strcmp(funcName, funcs[i].name) == 0) {
            return true;
        }
    }
    return false;
}

int varNameToIndex(char *name) {
    for(int i = 0; i < varNumber; i++) {
        if(strcmp(vars[i].name, name) == 0){
            return i;
        }
    }
    printf("%d:  Error: Variable `%s` was not declared \n", yylineno, name);
    return -1;
}

int funcNameToIndex(char *name) {
    for(int i = 0; i < funcNumber; i++) {
        if(strcmp(funcs[i].name, name) == 0){
            return i;
        }
    }
    printf("%d:  Error: Function `%s` was not declared \n", yylineno, name);
    return -1;
}

int nameTypeToNumber(char *name) {
    char nameCpy[strlen(name)];
    strcpy(nameCpy, name);
    if(isConstVar(name)) {
        strcpy(nameCpy, nameCpy + 6);
    }

    if(strcmp(nameCpy, "int") == 0) {
        return INT_TYPE;
    } else if(strcmp(nameCpy, "float") == 0) {
        return FLOAT_TYPE;
    } else if(strcmp(nameCpy, "char") == 0) {
        return CHAR_TYPE;
    } else if(strcmp(nameCpy, "string") == 0) {
        return STRING_TYPE;
    } else if(strcmp(nameCpy, "bool") == 0) {
        return BOOL_TYPE;
    } else {
        printf("%d:  Error: cannot find a valid type name\n", yylineno);
    }
}

char* numberTypeToName(int type) {
    if(type == INT_TYPE) {
        return "int";
    } else if(type == FLOAT_TYPE) {
        return "float";
    } else if(type == CHAR_TYPE) {
        return "char";
    } else if(type == STRING_TYPE) {
        return "string";
    } else if(type == BOOL_TYPE) {
        return "bool";
    } else {
        printf("Error: cannot find a valid type number\n");
        return NULL;
    }
}

void printVars() {
    printf("------------------------------------VARIABLES------------------------------------\n");
    for(int i = 0; i < varNumber; i++) {
        if(vars[i].isConst) {
            printf("name: %s | type: const %s | value: ", vars[i].name, numberTypeToName(vars[i].type));
        } else {
            printf("name: %s | type: %s | value: ", vars[i].name, numberTypeToName(vars[i].type));
        }
        switch (vars[i].type) {
        case INT_TYPE:
            printf("%d\n", vars[i].intVal);
            break;
        case FLOAT_TYPE:
            printf("%.3f\n", vars[i].floatVal);
            break;
        case CHAR_TYPE:
            printf("%c\n", vars[i].charVal);
            break;
        case STRING_TYPE:
            printf("%s\n", vars[i].stringVal);
            break;
        case BOOL_TYPE:
            if(vars[i].boolVal) {
                printf("True\n");
            } else {
                printf("False\n");
            }
            break;
        }
    }
    printf("---------------------------------------------------------------------------------\n");
}

void printFuncs() {
    printf("------------------------------------FUNCTIONS------------------------------------\n");
    for(int i = 0; i < funcNumber; i++) {
        printf("name: %s | type: %s | parameters: (", funcs[i].name, numberTypeToName(funcs[i].type));
        for(int j = 0; j < funcs[i].paramsNumber; j++) {
            printf("name: %s | type: %s", funcs[i].params[j].name, numberTypeToName(funcs[i].params[j].type));
            if(j != funcs[i].paramsNumber-1) {
                printf(", ");
            }
        }
        printf(")\n");
    }
    printf("---------------------------------------------------------------------------------\n");
}

void writeVarsToFile() {
    FILE *file;

    file = fopen("./symbol_table.txt", "w+");
    fprintf(file, "------------------------------------VARIABLES------------------------------------\n");
    for(int i = 0; i < varNumber; i++) {
        if(vars[i].isConst) {
            fprintf(file, "name: %s | type: const %s | value: ", vars[i].name, numberTypeToName(vars[i].type));
        } else {
            fprintf(file, "name: %s | type: %s | value: ", vars[i].name, numberTypeToName(vars[i].type));
        }
        switch (vars[i].type) {
        case INT_TYPE:
            fprintf(file, "%d\n", vars[i].intVal);
            break;
        case FLOAT_TYPE:
            fprintf(file, "%.3f\n", vars[i].floatVal);
            break;
        case CHAR_TYPE:
            fprintf(file, "%c\n", vars[i].charVal);
            break;
        case STRING_TYPE:
            fprintf(file, "%s\n", vars[i].stringVal);
            break;
        case BOOL_TYPE:
            if(vars[i].boolVal) {
                fprintf(file, "True\n");
            } else {
                fprintf(file, "False\n");
            }
            break;
        }
    }
    fprintf(file, "---------------------------------------------------------------------------------\n");
    fclose(file);
}

void writeFuncsToFile() {
    FILE *file;

    file = fopen("./symbol_table_functions.txt", "w+");
    fprintf(file, "------------------------------------FUNCTIONS------------------------------------\n");
    for(int i = 0; i < funcNumber; i++) {
        fprintf(file, "name: %s | type: %s | parameters: (", funcs[i].name, numberTypeToName(funcs[i].type));
        for(int j = 0; j < funcs[i].paramsNumber; j++) {
            fprintf(file, "name: %s | type: %s", funcs[i].params[j].name, numberTypeToName(funcs[i].params[j].type));
            if(j != funcs[i].paramsNumber-1) {
                fprintf(file, ", ");
            }
        }
        fprintf(file, ")\n");
    }
    fprintf(file, "---------------------------------------------------------------------------------\n");
    fclose(file);
}

//------- LOGIC -------

void addArray(char *typeName, char *name, int index) {
    vars[varNumber].type = nameTypeToNumber(typeName);
    strcpy(vars[varNumber].name, name);
    strcat(vars[varNumber].name, "[" );
    char buffer[50];
    buffer[0] = '\0';
    int factor = 1;
    int cifra;
    int result = 0;
    char character;
    while(index > 0){
        cifra = index % 10;
        index = index / 10;
        result = result * 10 + cifra;
    }
    while(result > 0){
        cifra = result % 10;
        character = cifra + '0';
        strncat(buffer, &character, 1);
        result = result / 10;
    }
    strcat(vars[varNumber].name, buffer);
    strcat(vars[varNumber].name, "]");
    varNumber++;
}

void addVar(char *typeName, char *name) {
    if(doesVarExist(name)){
        printf("%d:  Error: Variable `%s` already exists \n", yylineno, name);
        exit(1);
    }
    vars[varNumber].type = nameTypeToNumber(typeName);
    vars[varNumber].isConst = isConstVar(typeName);
    strcpy(vars[varNumber].name, name);
    varNumber++;
}

void addFunction(char *typeName, char *name) {
    funcs[funcNumber].type = nameTypeToNumber(typeName);
    strcpy(funcs[funcNumber].name, name);
    funcNumber++;
}

void validateExpressionTypes(int type1, int type2) {
    if(type1 != type2) {
        printf("%d:  Error: Cannot cast type %s to %s \n", yylineno, numberTypeToName(type2), numberTypeToName(type1));
        exit(1);
    }
}

void validateFunctionDefinitionParamsTypes(int index) {
    for(int i = 0; i < funcNumber; i++) {
        if(i != index) {
            if(funcs[index].type == funcs[i].type && funcs[index].paramsNumber == funcs[i].paramsNumber && strcmp(funcs[index].name, funcs[i].name) == 0) {
                bool differs = false;
                for(int j = 0; j < funcs[index].paramsNumber; j++) {
                    if(funcs[index].params[j].type != funcs[i].params[j].type) {
                        differs = true;
                        break;
                    }
                }
                if(!differs) {
                    printf("%d:  Error: Cannot define 2 functions with the same name and parameter types \n", yylineno);
                    exit(1);
                }
            }
        }
    }
} 

void validateVarExistance(char *name) {
    if(!doesVarExist(name)){
        printf("%d:  Error: Variable `%s` was not declared \n", yylineno, name);
        exit(1);
    }
}

//------- INITIALIZERS -------

void initializeVarWithVar(char *name1, char* name2) {
    if(!doesVarExist(name1)){
        printf("%d:  Error: Variable `%s` was not declared \n", yylineno, name1);
        exit(1);
    }
    if(!doesVarExist(name2)){
        printf("%d:  Error: Variable `%s` was not declared \n", yylineno, name2);
        exit(1);
    }
    int i1 = varNameToIndex(name1);
    int i2 = varNameToIndex(name2);
    if(vars[i1].type != vars[i2].type) {
        printf("%d:  Error: Cannot assign type %s to %s \n", yylineno, numberTypeToName(vars[i2].type), numberTypeToName(vars[i1].type));
        exit(1);
    } else {
        if(vars[i1].type == INT_TYPE) {
            vars[i1].intVal = vars[i2].intVal;
        } else if(vars[i1].type == FLOAT_TYPE) {
            vars[i1].floatVal = vars[i2].floatVal;
        } else if(vars[i1].type == CHAR_TYPE) {
            vars[i1].charVal = vars[i2].charVal;
        } else if(vars[i1].type == STRING_TYPE) {
            strcpy(vars[i1].stringVal, vars[i2].stringVal);
        } else if(vars[i1].type == BOOL_TYPE) {
            vars[i1].boolVal = vars[i2].boolVal;
        }
    }
}

void initializeVarWithInt(char *name, int value) {
    if(!doesVarExist(name)){
        printf("%d:  Error: Variable `%s` was not declared \n", yylineno, name);
        exit(1);
    }
    int i = varNameToIndex(name);
    if(vars[i].type == INT_TYPE) {
        vars[i].intVal = value;
    } else if(vars[i].type == FLOAT_TYPE) {
        vars[i].floatVal = value;
    } else {
        printf("%d:  Error: Cannot assign INT_TYPE to %s \n", yylineno, numberTypeToName(vars[i].type));
        exit(1);
    }
}

void initializeVarWithFloat(char *name, float value) {
    if(!doesVarExist(name)){
        printf("%d:  Error: Variable `%s` was not declared \n", yylineno, name);
        exit(1);
    }
    int i = varNameToIndex(name);
    if(vars[i].type == INT_TYPE) {
        vars[i].intVal = value;
    } else if(vars[i].type == FLOAT_TYPE) {
        vars[i].floatVal = value;
    } else {
        printf("%d:  Error: Cannot assign FLOAT_TYPE to %s \n", yylineno, numberTypeToName(vars[i].type));
        exit(1);
    }
}

void initializeVarWithString(char *name, char *value) {
    if(!doesVarExist(name)){
        printf("%d:  Error: Variable `%s` was not declared \n", yylineno, name);
        exit(1);
    }
    int i = varNameToIndex(name);
    if(vars[i].type == STRING_TYPE) {
        strcpy(vars[i].stringVal, value);
    } else {
        printf("%d:  Error: Cannot assign STRING_TYPE to %s \n", yylineno, numberTypeToName(vars[i].type));
        exit(1);
    }
}

void initializeVarWithBool(char *name, bool value) {
    if(!doesVarExist(name)){
        printf("%d:  Error: Variable `%s` was not declared \n", yylineno, name);
        exit(1);
    }
    int i = varNameToIndex(name);
    if(vars[i].type == BOOL_TYPE) {
        vars[i].boolVal = value;
    } else {
        printf("%d:  Error: Cannot assign BOOL_TYPE to %s \n", yylineno, numberTypeToName(vars[i].type));
        exit(1);
    }
}

void initializeVarWithChar(char *name, char value) {
    if(!doesVarExist(name)){
        printf("%d:  Error: Variable `%s` was not declared \n", yylineno, name);
        exit(1);
    }
    int i = varNameToIndex(name);
    if(vars[i].type == CHAR_TYPE) {
        vars[i].charVal = value;
    } else {
        printf("%d:  Error: Cannot assign CHAR_TYPE to %s \n", yylineno, numberTypeToName(vars[i].type));
        exit(1);
    }
}

//------- ASSIGNS -------

void assignVarToVar(char *name1, char *name2) {
     if(!doesVarExist(name1)){
        printf("%d:  Error: Variable `%s` was not declared \n", yylineno, name1);
        exit(1);
    }
    if(!doesVarExist(name2)){
        printf("%d:  Error: Variable `%s` was not declared \n", yylineno, name2);
        exit(1);
    }
    int i1 = varNameToIndex(name1);
    int i2 = varNameToIndex(name2);
    if(vars[i1].type != vars[i2].type) {
        printf("%d:  Error: Cannot assign type %s to %s \n", yylineno, numberTypeToName(vars[i2].type), numberTypeToName(vars[i1].type));
        exit(1);
    } else {
        if(vars[i1].type == INT_TYPE) {
            vars[i1].intVal = vars[i2].intVal;
        } else if(vars[i1].type == FLOAT_TYPE) {
            vars[i1].floatVal = vars[i2].floatVal;
        } else if(vars[i1].type == CHAR_TYPE) {
            vars[i1].charVal = vars[i2].charVal;
        } else if(vars[i1].type == STRING_TYPE) {
            strcpy(vars[i1].stringVal, vars[i2].stringVal);
        } else if(vars[i1].type == BOOL_TYPE) {
            vars[i1].boolVal = vars[i2].boolVal;
        } else {
            printf("%d:  Error: Cannot assign %s to %s \n", yylineno, numberTypeToName(vars[i1].type), numberTypeToName(vars[i2].type));
            exit(1);
        }
    }
}

void assignIntToVar(char *name, int value) {
    if(!doesVarExist(name)){
        printf("%d:  Error: Variable `%s` was not declared \n", yylineno, name);
        exit(1);
    }
    int i = varNameToIndex(name);
    if(vars[i].type == INT_TYPE) {
        if(!vars[i].isConst) {
            vars[i].intVal = value;
        } else {
            printf("%d:  Error: Cannot assign INT_TYPE to type const \n", yylineno);
            exit(1);
        }
    } else {
        printf("%d:  Error: Cannot assign INT_TYPE to %s \n", yylineno, numberTypeToName(vars[i].type));
        exit(1);
    }
}

void assignFloatToVar(char *name, float value) {
    if(!doesVarExist(name)){
        printf("%d:  Error: Variable `%s` was not declared \n", yylineno, name);
        exit(1);
    }
    int i = varNameToIndex(name);
    if(vars[i].type == INT_TYPE) {
        vars[i].intVal = value;
    } else if(vars[i].type == FLOAT_TYPE) {
        vars[i].floatVal = value;
    } else {
        printf("%d:  Error: Cannot assign FLOAT_TYPE to %s \n", yylineno, numberTypeToName(vars[i].type));
        exit(1);
    }
}

void assignCharToVar(char *name, char value) {
    if(!doesVarExist(name)){
        printf("%d:  Error: Variable `%s` was not declared \n", yylineno, name);
        exit(1);
    }
    int i = varNameToIndex(name);
    if(vars[i].type == CHAR_TYPE) {
        vars[i].charVal = value;
    } else {
        printf("%d:  Error: Cannot assign CHAR_TYPE to %s \n", yylineno, numberTypeToName(vars[i].type));
        exit(1);
    }
}

void assignStringToVar(char *name, char *value) {
    if(!doesVarExist(name)){
        printf("%d:  Error: Variable `%s` was not declared \n", yylineno, name);
        exit(1);
    }
    int i = varNameToIndex(name);
    if(vars[i].type == STRING_TYPE) {
        strcpy(vars[i].stringVal, value);
    } else {
        printf("%d:  Error: Cannot assign STRING_TYPE to %s \n", yylineno, numberTypeToName(vars[i].type));
        exit(1);
    }
}

void assignBoolToVar(char *name, bool value) {
    if(!doesVarExist(name)){
        printf("%d:  Error: Variable `%s` was not declared \n", yylineno, name);
        exit(1);
    }
    int i = varNameToIndex(name);
    if(vars[i].type == BOOL_TYPE) {
        vars[i].boolVal = value;
    } else {
        printf("%d:  Error: Cannot assign BOOL_TYPE to %s \n", yylineno, numberTypeToName(vars[i].type));
        exit(1);
    }
}
