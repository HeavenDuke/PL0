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
		const Atoken GetToken();
		ErrorContainer global_container;
		~TokenAnalyzer();
	private:
		ifstream in_file;
		ofstream out_file;
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
		bool IsEndOfFile();
		void GetChar();
		bool IsLineEnd();
		void Retreat();
		void LoadFile();
		void ParseNum();
		void ParseString();
		void ParsePunctuation();
		void Error(char *);

		void DisplayResult();
};

#endif