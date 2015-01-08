#include "RuntimeStack.h"

RuntimeStack::RuntimeStack(){
	pc = 0;
	sp = 0;
	bp = 0;
}

RuntimeStack::~RuntimeStack(){

}

void RuntimeStack::Push(Piece p){
	dst[sp++] = p;
}

Piece RuntimeStack::Pop(){
	return dst[--sp];
}

int RuntimeStack::GetSize(){
	return sp;
}

void RuntimeStack::Show(){
	cout << "**************************************" << endl;
	for (int i = 0; i < sp; i++){
		cout << dst[i].value<<endl;
	}
	cout << "**************************************" << endl;
}

void RuntimeStack::Run(PCodeGenerator g, SymTable t){
	cout << "START PL/0" << endl; 
	Piece p;
	do{
		//cout << pc << endl;
		PCode index = g.GetCommand(pc++);
		switch (index.Type){
			case LIT:
				Push(Piece(GetSize(), DATA, index.Value));
				break;
			case OPR:
				switch (index.Value){
					case RETURN:
						sp = bp;
						pc = dst[sp + 2].value;
						bp = dst[sp + 1].value;
						break;
					case NEG:
						dst[sp - 1].value *= -1;
						break;
					case ADD:
						Pop();
						dst[sp - 1].value += dst[sp].value;
						break;
					case SUB:
						Pop();
						dst[sp - 1].value -= dst[sp].value;
						break;
					case MUL:
						Pop();
						dst[sp - 1].value *= dst[sp].value;
						break;
					case DIV:
						Pop();
						dst[sp - 1].value /= dst[sp].value;
						break;
					case ODD:
						dst[sp - 1].value %= 2;
						break;
					case EQL:
						Pop();
						dst[sp - 1].value = (dst[sp - 1].value == dst[sp].value);
						break;
					case NEQ:
						Pop();
						dst[sp - 1].value = (dst[sp - 1].value != dst[sp].value);
						break;
					case LSS:
						Pop();
						dst[sp - 1].value = (dst[sp - 1].value < dst[sp].value);
						break;
					case GTR:
						Pop();
						dst[sp - 1].value = (dst[sp - 1].value > dst[sp].value);
						break;
					case LEQ:
						Pop();
						dst[sp - 1].value = (dst[sp - 1].value <= dst[sp].value);
						break;
					case GEQ:
						Pop();
						dst[sp - 1].value = (dst[sp - 1].value >= dst[sp].value);
						break;
				}
				break;
			case LOD:
				Push(Piece(sp, DATA, dst[Base(index.Gap, bp) + index.Value].value));
				break;
			case STO://OVER
				Pop();
				dst[Base(index.Gap, bp) + index.Value].value = dst[sp].value;
				break;
			case CAL:
				dst[sp].value = Base(index.Gap, bp);
				dst[sp + 1].value = bp;
				dst[sp + 2].value = pc;
				bp = sp;
				pc = index.Value;
				break;
			case INT://OVER
				sp += index.Value;
				break;
			case JMP://OVER
				pc = index.Value;
				break;
			case JPC://OVER
				Pop();
				if (dst[sp].value == 0){
					pc = index.Value;
				}
				break;
			case RED://OVER
				int result;
				cout << "ÇëÊäÈë£º";
				cin >> dst[Base(index.Gap, bp) + index.Value].value;
				break;
			case WRT://OVER
				cout << dst[sp - 1].value << endl;
				Pop();
				break;
		}
		//Show();
	}while (pc != 0);
	cout << "END PL/0" << endl;
}

int RuntimeStack::Base(int l, int b){
	while (l > 0){
		b = dst[b].value;
		l--;
	}
	return b;
}