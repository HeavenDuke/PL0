#include "Header.h"

#ifndef SYMTABLE
#define SYMTABLE

typedef struct{
	char name[LEN];
	bool isConst;
	int link;
}Symbol;

class SymTable{
	public:
		SymTable();
		~SymTable();
	private:
		int HashTable[MAXHASH];
		Symbol symbols[MAXHASH];
		int ProcedureIndex[MAXP];
};

#endif