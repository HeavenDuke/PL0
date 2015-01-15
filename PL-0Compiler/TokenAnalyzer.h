#include "Header.h"
#include "ErrorContainer.h"
#include "MyException.h"
#ifndef TOKENANALYZER_H
#define TOKENANALYZER_H

//用于给词法分析器向语法分析器传递消息的结构体
typedef struct{
	int Flag;
	int Number;
	char Value[MAXN];
}Atoken;

/********************************************************************
TokenAnalyzer-词法分析器
用于读取源程序，并从源程序中获得一个个的单词，存储在词法分析器内部。
并根据语法分析的需要，将这些解析出的单词，向语法分析器传递自身得到的单词。
主要功能：
（1）顺序读取源程序，识别源程序中的单词，或者报错。
（2）可以接受的字母表：a-zA-Z0-9 := = > >= <= <> < + - * / \n space \t .
（3）保留字表：begin,call,const,do,else,end,if,odd,procedure,read,repeat,then,until,var,while,write
（4）不识别：除（2）外的任何字符，遇到则报错，组合分界符不可拆分识别。
（5）可以动态载入数据，同时能够一次性载入一串字符，从而提高IO效率。
（6）包含错误存储模块，语法分析出错时通过调用该模块进行错误传递。
********************************************************************/

class TokenAnalyzer
{
	public:
		TokenAnalyzer& operator=(const TokenAnalyzer& analyzer);   //重载赋值运算符
		TokenAnalyzer(const TokenAnalyzer& analyzer);              //拷贝构造函数
		TokenAnalyzer();                                           //构造函数
		~TokenAnalyzer();                                          //析构函数

		void Run();                                                //运行词法分析模块，解析一个单词
		void TestRun();                                            //测试用函数，不被使用
		void Retreat();                                            //回退一位。

		const Atoken GetToken();                                   //获取单词数据，用于向语法分析模块传递数据
		
		bool IsEndOfFile();										   //判断是否运行至文件尾
		
		void Error(MyException m, bool canrun);                    //报错，用于词法分析器
		void Error(int type, bool canrun, char* msg);              //报错，用与语法分析器

		void DisplayResult();									   //输出解析结果，包含源代码与错误模块
		bool HasError();                                           //判断是否包含错误，如果包含错误，解释器将不会运行，P-CODE将不会被执行
	private:
		ErrorContainer global_container;						   //错误存储模块
		bool ContainError;                                         //用于判断是否存在错误
		int linenum;                                               //当前行号
		int charindex;                                             //当前行所读取到的字符序号

		char token[MAXN];                                          //用于存储标识符或保留字，最长不得超过255
		int num;                                                   //用于存储常量，一定是正数，但是不得超过INT_MAX
		int id_code;                                               //用于存储当前存储的单词的类别，具体类别见Header.h中的约定
		
		int index;												   //当前总字符序号
		string code;                                               //用于存储代码片的字符串
		char ch;                                                   //当前字符

		void Clear();                                              //清除上一轮的词法分析结果，token置为空串，num置为0，id_code置为默认值
		void GetChar();											   //读取下一个字符
		void LoadFile();                                           //载入文件，重定向IO至用户输入的输入及输出文件
		void Error(char *);                                        //测试用报错函数

		bool IsNum();			//判断当前字符是否为数字
		bool IsAlpha();         //判断当前字符是否为字母
		bool IsPunc();          //判断当前字符是否可能为识别标点
		int IsReserved();       //判断当前标识符是否为保留字
		bool IsSpace();         //判断当前字符是否为空格（包含space与\t）
		bool IsLineEnd();       //判断当前字符是否为行末
		
		void ParseNum();         //解析常数
		void ParseString();      //解析字符串，识别标识符
		void ParsePunctuation(); //解析分界符
};

#endif