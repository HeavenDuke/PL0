#include "Header.h"
#include "TokenAnalyzer.h"
#include "ErrorContainer.h"
#include "SymTable.h"
#include "RuntimeStack.h"
#include "PCodeGenerator.h"
#ifndef GRAMMARANALYZER_H
#define GRAMMARANALYZER_H

/********************************************************************************
GrammarAnalyzer语法分析模块
用于将词法分析器给出的结果解析出一系列语法成分，并转交给语义分析模块。
由于使用面向对象的设计方案，因此GrammarAnalyzer将语法分析模块、语义分析及中间代码生成模块、错误处理模块、符号表管理模块、解释器模块存储在同一个类中
同时尽可能保持原有的模块化格局不变
主要功能：
（1）语法分析，识别源代码是否符合PL0文法的语法规范，如果存在不和语法规范的部分，则需要报错
（2）语义分析，并生成中间代码，如果存在不和语义规范的部分，则需要报错
（3）错误处理，首先生成错误信息，并存储于包含在词法分析器中的错误存储模块，等待源程序解析完成时输出。同时在子程序末尾跳读一段，确保编译继续进行。
（4）符号表管理，确保作用于内标识符的合法性，不合法时需要报错。
（5）解释器。在语法分析模块完全完成之后，如果没有任何编译时错误，则解释中间代码并执行，如果运行时发生错误，则汇报错误，抛出异常，遇到不可挽回的错误时直接停止程序运行。
********************************************************************************/

class GrammarAnalyzer{
	public:
		GrammarAnalyzer();    //构造函数
		~GrammarAnalyzer();   //析构函数
		
		void Procedure();    //程序子程序
		int SubProcedure(int level, bool isroot, char *name, int prev, set<int> fsys);//分程序子程序
		void Sentence(int level, int begin, set<int> fsys);  //语句子程序
		void Condition(int level, set<int> fsys);        //条件子程序
		void Expression(int level, set<int> fsys);       //表达式子程序
		void Item(int level, set<int> fsys);             //项子程序
		void Factor(int level, set<int> fsys);           //因子子程序
		
		static char *GetMessage(int Type);               //根据错误码获取错误提示信息
		void Test(set<int>s1, set<int>s2, int Type);     //用于测试及跳读
		void Analysis();                                 //解释中间代码并执行
		void Run();                                      //语法分析器运行，事实上是完整系统的运行
		set<int> Union(set<int> set1, set<int> set2);    //将两个符号集包含在同一个中
		void Show();                                     //输出编译结果
		
		void ConstDeclaration(int level);                              //常量解析
		void VarDeclaration(int level, int &addr, int &variablenum);   //变量解析
		void BeginDeclaration(int level, int begin, set<int> fsys);    //begin解析
		void AssignDeclaration(int level, set<int> fsys);              //赋值解析
		void WriteDeclaration(int level, set<int> fsys);               //write解析
		void WhileDeclaration(int level, int begin, set<int> fsys);    //while解析
		void ReadDeclaration(int level, set<int> fsys);                //read解析
		void RepeatDeclaration(int level, int begin, set<int> fsys);   //repeat解析
		void IfDeclaration(int level, int begin, set<int> fsys);       //if解析
		void CallDeclaration(int level, set<int> fsys);                //call解析
	private:

		set<int> declbegsys;      //声明对应的开始符号集
		set<int> statbegsys;      //语句对应的开始符号集
		set<int> facbegsys;       //因子对应的开始符号集
		TokenAnalyzer analyzer;   //词法分析器
		SymTable table;           //符号表管理
		PCodeGenerator generator; //语义分析中间代码生成
		RuntimeStack rstack;      //运行时解释器
};

#endif