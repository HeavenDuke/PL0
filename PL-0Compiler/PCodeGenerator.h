#include "Header.h"

#ifndef PCODEGENERATOR_H
#define PCODEGENERATOR_H

#define LIT 1
#define OPR 2
#define LOD 3
#define STO 4
#define CAL 5
#define INT 6
#define JMP 7
#define JPC 8
#define RED 9
#define WRT 10

#define RETURN 0
#define NEG 1
#define ADD 2
#define SUB 3
#define MUL 4
#define DIV 5
#define ODD 6
#define MOD 7
#define EQL 8
#define NEQ 9
#define LSS 10
#define GEQ 11
#define GTR 12
#define LEQ 13

typedef struct PCode{
	int Type;
	int Gap;
	int Value;
	PCode(int t, int g, int v){
		Type = t;
		Gap = g;
		Value = v;
	}
	PCode(){
		Type = 0;
		Gap = 0;
		Value = 0;
	}
};


class PCodeGenerator{
	public:
		PCodeGenerator(){}
		~PCodeGenerator(){};
		void Add(int type, int gap, int value);
		void Show();
	private:
		list<PCode> codelist;
};

#endif