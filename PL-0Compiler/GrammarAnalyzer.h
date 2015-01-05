#include "Header.h"
#include "TokenAnalyzer.h"
#include "ErrorContainer.h"
#include "SymTable.h"
#include "PCodeGenerator.h"
#ifndef GRAMMARANALYZER_H
#define GRAMMARANALYZER_H

class GrammarAnalyzer{
	public:
		GrammarAnalyzer();
		~GrammarAnalyzer();
		void SubProcedure(int level, bool isroot, char *name,int index);
		void Sentence(int level, int indexnumber);
		void Condition(int level, int indexnumber);
		void Expression(int level, int indexnumber);
		void Item(int level, int indexnumber);
		void Factor(int level, int indexnumber);
		void Show(){
			table.Display();
			generator.Show();
		}
	private:
		TokenAnalyzer analyzer;
		SymTable table;
		PCodeGenerator generator;
};

#endif