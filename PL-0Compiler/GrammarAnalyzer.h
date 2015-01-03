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
		void SubProcedure();
		void Sentence();
		void Condition();
		void Expression();
		void Item();
		void Factor();
	private:
		TokenAnalyzer analyzer;
		SymTable table;
};

#endif