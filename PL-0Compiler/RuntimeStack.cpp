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
	freopen("con","r",stdin);
	freopen("con","w",stdout);
	cout << "START PL/0" << endl; 
	Piece p;
	try{
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
							if(dst[sp-1].value==INT_MAX){
								cout<<"Warning: 可能存在数值溢出"<<endl;
							}
							break;
						case ADD:
							Pop();
							dst[sp - 1].value += dst[sp].value;
							if(is_add_overflow(dst[sp-1].value,dst[sp].value)){
								cout<<"Warning: 可能存在数值溢出"<<endl;
							}
							break;
						case SUB:
							Pop();
							dst[sp - 1].value -= dst[sp].value;
							if(is_sub_overflow(dst[sp-1].value,dst[sp].value)){
								cout<<"Warning: 可能存在数值溢出"<<endl;
							}
							break;
						case MUL:
							Pop();
							if(is_mul_overflow(dst[sp-1].value,dst[sp].value)){
								cout<<"Warning: 可能存在数值溢出"<<endl;
							}
							dst[sp - 1].value *= dst[sp].value;
							break;
						case DIV:
							Pop();
							if(dst[sp].value==0){
								throw exception("Runtime Error: 分母为0！");
							}
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
					cout << "请输入：";
					cin >>result;
					if(cin.good()){
						dst[Base(index.Gap, bp) + index.Value].value=result;
					}
					else{
						throw exception("Runtime Error: 不合法的输入！(数值溢出或格式错误)");
					}
					break;
				case WRT://OVER
					cout << dst[sp - 1].value << endl;
					Pop();
					break;
			}
		}while (pc != 0);
	}catch(exception e){
		cout<<e.what()<<endl;
	}
	cout << "END PL/0" << endl;
}

bool RuntimeStack::is_mul_overflow(int a, int b) {
    if( a >= 0 && b >=0  ) {
        return INT_MAX / a < b;
    }
    else if( a < 0 && b < 0 ) {
        return INT_MAX / a > b;
    }
    else if( a * b == INT_MIN ) {
        return 0;
    }
    else {
        return a < 0 ? is_mul_overflow(-a, b) : is_mul_overflow(a, -b);
    }
}

bool RuntimeStack::is_add_overflow(int a, int b)
{
    int result = a+b;
    return (-(((a>=0) && (b>=0) && (result < 0)) || ((a<0) && (b<0) && (result >= 0))));
}

bool RuntimeStack::is_sub_overflow(int a, int b)
{
	int result=a-b;
    return (-(((a>0) && (b>0) && (result < 0)) || ((a<0) && (b<0) && (result > 0))));
}

int RuntimeStack::Base(int l, int b){
	while (l > 0){
		b = dst[b].value;
		l--;
	}
	return b;
}