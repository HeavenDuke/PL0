#include "SymTable.h"

SymTable::SymTable(){
	
}

SymTable::~SymTable(){
	
}

void SymTable::Add(Symbol s){
	int size = SymbolTable.size();
	if (s.kind == VARIABLE){
		list<Symbol>::reverse_iterator iter;
		int num = SymbolTable.size();
		for (iter = SymbolTable.rbegin(); iter != SymbolTable.rend(); iter++){
			num--;
			if (strcmp(iter->name, s.name) == 0){
				break;
			}
		}
		if (iter == SymbolTable.rend()){
			s.parent = -1;
		}
		else{
			s.parent = num;
		}
	}
	else{
		s.parent = -1;
	}
	SymbolTable.push_back(s);
}

int SymTable::AddPro(Procedure p){
	ProcedureIndex.push_back(p);
	return ProcedureIndex.size();
}

bool SymTable::Check(Symbol s){
	list<Symbol>::iterator it = find(SymbolTable.begin(), SymbolTable.end(), s);
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

void SymTable::Display(){
	cout << setw(15) << left << "name" << setw(15) << left << "level" << endl;
	for (list<Procedure>::iterator iter = ProcedureIndex.begin(); iter != ProcedureIndex.end(); iter++){
		cout << setw(15) << left << iter->level << setw(15) << left << iter->name << endl;
	}
	cout << setw(10) << left << "name" << setw(10) << left << "kind" << setw(10) << left << "value" << setw(10) << left << "proc" << setw(10) << left << "addr" << setw(10) << left << "parent" << endl;
	for (list<Symbol>::iterator iter = SymbolTable.begin(); iter != SymbolTable.end(); iter++){
		 cout<< setw(10) << left << iter->name 
			 << setw(10) << left << iter->kind
			 << setw(10) << left << iter->value
			 << setw(10) << left << iter->proindex
			 << setw(10) << left << iter->adr
			 << setw(10) << left << iter->parent << endl;
	}
}