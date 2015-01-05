#include "Header.h"
#ifndef SYMTABLE
#define SYMTABLE

#define CONSTANT false
#define VARIABLE true

typedef struct Symbol{
	char name[LEN];
	int value;
	bool kind;
	int adr;
	int proindex;
	int parent;
	Symbol(char *n,const int v, int a,int pro, bool k){
		strcpy(name, n);
		value = v;
		kind = k;
		adr = a;
		proindex = pro;
	}
	Symbol(){
		memset(name, 0, sizeof(name));
		value = 0;
		adr = 0;
		kind = VARIABLE;
		proindex = 0;
	}
	
	bool operator==(const Symbol &item) const
	{
		if (item.kind == VARIABLE){
			if (strcmp(name, item.name) == 0 && proindex == item.proindex){
				return 1;
			}
			else{
				return 0;
			}
		}
		else{
			if (strcmp(name, item.name) == 0){
				return 1;
			}
			else{
				return 0;
			}
		}
	}
};

typedef struct Procedure{
	char name[LEN];
	int level;
	Procedure(const char *n, int l){
		strcpy(name, n);
		level = l;
	};
	Procedure(){
		memset(name, 0, sizeof(name));
		level = -1;
	}
};

class SymTable{
	public:
		SymTable();
		~SymTable();
		void Add(Symbol s);
		bool Check(Symbol s);
		bool CheckPro(Procedure p);
		int AddPro(Procedure p);
		void Display();
	private:
		list<Symbol> SymbolTable;
		list<Procedure> ProcedureIndex;
};

#endif