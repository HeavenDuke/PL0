#include "SymTable.h"

SymTable::SymTable(){
	
}

SymTable::~SymTable(){
	
}

void SymTable::Add(Symbol s){
	int size = SymbolTable.size();
	SymbolTable.push_back(s);
}

int SymTable::AddPro(Procedure p){
	p.begin = SymbolTable.size();
	ProcedureIndex.push_back(p);
	return ProcedureIndex.size();
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

bool SymTable::CheckPro(Procedure p){
	return false;
}

int SymTable::Locate(const char *name, int level, int proindex){
	int index = SymbolTable.size() - 1;
	for (vector<Symbol>::reverse_iterator iter = SymbolTable.rbegin(); iter != SymbolTable.rend(); iter++){
		if (strcmp(name, iter->name) == 0){
			if (iter->proindex == proindex){
				return index;
			}
			else if (iter->level < level){
				if (index < ProcedureIndex[proindex - 1].begin){
					return index;
				}
			}
		}
		index--;
	}
	return -1;
}

void SymTable::Display(){
	cout << setw(15) << left << "name" << setw(15) << left << "level" << setw(15) << left << "begin" << endl;
	for (vector<Procedure>::iterator iter = ProcedureIndex.begin(); iter != ProcedureIndex.end(); iter++){
		cout << setw(15) << left << iter->level << setw(15) << left << iter->name << setw(15) << left << iter->begin << endl;
	}
	cout << setw(10) << left << "name" << setw(10) << left << "kind" << setw(10) << left << "value" << setw(10) << left << "proc" << setw(10) << left << "addr" << endl;
	for (vector<Symbol>::iterator iter = SymbolTable.begin(); iter != SymbolTable.end(); iter++){
		 cout<< setw(10) << left << iter->name 
			 << setw(10) << left << iter->kind
			 << setw(10) << left << iter->value
			 << setw(10) << left << iter->proindex
			 << setw(10) << left << iter->adr << endl;
	}
}