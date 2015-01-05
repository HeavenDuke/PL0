#include "Header.h"
#include "TokenAnalyzer.h"
#include "ErrorContainer.h"
#include "SymTable.h"
#ifndef GRAMMARANALYZER_H
#define GRAMMARANALYZER_H

class GrammarAnalyzer{
	public:
		GrammarAnalyzer();
		~GrammarAnalyzer();
		void SubProcedure(int level, bool isroot, char *name,int index);
		void Sentence();
		void Condition();
		void Expression();
		void Item();
		void Factor();
		void Show(){ table.Display(); }
	private:
		TokenAnalyzer analyzer;
		SymTable table;
};

#endif