#include "SymTable.h"

SymTable::SymTable(){
	
}

SymTable::~SymTable(){
	
}

void SymTable::Add(Symbol s){    //添加新符号
	int size = SymbolTable.size();
	SymbolTable.push_back(s);
}

bool SymTable::Check(Symbol s){   //自顶向下查找符号是否存在，仅从当前位置向上层查找。
	vector<Symbol>::reverse_iterator iter;
	int level = s.level;
	switch (s.kind){
		case PROCEDURE:
			//对于类型为过程的标识符
			//查找时要查找全部的相关标识符
			//不允许任何重名，一旦重名，就抛出异常
			for (iter = SymbolTable.rbegin(); iter != SymbolTable.rend(); iter++){
				if (strcmp(iter->name, s.name) == 0 && iter->level == s.level){
					return true;
				}
			}
			level = s.level-1;
			for (iter = SymbolTable.rbegin(); iter != SymbolTable.rend(); iter++){
				if (strcmp(iter->name, s.name) == 0 && iter->level == level){
					return true;
				}
				if (iter->kind == PROCEDURE){
					level--;
				}
			}
			return false;
			break;
		default:
			//对于其他类型
			//允许重名，但是不允许类型不同的重名
			for (iter = SymbolTable.rbegin(); iter != SymbolTable.rend(); iter++){//在当前变量所在的过程中查重
				if (strcmp(iter->name, s.name) == 0 && level == s.level){
					return true;
				}
				if (iter->kind == PROCEDURE){
					level--;
				}
			}
			level = s.level;
			for (iter = SymbolTable.rbegin(); iter != SymbolTable.rend(); iter++){//在上级过程中查重
				if (strcmp(iter->name, s.name) == 0 && iter->level == level){
					if (iter->kind != s.kind){
						return true;
					}
					else{
						return false;
					}
				}
				if (iter->kind == PROCEDURE){
					level--;
				}
			}
			return false;
			break;
	}
}

int SymTable::Locate(const char *name,int level){  //找在作用于范围内遇到的第一个元素，找不到则返回-1，找到则返回位置
	int index = SymbolTable.size() - 1;
	for (vector<Symbol>::reverse_iterator iter = SymbolTable.rbegin(); iter != SymbolTable.rend(); iter++){
		if (strcmp(name, iter->name) == 0&&iter->level<=level){
			return index;
		}
		if(iter->level<level){
			level=iter->level;
		}
		index--;
	}
	return -1;
}

void SymTable::Display(){//按照预设格式输出符号表，仅做测试用
	cout << endl;
	cout << setw(10) << right << "name" << setw(10) << right << "kind" << setw(10) << right << "value" << setw(10) << right << "level" << setw(10) << right << "addr" << endl;
	for (vector<Symbol>::iterator iter = SymbolTable.begin(); iter != SymbolTable.end(); iter++){
		 cout<< setw(10) << right << iter->name 
			 << setw(10) << right << iter->kind
			 << setw(10) << right << iter->value
			 << setw(10) << right << iter->level
			 << setw(10) << right << iter->adr << endl;
	}
}