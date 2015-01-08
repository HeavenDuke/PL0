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
	in_file.open("in.txt",ios::in);
	out_file.open("out.txt",ios::out);
	code.clear();
	char temp;
	while((temp=in_file.get())!=EOF){
		code+=temp;
	}
	code += '#';
	in_file.close();
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
	if(charindex==1){
		cout << "Line " << setw(5) << right << linenum << ": ";
	}
	ch=code[++index];
	charindex++;
	if(ch=='\n'){
		cout << endl;
		global_container.Display();
		global_container.Clear();
		charindex=1;
		linenum++;
	}
}

bool TokenAnalyzer::IsEndOfFile(){
	return (ch == '#');
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
		if (!IsLineEnd()){
			cout << ch;
		}
		GetChar();
	}
	if(IsAlpha()){
		ParseString();
		cout << token;
	}
	else if(IsNum()){
		ParseNum();
		cout << token;
	}
	else if(IsPunc()){
		ParsePunctuation();
		GetChar();
		cout << token;
	}
	else if(!IsEndOfFile()){
		while (IsEndOfFile() == false){
			if (!IsSpace() && !IsLineEnd() && !IsAlpha() && !IsNum() && !IsPunc()){
				Error(MyException(UNIDENTIFIED_CHARACTER, linenum, charindex - 2, "不合法的字符！"), false);
			}
			cout << ch;
			GetChar();
		}
		throw exception("unidentified character!");
	}
	DisplayResult();
}

void TokenAnalyzer::Error(MyException e,bool canrun){
	global_container.EnQueue(e);
	if (canrun == false){
		ContainError = canrun;
	}
}

void TokenAnalyzer::Error(int type, bool canrun,char *msg){
	global_container.EnQueue(MyException(type, linenum, charindex - strlen(token) - 2, msg));
}

void TokenAnalyzer::DisplayResult(){
	/*cout <<"code: "<<id_code<<endl;
	cout << "number: " << num << endl;
	cout <<"token: "<<token<<endl;*/

	out_file << "code: " << id_code << endl;
	out_file << "number: " << num << endl;
	out_file << "token: " << token << endl;
}

void TokenAnalyzer::Error(char *s){
	printf("%s\n",s);
}

void TokenAnalyzer::Clear(){
	memset(token,0,sizeof(token));
	num=0;
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
		if (sum < (INT_MAX - (ch - '0')) / 10){
			sum = sum * 10 + (ch - '0');
			token[index++] = ch;
		}
		else{
			overflow = true;
		}
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
	bool PreCheckReserved=false;
	while(IsAlpha()||(PreCheckReserved=IsNum())){
		if (i < MAXN){
			token[i++] = ch;
		}
		else{
			overflow = true;
		}
		GetChar();
	}
	if (overflow == true){
		memset(token, 0, sizeof(token));
		strcpy(token, "#unidentified#");
		Error(MyException(STRING_LENGTH_OVERFLOW, linenum, begin - 2, "标识符名称过长！"), false);
	}
	if(!PreCheckReserved){
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
			if(strcmp(token,"call")==0){
				return CALL_RESERVED;
			}
			else if(strcmp(token,"const")==0){
				return CONST_RESERVED;
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
			if (strcmp(token, "end") == 0){
				return END_RESERVED;
			}
			else if (strcmp(token, "else") == 0){
				return ELSE_RESERVED;
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
			if(strcmp(token,"read")==0){
				return READ_RESERVED;
			}
			else if (strcmp(token, "repeat") == 0){
				return REPEAT_RESERVED;
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
			if(strcmp(token,"while")==0){
				return WHILE_RESERVED;
			}
			else if(strcmp(token,"write")==0){
				return WRITE_RESERVED;
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