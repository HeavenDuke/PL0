#include "Header.h"
#include "TokenAnalyzer.h"
#include "ErrorContainer.h"
#include "SymTable.h"
#include "RuntimeStack.h"
#include "PCodeGenerator.h"
#ifndef GRAMMARANALYZER_H
#define GRAMMARANALYZER_H

typedef struct Adjust{
	int index;
	int addr;
	Adjust(int i, int a){
		index = i;
		addr = a;
	}
	Adjust(){
		index = 0;
		addr = 0;
	}
};

class GrammarAnalyzer{
	public:
		GrammarAnalyzer();
		~GrammarAnalyzer();
		int SubProcedure(int level, bool isroot, char *name, int prev);
		void Sentence(int level, int begin);
		void Condition(int level);
		void Expression(int level);
		void Item(int level);
		void Factor(int level);
		void Show(){
			table.Display();
			generator.Show();
		}
		void Analysis();
		void Run(){
			try{
				SubProcedure(0, true, "", -1);
			}
			catch (exception e){
				//cout << e.what() << endl;
				generator.Add(OPR, 0, 0);
				Show();
				Analysis();
			}
		}
	private:
		TokenAnalyzer analyzer;
		SymTable table;
		PCodeGenerator generator;
		RuntimeStack rstack;
};

#endif