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
#define IDENTIFIER 10
#define CONST_NUMBER 11
#define PLUS_OPERAND 12
#define MINUS_OPERAND 13
#define DOT_OPERAND 14
#define COMMA_OPERAND 15
#define SEMICOLON_OPERAND 16
#define EQUAL_OPERAND 17
#define SET_OPERAND 18
#define LBRACKET_OPERAND 19
#define RBRACKET_OPERAND 20
#define MUL_OPERAND 21
#define DIV_OPERAND 22
#define NEQUAL_OPERAND 23
#define LESS_OPERAND 24
#define LEQUAL_OPERAND 25
#define MORE_OPERAND 26
#define MEQUAL_OPERAND 27

#define TOKEN_RESULT_STRING -1
#define TOKEN_RESULT_NUMBER -2
#define TOKEN_ERROR -3
#define TOKEN_WARNING -4

using namespace std;