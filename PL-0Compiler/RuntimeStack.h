#include "Header.h"
#include "SymTable.h"
#include "PCodeGenerator.h"
#ifndef RUNTIMESTACK_H
#define RUNTIMESTACK_H

#define DATA 1
#define RA 2
#define DL 3
#define SL 4
#define UN 5

typedef struct Piece{
	int bias;
	int type;
	int value;
	Piece(int b, int t, int v){
		bias = b;
		type = t;
		value = v;
	}
	Piece(){
		bias = 0;
		type = 0;
		value = 0;
	}
};

class RuntimeStack{
	public:
		RuntimeStack();
		~RuntimeStack();
		void Push(Piece p);
		Piece Pop();
		int GetSize();
		void Show();
		void Run(PCodeGenerator generator, SymTable table);
		int Base(int l, int b);
		bool is_mul_overflow(int a,int b);
		bool is_add_overflow(int a,int b);
		bool is_sub_overflow(int a,int b);
	private:
		int pc,
			bp,
			sp;
		Piece dst[MAXHASH];
};

#endif