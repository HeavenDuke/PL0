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
#include<queue>
#include<algorithm>
#include<set>
#include<list>


/***************************************************
Header.h-用于存储全局信息与约定的文件
主要功能:
（1）包含所有可能用到的头文件，减少include次数
（2）包含词法分析中的保留字表
（3）包含错误处理模块的错误码表
（3）定义全局长度与规模约束，诸如标识符名称长度
***************************************************/

//定义最大程序嵌套层数
#define MAXLEVEL 2


//定义词汇表
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


//定义错误码表
#define EQUAL_NOT_SET 1
#define NUMBER_AFTER_EQUAL 2
#define EQUAL_AFTER_IDENTIFIER 3
#define IDENTIFIER_AFTER_CVP 4
#define MISSING_COMMA_OR_SEMI 5
#define INCORRECT_SYMBOL_AFTER_PROCEDURE 6
#define SHOULD_BE_SENTENCE 7
#define INCORRECT_AFTER_SENTENCE_INSIDE_PROCEDURE 8
#define SHOULD_BE_DOT 9
#define MISSING_SEMI_BETWEEN 10
#define UNKNOWN_IDENTIFIER 11
#define CANNOT_ASSIGN 12
#define SHOULD_BE_SET 13
#define IDENTIFIER_AFTER_CALL 14
#define CANNOT_REFER 15
#define SHOULD_BE_THEN 16
#define SHOULD_BE_END_OR_SEMI 17
#define SHOULD_BE_DO 18
#define INCORRECT_AFTER_SENTENCE 19
#define SHOULD_BE_RELATION 20
#define CANNOT_HAVE_PROCEDURE 21
#define MISSING_RIGHT_BRACKET 22
#define CANNOT_AFTER_FACTOR 23
#define CANNOT_START_WITH_THIS 24
#define UNIDENTIFIED_CHARACTER 25
#define COLON_WITHOUT_EQUAL 26
#define WRITE_PROCEDURE 27
#define NUMERIC_OVERFLOW 31
#define STRING_LENGTH_OVERFLOW 32
#define READ_CONST 33
#define READ_UNIDENTIFIED 34
#define READ_PROCEDURE 35
#define SHOULD_BE_LEFT_BRACKET 36
#define TOODEEP 37
#define DUPLICATE_DECLARATION 38

//定义当前语法分析子程序类型
#define SUBPROCEDURE 1
#define SENTENCE 2
#define CONDITION 3
#define EXPRESSION 4
#define FACTOR 5
#define ITEM 6

//定义其他相关最大限制
#define MAXHASH 10000
#define LEN 255
#define MAXP 100
#define MAXN 255
#define MAXLEN 100000

using namespace std;