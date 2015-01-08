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
		
		int SubProcedure(int level, bool isroot, char *name, int prev, set<int> fsys);
		void Sentence(int level, int begin, set<int> fsys);
		void Condition(int level, set<int> fsys);
		void Expression(int level, set<int> fsys);
		void Item(int level, set<int> fsys);
		void Factor(int level, set<int> fsys);
		set<int> Union(set<int> set1, set<int> set2);
		void Show(){
			table.Display();
			generator.Show();
		}
		
		void ConstDeclaration(int level);
		void VarDeclaration(int level, int &addr, int &variablenum);
		void BeginDeclaration(int level, int begin, set<int> fsys);
		void AssignDeclaration(int level, set<int> fsys);
		void WriteDeclaration(int level, set<int> fsys);
		void WhileDeclaration(int level, int begin, set<int> fsys);
		void ReadDeclaration(int level, set<int> fsys);
		void RepeatDeclaration(int level, int begin, set<int> fsys);
		void IfDeclaration(int level, int begin, set<int> fsys);
		void CallDeclaration(int level, set<int> fsys);
		
		void Test(set<int>s1, set<int>s2, int Type);
		void Analysis();
		void Run(){
			try{
				Procedure();
				Show();
				if (analyzer.HasError() == false){
					Analysis();
				}
			}
			catch (exception e){
				//cout << e.what() << endl;
				Show();
				if (analyzer.HasError() == false){
					Analysis();
				}
			}
		}
	private:
		set<int> declbegsys;
		set<int> statbegsys;
		set<int> facbegsys;
		TokenAnalyzer analyzer;
		SymTable table;
		PCodeGenerator generator;
		RuntimeStack rstack;
};

#endif