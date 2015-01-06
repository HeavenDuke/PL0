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
#include<map>
#include<stack>
#include<algorithm>
#include<list>

#define BEGIN_RESERVED 1
#define CALL_RESERVED 2
#define CONST_RESERVED 3
#define DO_RESERVED 4
#define ELSE_RESERVED 5
#define END_RESERVED 6
#define IF_RESERVED 7
#define ODD_RESERVED 8
#define PROCEDURE_RESERVED 9
#define READ_RESERVED 10
#define REPEAT_RESERVED 11
#define THEN_RESERVED 12
#define UNTIL_RESERVED 13
#define VAR_RESERVED 14
#define WHILE_RESERVED 15
#define WRITE_RESERVED 16
#define IDENTIFIER 17
#define CONST_NUMBER 18
#define PLUS_OPERAND 19
#define MINUS_OPERAND 20
#define DOT_OPERAND 21
#define COMMA_OPERAND 22
#define SEMICOLON_OPERAND 23
#define EQUAL_OPERAND 24
#define SET_OPERAND 25
#define LBRACKET_OPERAND 26
#define RBRACKET_OPERAND 27
#define MUL_OPERAND 28
#define DIV_OPERAND 29
#define NEQUAL_OPERAND 30
#define LESS_OPERAND 31
#define LEQUAL_OPERAND 32
#define MORE_OPERAND 33
#define MEQUAL_OPERAND 34

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