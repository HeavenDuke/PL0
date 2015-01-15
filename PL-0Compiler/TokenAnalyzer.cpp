#include "TokenAnalyzer.h"

//构造函数，初始化所有相关参数，并由用户指定输入和输出文件
TokenAnalyzer::TokenAnalyzer(){
	index=-1;
	linenum=1;
	charindex=1;
	ContainError = false;
	num=0;
	LoadFile();
	GetChar();   //需要先读取一个字符
}

//重载赋值运算符
TokenAnalyzer& TokenAnalyzer::operator=(const TokenAnalyzer& analyzer){
	return TokenAnalyzer(analyzer);
}

//拷贝构造函数
TokenAnalyzer::TokenAnalyzer(const TokenAnalyzer& analyzer){
	index=analyzer.index;
	linenum=analyzer.linenum;
	charindex=analyzer.charindex;
	ContainError = analyzer.ContainError;
	num=analyzer.num;
	LoadFile();
	GetChar();
}

//构造Atoken结构体并向语法分析器传送
const Atoken TokenAnalyzer::GetToken(){
	Atoken atoken;
	atoken.Number = num;
	memset(atoken.Value, 0, sizeof(atoken.Value));
	strcpy(atoken.Value, token);
	atoken.Flag = id_code;
	return atoken;
}

//分析源程序中是否包含错误，true为是，false为否
bool TokenAnalyzer::HasError(){
	return ContainError;
}

//载入输入与输出文件
void TokenAnalyzer::LoadFile(){
	string path_in;
	string path_out;
	ifstream in_file;
	ofstream out_file;
	do{
		cout << "请指定输入文件路径:";
		cin >> path_in;
		in_file.open(path_in, ios::in);
		if (!in_file){
			cout << "路径不合法或文件不存在，请重新输入!";
		}
	} while (!in_file);
	do{
		cout << "请指定输入文件路径:";
		cin >> path_out;
		out_file.open(path_out, ios::out);
		if (!out_file){
			cout << "路径不合法或文件不存在，请重新输入!";
		}
	} while (!out_file);
	freopen(path_in.c_str(), "r", stdin);
	freopen(path_out.c_str(), "w", stdout);
	code.clear();
	char temp;
	while((temp=in_file.get())!=EOF){
		code+=temp;
	}
	in_file.close();
	out_file.close();
}

//解析分界符，单一冒号不被识别
void TokenAnalyzer::ParsePunctuation(){
	int index = 0;
	switch(ch){
		case ',':
			token[index++] = ',';
			id_code=COMMA_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case ';':
			token[index++] = ';';
			id_code=SEMICOLON_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case '.':
			token[index++] = '.';
			id_code=DOT_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case '=':
			token[index++] = '=';
			id_code=EQUAL_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case ':':
			token[index++] = ':';
			GetChar();
			if(ch=='='){
				token[index++] = '=';
				id_code=SET_OPERAND;
					//cout<<"Operand : \':=\'"<<endl;
			}
			else{
				Retreat();
				Error(COLON_WITHOUT_EQUAL,false,"冒号后必须包含等号");    //单一冒号不被接受
				//ERROR!
			}
			break;
		case '+':
			token[index++] = '+';
			id_code=PLUS_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case '-':
			token[index++] = '-';
			id_code=MINUS_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case '*':
			token[index++] = '*';
			id_code=MUL_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case '/':
			token[index++] = '/';
			id_code=DIV_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case '<':
			token[index++] = '<';
			GetChar();
			if(ch=='='){
				token[index++] = '=';
				id_code=LEQUAL_OPERAND;
					//cout<<"Operand : \'<=\'"<<endl;
			}
			else if(ch=='>'){
				token[index++] = '>';
				id_code=NEQUAL_OPERAND;
					//cout<<"Operand : \'<>\'"<<endl;
			}
			else{
				id_code=LESS_OPERAND;
					//cout<<"Operand : \'<\'"<<endl;
				Retreat();
			}
			break;
		case '>':
			token[index++] = '>';
			GetChar();
			if(ch=='='){
				token[index++] = '=';
				id_code=MEQUAL_OPERAND;
					//cout<<"Operand : \'>=\'"<<endl;
			}
			else{
				id_code=MORE_OPERAND;
					//cout<<"Operand : \'>\'"<<endl;
				Retreat();
			}
			break;
		case '(':
			token[index++] = '(';
			id_code=LBRACKET_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case ')':
			token[index++] = ')';
			id_code=RBRACKET_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		default:
			break;
	}
}

//获取下一个字符，同时更新行号和行字符序号
void TokenAnalyzer::GetChar(){
	if(IsEndOfFile()==false){
		ch=code[++index];
		charindex++;
		if(ch=='\n'){
			charindex=1;
			linenum++;
		}
	}
}

//判断是否为文件末尾
bool TokenAnalyzer::IsEndOfFile(){
	return (index>=int(code.length()));
}


//回退一个字符
void TokenAnalyzer::Retreat(){
	ch=code[--index];
}

//判断是否为空格
bool TokenAnalyzer::IsSpace(){
	if (ch == ' '||ch=='\t'){
		return true;
	}
	return false;
}

//判断是否为行末
bool TokenAnalyzer::IsLineEnd(){
	if(ch=='\n'){
		return true;
	}
	return false;
}

//执行词法分析
void TokenAnalyzer::Run(){
	Clear();    //首先清空上一轮的结果
	while(IsSpace()||IsLineEnd()){  
		GetChar();   //清除行首空白字符
	}
	if(IsAlpha()){
		ParseString();   //如果以字母，那么必然为标识符，进入标识符解析子单元
	}
	else if(IsNum()){
		ParseNum();      //如果以数字开头，则必然为常量，进入常量解析子单元
	}
	else if(IsPunc()){   //如果为PL0接受的标点，则必然为分界符，进入分界符解析子单元
		ParsePunctuation();
		GetChar();
	}
	else if(!IsEndOfFile()){ //否则如果不是行末，则一直跳读至文件末尾，并汇报错误
		Error(MyException(UNIDENTIFIED_CHARACTER, linenum, charindex - 2, "不合法的字符！"), false);
		throw exception("unidentified character!");
	}
}

//用于词法分分析的报错函数
void TokenAnalyzer::Error(MyException e,bool canrun){
	global_container.EnQueue(e);
	if (canrun == false){
		ContainError = !canrun;
	}
}

//用于语法分析的报错函数
void TokenAnalyzer::Error(int type, bool canrun,char *msg){
	global_container.EnQueue(MyException(type, linenum, charindex - strlen(token) - 2, msg));
	if (canrun == false){
		ContainError = !canrun;
	}
}

//按照约定输出结果，将错误容器中的错误与源程序综合
void TokenAnalyzer::DisplayResult(){
	int line = 1;
	string temp = code;
	while (temp.find("\n") != temp.npos){
		cout << "Line " << setw(5) << right << line << ": " << temp.substr(0, temp.find("\n")) << endl;
		temp.erase(0, temp.find("\n") + 1);
		while (global_container.IsEmpty() == false && global_container.GetFront().GetRow() == line){
			MyException exp = global_container.DeQueue();
			cout << "**** " << setw(7 + exp.GetColumn() + 1) << right << "^" << exp.GetType() << ":" << exp.what() << endl;
		}
		line++;
	}
	cout << "Line " << setw(5) << right << line << ": " << temp.substr(0, temp.find("\n")) << endl;
	while (global_container.IsEmpty() == false && global_container.GetFront().GetRow() == line){
		MyException exp = global_container.DeQueue();
		cout << "**** " << setw(7 + exp.GetColumn() + 1) << right << "^" << exp.GetType() << ":" << exp.what() << endl;
	}
	line++;
}

//输出错误，用于调试
void TokenAnalyzer::Error(char *s){
	printf("%s\n",s);
}

//清除上一轮结果
void TokenAnalyzer::Clear(){
	memset(token,0,sizeof(token));
	num=0;
	id_code=-1;
}

//判断是否为常数
bool TokenAnalyzer::IsNum(){
	if(ch>='0'&&ch<='9'){
		return true;
	}
	return false;
}

//判断是否为字母
bool TokenAnalyzer::IsAlpha(){
	if((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z')){
		return true;
	}
	return false;
}

//判断是否为识别标点
bool TokenAnalyzer::IsPunc(){
	bool res=true;
	switch(ch){
		case ',':
		case ';':
		case '.':
		case '=':
		case ':':
		case '+':
		case '-':
		case '*':
		case '/':
		case '<':
		case '>':
		case '(':
		case ')':
			res=true;
			break;
		default:
			res=false;
			break;
	}
	return res;
}

//解析常数
void TokenAnalyzer::ParseNum(){
	int sum=0;
	int index = 0;
	bool overflow = false;
	int begin = charindex;
	while(IsNum()){
		if (sum >= (INT_MAX - (ch - '0')) / 10){
			overflow = true;
		}
		sum = sum * 10 + (ch - '0');
		token[index++] = ch;
		GetChar();
	}
	if (overflow == true){
		sum = 0;
		Error(MyException(NUMERIC_OVERFLOW, linenum, begin - 2, "这个数太大！"), false);
	}
	id_code=CONST_NUMBER;
	num=sum;
}

//解析标识符
void TokenAnalyzer::ParseString(){
	bool overflow = false;
	int begin = charindex;
	int i=0;
	bool PreCheckReserved=true;
	do{
		if(IsNum()){  //预判，如果标识符中包含数字，则一定不是保留字
			PreCheckReserved=false;
		}
		if (i < MAXN){   //预判，如果标识符名称过长，则报错
			token[i++] = ch;
		}
		else{
			overflow = true;
		}
		GetChar();
	}while((IsAlpha()||IsNum())&&!IsEndOfFile());
	if (overflow == true){
		memset(token, 0, sizeof(token));
		strcpy(token, "#unidentified#");
		Error(MyException(STRING_LENGTH_OVERFLOW, linenum, begin - 2, "标识符名称过长！"), false);
	}
	if(PreCheckReserved==true){//解析保留字
		id_code=IsReserved();
	}
	else{  //否则为常量
		id_code=IDENTIFIER;
	}
}


//用伪字典树的方法查找保留字表
int TokenAnalyzer::IsReserved(){
	switch(token[0]){
		case 'b':
			if(strcmp(token,"begin")==0){
				return BEGIN_RESERVED;
			}
			else{
				return IDENTIFIER;
			}
			break;
		case 'c':
			if(token[1]=='a'){
				if(strcmp(token,"call")==0){
					return CALL_RESERVED;
				}
				else{
					return IDENTIFIER;
				}
			}
			else if(token[1]=='o'){
				if(strcmp(token,"const")==0){
					return CONST_RESERVED;
				}
				else{
					return IDENTIFIER;
				}
			}
			else{
				return IDENTIFIER;
			}
			break;
		case 'd':
			if (strcmp(token, "do") == 0){
				return DO_RESERVED;
			}
			else{
				return IDENTIFIER;
			}
			break;
		case 'e':
			if(token[1]=='n'){
				if(strcmp(token,"end")==0){
					return END_RESERVED;
				}
				else{
					return IDENTIFIER;
				}
			}
			else if(token[1]=='l'){
				if(strcmp(token,"else")==0){
					return ELSE_RESERVED;
				}
				else{
					return IDENTIFIER;
				}
			}
			else{
				return IDENTIFIER;
			}
			break;
		case 'i':
			if(strcmp(token,"if")==0){
				return IF_RESERVED;
			}
			else{
				return IDENTIFIER;
			}
			break;
		case 'o':
			if (strcmp(token, "odd") == 0){
				return ODD_RESERVED;
			}
			else{
				return IDENTIFIER;
			}
			break; 
		case 'p':
			if(strcmp(token,"procedure")==0){
				return PROCEDURE_RESERVED;
			}
			else{
				return IDENTIFIER;
			}
			break;
		case 'r':
			if(token[1]=='e'){
				if(token[2]=='a'){
					if(strcmp(token,"read")==0){
						return READ_RESERVED;
					}
					else{
						return IDENTIFIER;
					}
				}
				else if(token[2]=='p'){
					if(strcmp(token,"repeat")==0){
						return REPEAT_RESERVED;
					}
					else{
						return IDENTIFIER;
					}
				}
				else{
					return IDENTIFIER;
				}
			}
			else{
				return IDENTIFIER;
			}
			break;
		case 't':
			if (strcmp(token, "then") == 0){
				return THEN_RESERVED;
			}
			else{
				return IDENTIFIER;
			}
			break;
		case 'u':
			if (strcmp(token, "until") == 0){
				return UNTIL_RESERVED;
			}
			else{
				return IDENTIFIER;
			}
			break;
		case 'v':
			if(strcmp(token,"var")==0){
				return VAR_RESERVED;
			}
			else{
				return IDENTIFIER;
			}
			break;
		case 'w':
			if(token[1]=='h'){
				if(strcmp(token,"while")==0){
					return WHILE_RESERVED;
				}
				else{
					return IDENTIFIER;
				}
			}
			else if(token[1]=='r'){
				if(strcmp(token,"write")==0){
					return WRITE_RESERVED;
				}
				else{
					return IDENTIFIER;
				}
			}
			else{
				return IDENTIFIER;
			}
			break;
		default:
			return IDENTIFIER;
			break;
	}
}

//析构函数
TokenAnalyzer::~TokenAnalyzer(){

}