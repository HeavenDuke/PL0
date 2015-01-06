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
	cout << setw(10) << left << "name" << setw(10) << left << "kind" << setw(10) << left << "value" << setw(10) << left << "level" << setw(10) << left << "addr" << endl;
	for (vector<Symbol>::iterator iter = SymbolTable.begin(); iter != SymbolTable.end(); iter++){
		 cout<< setw(10) << left << iter->name 
			 << setw(10) << left << iter->kind
			 << setw(10) << left << iter->value
			 << setw(10) << left << iter->level
			 << setw(10) << left << iter->adr << endl;
	}
}