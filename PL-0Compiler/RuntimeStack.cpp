#include "RuntimeStack.h"

RuntimeStack::RuntimeStack(){
	pc = 0;
	sp = 0;
	bp = 0;
}

RuntimeStack::~RuntimeStack(){

}

void RuntimeStack::Push(Piece p){
	if(sp>=MAXHASH){                                //����Ϊ����ԭ��ջ��Ԫ�س�������ֵ
		throw exception("Runtime Error: ջ���");   //��ô�׳��쳣
	}
	else{
		dst[sp++] = p;                              //������ջ
	}
}

Piece RuntimeStack::Pop(){
	if(sp<=0){
		throw exception("Runtime Error: ջ���");    //���ջΪ�գ���ô�׳��쳣
	}
	else{
		return dst[--sp];                            //�����ջ
	}
}

int RuntimeStack::GetSize(){                        //��ȡ����ջ��С
	return sp;
}

void RuntimeStack::Show(){                          //�������������
	cout << "**************************************" << endl;
	for (int i = 0; i < sp; i++){
		cout << dst[i].value<<endl;
	}
	cout << "**************************************" << endl;
}

void RuntimeStack::Run(PCodeGenerator g, SymTable t){   //���������������ջ����
	freopen("con","r",stdin);     //�ض�������
	freopen("con","w",stdout);    //�ض������
	cout << "START PL/0" << endl; 
	Piece p;
	try{
		do{
			//cout << pc << endl;
			PCode index = g.GetCommand(pc++);  //��һ����ָ����
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
							if(dst[sp-1].value==INT_MIN){     //�����з���������������ΪINT_MINʱ��ȡ�෴���ᷢ����ֵ���
								cout<<"Warning: ���ܴ�����ֵ���"<<endl;
							}
							break;
						case ADD:
							Pop();
							dst[sp - 1].value += dst[sp].value;
							if(is_add_overflow(dst[sp-1].value,dst[sp].value)){  //�ӷ��������
								cout<<"Warning: ���ܴ�����ֵ���"<<endl;
							}
							break;
						case SUB:
							Pop();
							dst[sp - 1].value -= dst[sp].value;
							if(is_sub_overflow(dst[sp-1].value,dst[sp].value)){   //�����������
								cout<<"Warning: ���ܴ�����ֵ���"<<endl;
							}
							break;
						case MUL:
							Pop();
							if(is_mul_overflow(dst[sp-1].value,dst[sp].value)){   //�˷��������
								cout<<"Warning: ���ܴ�����ֵ���"<<endl;
							}
							dst[sp - 1].value *= dst[sp].value;
							break;
						case DIV:
							Pop();
							if(dst[sp].value==0){             //������ĸΪ0
								throw exception("Runtime Error: ��ĸΪ0��");
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
						default:
							throw exception("Runtime Error: ��ʶ���ָ��");   //���ִ���ָ��׳��쳣
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
					cout << "�����룺";
					cin >>result;
					if(cin.good()){
						dst[Base(index.Gap, bp) + index.Value].value=result;
					}
					else{
						throw exception("Runtime Error: ���Ϸ������룡(��ֵ������ʽ����)");   //�û�����������ݣ��׳��쳣
					}
					break;
				case WRT://OVER
					cout << dst[sp - 1].value << endl;
					Pop();
					break;
				default:
					throw exception("Runtime Error: ����ʶ���ָ�");      //���ִ���ָ��׳��쳣
					break;
			}
		}while (pc != 0);
	}catch(exception e){
		cout<<e.what()<<endl;
	}
	cout << "END PL/0" << endl;
}

bool RuntimeStack::is_mul_overflow(int a, int b) {//�����жϳ˷��쳣
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

bool RuntimeStack::is_add_overflow(int a, int b)//�����жϼӷ��쳣
{
    int result = a+b;
    return (-(((a>=0) && (b>=0) && (result < 0)) || ((a<0) && (b<0) && (result >= 0))));
}

bool RuntimeStack::is_sub_overflow(int a, int b)//�����жϼ����쳣
{
	int result=a-b;
    return (-(((a>0) && (b>0) && (result < 0)) || ((a<0) && (b<0) && (result > 0))));
}

int RuntimeStack::Base(int l, int b){  //��ȡ��̬������һ����¼��ַ
	while (l > 0){
		b = dst[b].value;
		l--;
	}
	return b;
}