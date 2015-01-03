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
#define DO_RESERVED 4
#define END_RESERVED 5
#define IF_RESERVED 6
#define ODD_RESERVED 7
#define PROCEDURE_RESERVED 8
#define READ_RESERVED 9
#define THEN_RESERVED 10
#define VAR_RESERVED 11
#define WHILE_RESERVED 12
#define WRITE_RESERVED 13
#define IDENTIFIER 14
#define CONST_NUMBER 15
#define PLUS_OPERAND 16
#define MINUS_OPERAND 17
#define DOT_OPERAND 18
#define COMMA_OPERAND 19
#define SEMICOLON_OPERAND 20
#define EQUAL_OPERAND 21
#define SET_OPERAND 22
#define LBRACKET_OPERAND 23
#define RBRACKET_OPERAND 24
#define MUL_OPERAND 25
#define DIV_OPERAND 26
#define NEQUAL_OPERAND 27
#define LESS_OPERAND 28
#define LEQUAL_OPERAND 29
#define MORE_OPERAND 30
#define MEQUAL_OPERAND 31

#define TOKEN_RESULT_STRING -1
#define TOKEN_RESULT_NUMBER -2
#define TOKEN_ERROR -3
#define TOKEN_WARNING -4

#define MAXHASH 1000
#define LEN 255
#define MAXP 100
#define MAXN 255
#define MAXLEN 100000

using namespace std;