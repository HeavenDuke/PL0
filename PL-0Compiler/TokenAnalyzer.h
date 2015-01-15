#include "Header.h"
#include "ErrorContainer.h"
#include "MyException.h"
#ifndef TOKENANALYZER_H
#define TOKENANALYZER_H

typedef struct{
	int Flag;
	int Number;
	char Value[MAXN];
}Atoken;

class TokenAnalyzer
{
	public:
		TokenAnalyzer& operator=(const TokenAnalyzer& analyzer);
		TokenAnalyzer(const TokenAnalyzer& analyzer);
		TokenAnalyzer();
		~TokenAnalyzer();

		void Run();
		void TestRun();
		void Retreat();

		const Atoken GetToken();
		
		bool IsEndOfFile();
		
		void Error(MyException m, bool canrun);
		void Error(int type, bool canrun, char* msg);

		void DisplayResult();
		bool HasError();
	private:
		ErrorContainer global_container;
		bool ContainError;
		int linenum;
		int charindex;

		char token[MAXN];
		int num;
		int id_code;
		
		int index;
		string code;
		char ch;

		void Clear();
		void GetChar();
		void LoadFile();
		void Error(char *);

		bool IsNum();
		bool IsAlpha();
		bool IsPunc();
		int IsReserved();
		bool IsSpace();
		bool IsLineEnd();
		
		void ParseNum();
		void ParseString();
		void ParsePunctuation();
};

#endif