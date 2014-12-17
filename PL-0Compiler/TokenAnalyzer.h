#include "Header.h"
#ifndef TOKENANALYZER_H
#define TOKENANALYZER_H
#define MAXN 255
#define MAXLEN 100000
class TokenAnalyzer
{
	public:
		TokenAnalyzer();
		void Run();
		void TestRun();
		~TokenAnalyzer();
	private:
		ifstream infile;
		ofstream outfile;
		int linenum;
		int charindex;
		char ch;
		char token[MAXN];
		int num;
		int index;
		string code;
		void Clear();
		bool IsNum();
		bool IsAlpha();
		bool IsPunc();
		bool IsReserved();
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