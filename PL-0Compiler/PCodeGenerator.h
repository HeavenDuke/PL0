#include "Header.h"

#ifndef PCODEGENERATOR_H
#define PCODEGENERATOR_H

//定义指令代码
#define LIT 1
#define OPR 2
#define LOD 3
#define STO 4
#define CAL 5
#define INT 6
#define JMP 7
#define JPC 8
#define RED 9
#define WRT 10

//定义对于OPR指令，a的代码
#define RETURN 0
#define NEG 1
#define ADD 2
#define SUB 3
#define MUL 4
#define DIV 5
#define ODD 6
#define MOD 7
#define EQL 8
#define NEQ 9
#define LSS 10
#define GEQ 11
#define GTR 12
#define LEQ 13

//P-CODE数据结构
typedef struct PCode{
	int Type;
	int Gap;
	int Value;
	PCode(int t, int g, int v){
		Type = t;
		Gap = g;
		Value = v;
	}
	PCode(){
		Type = 0;
		Gap = 0;
		Value = 0;
	}
};

/***************************************************************************
PCodeGenerator-语义分析与中间代码生成模块
实际行为穿插在语法分析模块中进行
主要功能：
(1)进行语义分析，识别语法成分，生成相应的P-Code
(2)出现错误调用错误处理模块进行处理。
***************************************************************************/


class PCodeGenerator{
	public:
		PCodeGenerator(){};                     //构造函数
		~PCodeGenerator(){};                    //析构函数
		void Add(int type, int gap, int value); //添加新指令
		void Show();                            //输出全部P-CODE
		int GetSize();                          //获取指令容器长度
		int Redirect(int index);                //重定位分程序入口
		void AdjustEntry();                     //重新定位程序入口
		void AdjustJump(int index, int addr);   //重定位（用于while\if\repeat结构）
		PCode GetCommand(int index);            //获得特定位置的P-Code
	private:
		const char * Select(int value);         //将指令码转换为指令名
		vector<PCode> codelist;					//指令容器
};

#endif