#include "Header.h"
#include "SymTable.h"
#include "PCodeGenerator.h"
#ifndef RUNTIMESTACK_H
#define RUNTIMESTACK_H

//程序栈中单一元素的类型
#define DATA 1
#define RA 2
#define DL 3
#define SL 4
#define UN 5

//程序栈中单一元素的数据结构
typedef struct Piece{
	int bias;
	int type;
	int value;
	Piece(int b, int t, int v){
		bias = b;
		type = t;
		value = v;
	}
	Piece(){
		bias = 0;
		type = 0;
		value = 0;
	}
};

/***************************************************************
运行时程序栈管理模块
包含解释器与符号栈管理
仅仅在运行时执行。
功能：
(1)在语法分析中不包含编译错误的情况下，解释执行从语法分析模块得到的P-code
(2)执行过程中维护与管理数据区，数据区以栈式存储
(3)在执行过程出现异常时，抛出异常，或给出警告

***************************************************************/


class RuntimeStack{
	public:
		RuntimeStack();          //构造函数
		~RuntimeStack();         //析构函数
		void Push(Piece p);      //将数据压入数据栈
		Piece Pop();             //从数据栈中弹出数据
		int GetSize();           //获取数据栈长度
		void Show();             //显示当前数据栈，仅用于测试
		void Run(PCodeGenerator generator, SymTable table);    //运行运行时管理模块
		int Base(int l, int b);                                //获取静态链上上一层活动单元的基址
		bool is_mul_overflow(int a,int b);                     //检测乘法是否溢出
		bool is_add_overflow(int a,int b);                     //检测加法是否溢出
		bool is_sub_overflow(int a,int b);                     //检测减法是否溢出
	private:
		int pc,    //指向当前正在执行的P-Code
			bp,    //指向当前活动记录的基地址
			sp;    //指向栈顶
		Piece dst[MAXHASH];  //符号栈
};

#endif