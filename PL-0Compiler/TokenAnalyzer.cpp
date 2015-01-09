#include "TokenAnalyzer.h"

TokenAnalyzer::TokenAnalyzer(){
	global_conteiner=ErrorContainer();
	index=-1;
	linenum=1;
	charindex=1;
	num=0;
	LoadFile();
	GetChar();
}

bool TokenAnalyzer::IsEndOfFile(){
	return (index>=code.length());
}

void TokenAnalyzer::LoadFile(){
	infile.open("in.txt",ios::in);
	outfile.open("out.txt",ios::out);
	code.clear();
	char temp;
	while((temp=infile.get())!=EOF){
		code+=temp;
	}
	code+="#";
}

void TokenAnalyzer::ParsePunctuation(){
	switch(ch){
		case ',':
			id_code=COMMA_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case ';':
			id_code=SEMICOLON_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case '.':
			id_code=DOT_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case '=':
			id_code=EQUAL_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case ':':
			if(IsEndOfFile()==false){
				GetChar();
				if(ch=='='){
					id_code=SET_OPERAND;
					//cout<<"Operand : \':=\'"<<endl;
				}
				else{
					if(index>0){
						Retreat();
					}
					throw exception("UnIdentified Token!");
				}
			}
			else{
				throw exception("UnIdentified Token!");
			}
			break;
		case '+':
			id_code=PLUS_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case '-':
			id_code=MINUS_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case '*':
			id_code=MUL_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case '/':
			id_code=DIV_OPERAND;
			//cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case '<':
			if(IsEndOfFile()==false){
				GetChar();
				if(ch=='='){
					id_code=LEQUAL_OPERAND;
					//cout<<"Operand : \'<=\'"<<endl;
				}
				else if(ch=='>'){
					id_code=NEQUAL_OPERAND;
					//cout<<"Operand : \'<>\'"<<endl;
				}
				else{
					id_code=LESS_OPERAND;
					//cout<<"Operand : \'<\'"<<endl;
					if(index>0){
						Retreat();
					}
				}
			}
			else{
				id_code=LESS_OPERAND;
			}
			break;
		case '>':
			if(IsEndOfFile()==false){
				GetChar();
				if(ch=='='){
					id_code=MEQUAL_OPERAND;
				}
				else{
					id_code=MORE_OPERAND;
					if(index>0){
						Retreat();
					}
				}
			}
			else{
				id_code=MORE_OPERAND;
			}
			break;
		case '(':
			id_code=LBRACKET_OPERAND;

			break;
		case ')':
			id_code=RBRACKET_OPERAND;
			break;
		default:
			break;
	}
}


void TokenAnalyzer::GetChar(){
	if(charindex==1){
		cout<<"Line "<<linenum<<": ";
	}
	ch=code[++index];
	cout<<ch;
	charindex++;
	if(ch=='\n'){
		global_conteiner.Display();
		global_conteiner.Clear();
		charindex=1;
		linenum++;
	}
}

void TokenAnalyzer::Retreat(){
	ch=code[--index];
}

bool TokenAnalyzer::IsSpace(){
	if(ch==' '){
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
		if(IsEndOfFile()==true){
			break;
		}
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
		if(IsEndOfFile()==false){
			GetChar();
		}
	}
	else{
		if(IsEndOfFile()==false){
			GetChar();
		}
		throw exception("Unidentified Character!");
	}
	DisplayResult();
	if(IsEndOfFile()==true){
		throw exception("End Of File!");
	}
}

void TokenAnalyzer::DisplayResult(){
	cout<<"code: "<<id_code<<endl;
	cout<<"number: "<<num<<endl;
	cout<<"token: "<<token<<endl;
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
	while(IsNum()){
		sum=sum*10+(ch-'0');
		if(IsEndOfFile()==false){
			GetChar();
		}
		else{
			break;
		}
	}
	id_code=CONST_NUMBER;
	num=sum;
}

void TokenAnalyzer::ParseString(){
	int i=0;
	bool PreCheckReserved=false;
	while(IsAlpha()||(PreCheckReserved=IsNum())){
		token[i++]=ch;
		if(IsEndOfFile()==false){
			GetChar();
		}
		else{
			break;
		}
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
		case 'i':
			if(strcmp(token,"if")==0){
				return IF_RESERVED;
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