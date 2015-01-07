#include "RuntimeStack.h"

RuntimeStack::RuntimeStack(){
	base = 0;
	top = 0;
}

RuntimeStack::~RuntimeStack(){

}

void RuntimeStack::Push(Piece p){
	dst[top++] = p;
}

Piece RuntimeStack::Pop(){
	return dst[--top];
}

int RuntimeStack::GetSize(){
	return top;
}

void RuntimeStack::Run(PCodeGenerator g, SymTable t){
	int index = 0;
	int value1;
	int value2;
	int begin;
	Piece p;
	Push(Piece(0, UN, 0));
	Push(Piece(0, UN, 0));
	Push(Piece(0, UN, 0));
	base = 0;
	while (index < g.GetSize()){
		cout << index << endl;
		PCode pc = g.GetCommand(index);
		switch (pc.Type){
			case LIT:
				Push(Piece(GetSize(), DATA, pc.Value));
				index++;
				break;
			case OPR:
				switch (pc.Value){
					case RETURN:
						top = base;
						index = dst[base + 2].value;
						base = dst[base + 1].value;
						break;
					case NEG:
						p = Pop();
						p.value *= -1;
						Push(p);
						break;
					case ADD:
						value1 = Pop().value;
						value2 = Pop().value;
						Push(Piece(GetSize(), DATA, value1 + value2));
						break;
					case SUB:
						value1 = Pop().value;
						value2 = Pop().value;
						Push(Piece(GetSize(), DATA, value1 - value2));
						break;
					case MUL:
						value1 = Pop().value;
						value2 = Pop().value;
						Push(Piece(GetSize(), DATA, value1 * value2));
						break;
					case DIV:
						value1 = Pop().value;
						value2 = Pop().value;
						Push(Piece(GetSize(), DATA, value1 / value2));
						break;
					case ODD:
						p = Pop();
						p.value %= 2;
						Push(p);
						break;
					case EQL:
						value1 = Pop().value;
						value2 = Pop().value;
						Push(Piece(GetSize(), DATA, value1 == value2));
						break;
					case NEQ:
						value1 = Pop().value;
						value2 = Pop().value;
						Push(Piece(GetSize(), DATA, value1 != value2));
						break;
					case LSS:
						value1 = Pop().value;
						value2 = Pop().value;
						Push(Piece(GetSize(), DATA, value1 < value2));
						break;
					case GTR:
						value1 = Pop().value;
						value2 = Pop().value;
						Push(Piece(GetSize(), DATA, value1 > value2));
						break;
					case LEQ:
						value1 = Pop().value;
						value2 = Pop().value;
						Push(Piece(GetSize(), DATA, value1 <= value2));
						break;
					case GEQ:
						value1 = Pop().value;
						value2 = Pop().value;
						Push(Piece(GetSize(), DATA, value1 >= value2));
						break;
				}
				index++;
				break;
			case LOD:
				begin = base;
				for (int i = 0; i < pc.Gap; i++){
					begin = dst[begin].value;
				}
				Push(Piece(GetSize(), DATA, dst[begin + pc.Value].value));
				index++;
				break;
			case STO://OVER
				begin = base;
				for (int i = 0; i < pc.Gap; i++){
					begin = dst[begin].value;
				}
				Push(Piece(GetSize(), DATA, dst[top - 1].value));
				index++;
				break;
			case CAL:
				Push(Piece(GetSize(), SL, Base(t, index)));//TODO:确定SL指向
				Push(Piece(GetSize(), DL, base));
				Push(Piece(GetSize(), RA, index + 1));
				index = pc.Value;
				base = GetSize() - 3;
				break;
			case INT://OVER
				for (int i = 0; i < pc.Value - 3; i++){
					Push(Piece(GetSize(), DATA, 0));
				}
				index++;
				break;
			case JMP://OVER
				index = pc.Value;
				break;
			case JPC://OVER
				if (dst[top - 1].value == 0){
					index = pc.Value;
				}
				else{
					index++;
				}
				break;
			case RED://OVER
				int result;
				cout << "请输入：";
				cin >> result;
				begin = base;
				for (int i = 0; i < pc.Gap; i++){
					begin = dst[begin].value;
				}
				Push(Piece(GetSize(), DATA, result));
				index++;
				break;
			case WRT://OVER
				cout << dst[top - 1].value << endl;
				index++;
				break;
		}
	}
}

int RuntimeStack::Base(SymTable table, int index){
	return 0;
}