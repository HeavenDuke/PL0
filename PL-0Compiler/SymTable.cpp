#include "SymTable.h"

SymTable::SymTable(){
	memset(HashTable, 0, sizeof(HashTable));
	memset(symbols, 0, sizeof(symbols));
	memset(ProcedureIndex, 0, sizeof(ProcedureIndex));
}

SymTable::~SymTable(){
	memset(HashTable, 0, sizeof(HashTable));
	memset(symbols, 0, sizeof(symbols));
	memset(ProcedureIndex, 0, sizeof(ProcedureIndex));
}