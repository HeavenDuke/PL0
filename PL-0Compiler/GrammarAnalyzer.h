#include "Header.h"
#include "TokenAnalyzer.h"
#include "ErrorContainer.h"
#include "SymTable.h"
#include "RuntimeStack.h"
#include "PCodeGenerator.h"
#ifndef GRAMMARANALYZER_H
#define GRAMMARANALYZER_H

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
		
		void ConstDeclaration(int level);
		void VarDeclaration(int level, int &addr, int &variablenum);
		void BeginDeclaration(int level, int begin);
		void AssignDeclaration(int level);
		void WriteDeclaration(int level);
		void WhileDeclaration(int level, int begin);
		void ReadDeclaration(int level);
		void RepeatDeclaration(int level, int begin);
		void IfDeclaration(int level, int begin);
		void CallDeclaration(int level);
		
		void Test(set<int>s1, set<int>s2, int Type);
		void Analysis();
		void Run(){
			try{
				Procedure();
				Show();
				if (analyzer.HasError() == false){
					//Analysis();
				}
			}
			catch (exception e){
				//cout << e.what() << endl;
				Show();
				if (analyzer.HasError() == false){
					//Analysis();
				}
			}
		}
	private:
		TokenAnalyzer analyzer;
		SymTable table;
		PCodeGenerator generator;
		RuntimeStack rstack;
};

#endif