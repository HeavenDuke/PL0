#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<exception>
#include<iomanip>
#include<memory>
#include<cmath>
#include<ctime>
#include<cstring>
#include<string>
#include<vector>

#define BEGIN_RESERVED 1
#define CALL_RESERVED 2
#define CONST_RESERVED 3
#define IF_RESERVED 4
#define PROCEDURE_RESERVED 5
#define READ_RESERVED 6
#define VAR_RESERVED 7
#define WHILE_RESERVED 8
#define WRITE_RESERVED 9
#define IDENTIFIER_RESERVED 10
#define IDENTIFIER 11
#define CONST_NUMBER 12
#define PLUS_OPERAND 13
#define MINUS_OPERAND 14
#define DOT_OPERAND 15
#define COMMA_OPERAND 16
#define SEMICOLON_OPERAND 17
#define EQUAL_OPERAND 18
#define SET_OPERAND 19
#define LBRACKET_OPERAND 20
#define RBRACKET_OPERAND 21
#define MUL_OPERAND 22
#define DIV_OPERAND 23
#define NEQUAL_OPERAND 24
#define LESS_OPERAND 25
#define LEQUAL_OPERAND 26
#define MORE_OPERAND 27
#define MEQUAL_OPERAND 28

#define TOKEN_RESULT -1
#define TOKEN_ERROR -2
#define TOKEN_WARNING -3

using namespace std;