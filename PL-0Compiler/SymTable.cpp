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
	vector<Symbol>::iterator it = find(SymbolTable.begin(), SymbolTable.end(), s);
	if (it != SymbolTable.end()){
		return true;
	}
	else{
		return false;
	}
}
/*int SymTable::Locate(const char *name,int level,const char* proname){
	int index = SymbolTable.size() - 1;
	for (vector<Symbol>::reverse_iterator iter = SymbolTable.rbegin(); iter != SymbolTable.rend(); iter++){
		if (strcmp(name, iter->name) == 0){
			if(iter->level<
				return index;
		}
		index--;
	}
	return -1;
}*/
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