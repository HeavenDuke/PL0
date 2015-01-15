#include "RuntimeStack.h"

RuntimeStack::RuntimeStack(){
	pc = 0;
	sp = 0;
	bp = 0;
}

RuntimeStack::~RuntimeStack(){

}

void RuntimeStack::Push(Piece p){
	if(sp>=MAXHASH){                                //若因为各种原因，栈顶元素超过上限值
		throw exception("Runtime Error: 栈溢出");   //那么抛出异常
	}
	else{
		dst[sp++] = p;                              //否则入栈
	}
}

Piece RuntimeStack::Pop(){
	if(sp<=0){
		throw exception("Runtime Error: 栈溢出");    //如果栈为空，那么抛出异常
	}
	else{
		return dst[--sp];                            //否则出栈
	}
}

int RuntimeStack::GetSize(){                        //获取数据栈大小
	return sp;
}

void RuntimeStack::Show(){                          //测试用输出函数
	cout << "**************************************" << endl;
	for (int i = 0; i < sp; i++){
		cout << dst[i].value<<endl;
	}
	cout << "**************************************" << endl;
}

void RuntimeStack::Run(PCodeGenerator g, SymTable t){   //解释器运行与符号栈管理
	freopen("con","r",stdin);     //重定向输入
	freopen("con","w",stdout);    //重定向输出
	cout << "START PL/0" << endl; 
	Piece p;
	try{
		do{
			//cout << pc << endl;
			PCode index = g.GetCommand(pc++);  //拿一个新指令来
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
							if(dst[sp-1].value==INT_MIN){     //对于有符号整数，当数据为INT_MIN时，取相反数会发生数值溢出
								cout<<"Warning: 可能存在数值溢出"<<endl;
							}
							break;
						case ADD:
							Pop();
							dst[sp - 1].value += dst[sp].value;
							if(is_add_overflow(dst[sp-1].value,dst[sp].value)){  //加法数据溢出
								cout<<"Warning: 可能存在数值溢出"<<endl;
							}
							break;
						case SUB:
							Pop();
							dst[sp - 1].value -= dst[sp].value;
							if(is_sub_overflow(dst[sp-1].value,dst[sp].value)){   //减法数据溢出
								cout<<"Warning: 可能存在数值溢出"<<endl;
							}
							break;
						case MUL:
							Pop();
							if(is_mul_overflow(dst[sp-1].value,dst[sp].value)){   //乘法数据溢出
								cout<<"Warning: 可能存在数值溢出"<<endl;
							}
							dst[sp - 1].value *= dst[sp].value;
							break;
						case DIV:
							Pop();
							if(dst[sp].value==0){             //除法分母为0
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
						default:
							throw exception("Runtime Error: 不识别的指令");   //出现错误指令，抛出异常
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
						throw exception("Runtime Error: 不合法的输入！(数值溢出或格式错误)");   //用户输入错误数据，抛出异常
					}
					break;
				case WRT://OVER
					cout << dst[sp - 1].value << endl;
					Pop();
					break;
				default:
					throw exception("Runtime Error: 不被识别的指令！");      //出现错误指令，抛出异常
					break;
			}
		}while (pc != 0);
	}catch(exception e){
		cout<<e.what()<<endl;
	}
	cout << "END PL/0" << endl;
}

bool RuntimeStack::is_mul_overflow(int a, int b) {//用来判断乘法异常
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

bool RuntimeStack::is_add_overflow(int a, int b)//用来判断加法异常
{
    int result = a+b;
    return (-(((a>=0) && (b>=0) && (result < 0)) || ((a<0) && (b<0) && (result >= 0))));
}

bool RuntimeStack::is_sub_overflow(int a, int b)//用来判断减法异常
{
	int result=a-b;
    return (-(((a>0) && (b>0) && (result < 0)) || ((a<0) && (b<0) && (result > 0))));
}

int RuntimeStack::Base(int l, int b){  //获取静态链上上一层活动记录基址
	while (l > 0){
		b = dst[b].value;
		l--;
	}
	return b;
}