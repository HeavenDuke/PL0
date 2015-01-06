#include "Header.h"
#ifndef SYMTABLE
#define SYMTABLE

#define CONSTANT 1
#define VARIABLE 2
#define PROCEDURE 3

typedef struct Symbol{
	char name[LEN];
	int value;
	int kind;
	int adr;
	int level;
	Symbol(char *n, const int v, int a, int k, int l){
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

class SymTable{
	public:
		SymTable();
		~SymTable();
		void Add(Symbol s);
		bool Check(Symbol s);
		void Remove();
		Symbol GetSymbol(int index){
			return SymbolTable[index];
		}
		int Locate(const char *name);
		void Display();
	private:
		vector<Symbol> SymbolTable;
};

#endif