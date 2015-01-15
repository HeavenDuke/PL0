#include "TokenAnalyzer.h"

TokenAnalyzer::TokenAnalyzer(){
	index=-1;
	linenum=1;
	charindex=1;
	ContainError = false;
	num=0;
	LoadFile();
	GetChar();
}

TokenAnalyzer& TokenAnalyzer::operator=(const TokenAnalyzer& analyzer){
	return TokenAnalyzer(analyzer);
}

TokenAnalyzer::TokenAnalyzer(const TokenAnalyzer& analyzer){
	index=analyzer.index;
	linenum=analyzer.linenum;
	charindex=analyzer.charindex;
	ContainError = analyzer.ContainError;
	num=analyzer.num;
	LoadFile();
	GetChar();
}

const Atoken TokenAnalyzer::GetToken(){
	Atoken atoken;
	atoken.Number = num;
	memset(atoken.Value, 0, sizeof(atoken.Value));
	strcpy(atoken.Value, token);
	atoken.Flag = id_code;
	return atoken;
}

bool TokenAnalyzer::HasError(){
	return ContainError;
}

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

bool TokenAnalyzer::IsEndOfFile(){
	return (index>=int(code.length()));
}

void TokenAnalyzer::Retreat(){
	ch=code[--index];
}

bool TokenAnalyzer::IsSpace(){
	if (ch == ' '){
		return true;
	}
	return false;
}

bool TokenAnalyzer::IsLineEnd(){
	if(ch=='\n'){
		return true;
	}
	return false;
}

void TokenAnalyzer::Run(){
	Clear();
	while(IsSpace()||IsLineEnd()){
		GetChar();
	}
	if(IsAlpha()){
		ParseString();
	}
	else if(IsNum()){
		ParseNum();
	}
	else if(IsPunc()){
		ParsePunctuation();
		GetChar();
	}
	else if(!IsEndOfFile()){
		Error(MyException(UNIDENTIFIED_CHARACTER, linenum, charindex - 2, "不合法的字符！"), false);
		throw exception("unidentified character!");
	}
}

void TokenAnalyzer::Error(MyException e,bool canrun){
	global_container.EnQueue(e);
	if (canrun == false){
		ContainError = !canrun;
	}
}

void TokenAnalyzer::Error(int type, bool canrun,char *msg){
	global_container.EnQueue(MyException(type, linenum, charindex - strlen(token) - 2, msg));
	if (canrun == false){
		ContainError = !canrun;
	}
}

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

void TokenAnalyzer::Error(char *s){
	printf("%s\n",s);
}

void TokenAnalyzer::Clear(){
	memset(token,0,sizeof(token));
	num=0;
	id_code=-1;
}

bool TokenAnalyzer::IsNum(){
	if(ch>='0'&&ch<='9'){
		return true;
	}
	return false;
}

bool TokenAnalyzer::IsAlpha(){
	if((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z')){
		return true;
	}
	return false;
}

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

void TokenAnalyzer::ParseString(){
	bool overflow = false;
	int begin = charindex;
	int i=0;
	bool PreCheckReserved=true;
	do{
		if(IsNum()){
			PreCheckReserved=false;
		}
		if (i < MAXN){
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
	if(PreCheckReserved==true){
		id_code=IsReserved();
	}
	else{
		id_code=IDENTIFIER;
	}
}

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

TokenAnalyzer::~TokenAnalyzer(){

}