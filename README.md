# LFAC-Project

This project implements a custom-designed programming language using YACC, focused on strong typing, user-defined data structures, and semantic correctness. The language is designed to include modern features such as type declarations, array support, control flow and functions, and abstract syntax tree (AST) evaluation for arithmetic expressions.

## Features
### Language Components
* **Primitve Types**: `int`, `float`, `char`, `string`, `bool`
* **Arrays**: Type-safe array declarations and usage
* **User-Defined Data Types**:
   * Similar to object-oriented classes
   * Custom syntax for defining fields and methods
   * Support for initialization and member access
* **Variables & Constants**: Declarations and definitions for variables and constats
* **Functions**: Definitions with parameter and return types
* **Control Flow**: Support for `if`, `for`, `while`, etc.
* **Assignments**:
   * Syntax: `left_value => expression`
   * ``left_value`` can be an identifier, array element, etc.
* **Expressions**: Arithmetic and boolean expressions
* **Function calls**: Arguments can be expressions, function calls, constants, identifiers, etc.

### Built-in Functions
* `Eval(arg)` - Evaluates arithmetic expressions and prints the result
* `TypeOf(arg)` - Returns the type of an expression; checks for type mismatches

## Program Structure
Programs in this language follow a structured layout with four sections:
1. **Global Variables**
2. **User Defined Types**
3. **Functions**
4. **Entry Point Function** - A special function that acts as `main`

## Symbol Table
Two symbol tables are generated per source program:
* `symbol_table.txt`:
   * Variables and constants with their types and values
* `symbol_table_functions.txt`:
   * Functions with return types and parameter details

## Semantic Analysis
The language includes comprehensive semantic validation:
* All variables/functions must be declared before use
* Duplicate declarations are disallowed
* Operands in expressions must have the same type (no implicit casting)
* Assignment sides must have matching types
* Function arguments must match declared parameter types
* Semantic errors prevent execution, and detailed error messages are provided

## Abstract Syntax Tree (AST)
Arithmetic expressions are parsed into ASTs for evaluation.

### AST Structure
* **Nodes**: Operators (e.g., +, -, *, /)
* **Leafs**: Operands (e.g., numbers, variables, array elements, function calls)

### AST Construction
```
buildAST(root, left_tree, right_tree, type);
```
* `root`: Operand or operator
* `left_tree` / `right_tree`: Subtrees
* `type`: Enum indicating the node type (NUMBER, IDENTIFIER, OPERATOR, etc.)

### AST Evaluation
```
int evalAST(ast);
```
* Leaf (number): returns the number
* Leaf (identifier): returns the variable’s value
* Operator node: recursively evaluates left and right subtrees, then combines values

## Error Handling
Semantic and syntactic errors are explicitly reported with line numbers and descriptions. Programs with any errors will not execute.

Example messages:
* ```32:  Error: Variable `@x1` already exists```
* ```14:  Error: Cannot define 2 functions with the same name and parameter types```

## Installation

### Prerequisites
* Lex
* Yacc
* Gcc Compiler

### Steps
1. Compile the parser:
   ```
   yacc -d compiler.y
   ``` 
2. Compile the lexer:
   ```
   lex compiler.l
   ```
3. Compile the generated C files:
   ```
   gcc lex.yy.c y.tab.c -o compiler
   ```

## Usage Examples
### ✅ Run the compiler on a syntactically correct file:
```
./compiler wrong.txt
```

### ❌ Run the compiler on a file with syntax errors:
```
./compiler correct.txt
```
