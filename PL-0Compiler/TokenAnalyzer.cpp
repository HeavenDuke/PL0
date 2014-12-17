#include "TokenAnalyzer.h"

TokenAnalyzer::TokenAnalyzer(){
	index=-1;
	linenum=1;
	charindex=1;
	num=0;
	LoadFile();
	GetChar();
}

bool TokenAnalyzer::IsEndOfFile(){
	return (index>=code.length()-1&&code[index]=='#');
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

bool TokenAnalyzer::IsReserved(){
	return false;
}

void TokenAnalyzer::ParsePunctuation(){
	switch(ch){
		case ',':
		case ';':
		case '.':
		case '=':
			cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case ':':
			if(IsEndOfFile()==false){
				GetChar();
				if(ch=='='){
					cout<<"Operand : \':=\'"<<endl;
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
		case '-':
		case '*':
		case '/':
			cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		case '<':
			if(IsEndOfFile()==false){
				GetChar();
				if(ch=='='){
					cout<<"Operand : \'<=\'"<<endl;
				}
				else if(ch=='>'){
					cout<<"Operand : \'<>\'"<<endl;
				}
				else{
					cout<<"Operand : \'<\'"<<endl;
					if(index>0){
						Retreat();
					}
				}
			}
			else{
				cout<<"Operand : \'<\'"<<endl;
			}
			break;
		case '>':
			if(IsEndOfFile()==false){
				GetChar();
				if(ch=='='){
					cout<<"Operand : \'>=\'"<<endl;
				}
				else{
					cout<<"Operand : \'>\'"<<endl;
					if(index>0){
						Retreat();
					}
				}
			}
			else{
				cout<<"Operand : \'>\'"<<endl;
			}
			break;
		case '(':
		case ')':
			cout<<"Operand : \'"<<ch<<"\'"<<endl;
			break;
		default:
			break;
	}
}


void TokenAnalyzer::GetChar(){
	if(charindex==1){
		outfile<<"Line "<<linenum<<": ";
	}
	ch=code[++index];
	charindex++;
	if(ch=='\n'){
		charindex=1;
		linenum++;
	}
	outfile<<ch;
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
		if(IsEndOfFile()==false){
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
		if(IsReserved()){

		}
		else{
			
		}
	}
	else{

	}
}

TokenAnalyzer::~TokenAnalyzer(){

}