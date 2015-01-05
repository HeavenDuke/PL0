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
	cout << setw(10) << left << "name" << setw(10) << left << "kind" << setw(10) << left << "value" << setw(10) << left << "proc" << setw(10) << left << "addr" << endl;
	for (list<Symbol>::iterator iter = SymbolTable.begin(); iter != SymbolTable.end(); iter++){
		 cout<< setw(10) << left << iter->name 
			 << setw(10) << left << iter->kind
			 << setw(10) << left << iter->value
			 << setw(10) << left << iter->proindex
			 << setw(10) << left << iter->adr << endl;
	}
}