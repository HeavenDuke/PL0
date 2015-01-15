#include "Header.h"
#ifndef SYMTABLE
#define SYMTABLE


//定义kind字段的值
#define CONSTANT 1
#define VARIABLE 2
#define PROCEDURE 3


//定义符号的数据结构
typedef struct Symbol{
	char name[LEN];  //名字属性
	int value;       //值属性
	int kind;        //类型属性
	int adr;         //地址偏移属性
	int level;       //层级属性
	Symbol(const char *n, const int v, int a, int k, int l){
		strcpy(name, n);
		value = v;
		kind = k;
		adr = a;
		level = l;
	}
	Symbol(){
		memset(name, 0, sizeof(name));
		value = 0;
		adr = 0;
		kind = VARIABLE;
		level = 0;
	}
	
	bool operator==(const Symbol &item) const
	{
		if (strcmp(name, item.name) == 0 && level == item.level){
			return 1;
		}
		else{
			return 0;
		}
	}
};

/***************************************************************************************
SymTable-符号表管理模块
用于在语义分析的过程中进行查找、定位、获取、修改、插入等操作
主要功能：
(1)查找是否已经存在一个与待插入标识符相同且处于对应作用域的标识符（Procedure与其他分别对待）
(2)插入符号至符号表
(3)根据需要获取特定符号
(4)根据特定需要修改特定符号的属性值
***************************************************************************************/

class SymTable{
	public:
		SymTable();				//构造函数
		~SymTable();            //析构函数
		void Add(Symbol s);     //添加新符号
		bool Check(Symbol s);   //检查符号是否已存在
		void Remove();          //删除符号
		Symbol GetSymbol(int index){ return SymbolTable[index]; }   //Getter
		int Locate(const char *name,int level); //定位某一符
		void Display();         //显示符号表
	private:
		vector<Symbol> SymbolTable;  //符号表（使用vector进行存储，方便查找）
};

#endif