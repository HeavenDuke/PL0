#include "SymTable.h"

SymTable::SymTable(){
	
}

SymTable::~SymTable(){
	
}

void SymTable::Add(Symbol s){
	int size = SymbolTable.size();
	SymbolTable.push_back(s);
}

bool SymTable::Check(Symbol s){
	vector<Symbol>::reverse_iterator iter;
	int level = s.level;
	switch (s.kind){
		case PROCEDURE:
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
			for (iter = SymbolTable.rbegin(); iter != SymbolTable.rend(); iter++){
				if (strcmp(iter->name, s.name) == 0 && level == s.level){
					return true;
				}
				if (iter->kind == PROCEDURE){
					level--;
				}
			}
			level = s.level;
			for (iter = SymbolTable.rbegin(); iter != SymbolTable.rend(); iter++){
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

int SymTable::Locate(const char *name){
	int index = SymbolTable.size() - 1;
	for (vector<Symbol>::reverse_iterator iter = SymbolTable.rbegin(); iter != SymbolTable.rend(); iter++){
		if (strcmp(name, iter->name) == 0){
			return index;
		}
		index--;
	}
	return -1;
}

void SymTable::Display(){
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