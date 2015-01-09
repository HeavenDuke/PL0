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
		TokenAnalyzer();
		void Run();
		void TestRun();
		void Retreat();
		const Atoken GetToken();
		bool IsEndOfFile();
		void Error(MyException m, bool canrun);
		void Error(int type, bool canrun, char* msg);
		void Display(){ global_container.Display(); }
		void DisplayResult();
		bool HasError();
		~TokenAnalyzer();

		ErrorContainer global_container;
	private:
		ifstream in_file;
		ofstream out_file;
		bool ContainError;
		int linenum;
		int charindex;
		char ch;
		char token[MAXN];
		int num;
		int index;
		int id_code;
		string code;

		void Clear();
		bool IsNum();
		bool IsAlpha();
		bool IsPunc();
		int IsReserved();
		bool IsSpace();
		void GetChar();
		bool IsLineEnd();
		void LoadFile();
		void ParseNum();
		void ParseString();
		void ParsePunctuation();
		void Error(char *);
};

#endif