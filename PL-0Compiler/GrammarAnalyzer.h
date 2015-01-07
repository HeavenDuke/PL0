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
		void Procedure();
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
				Procedure();
				Show();
			}
			catch (exception e){
				//cout << e.what() << endl;
				Show();
				//Analysis();
			}
		}
	private:
		TokenAnalyzer analyzer;
		SymTable table;
		PCodeGenerator generator;
		RuntimeStack rstack;
};

#endif