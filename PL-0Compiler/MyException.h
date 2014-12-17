#include "Header.h"
#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H

class MyException : exception{
	public:
		MyException(int t,int r,int c,string msg){
			type=t;
			row=r;
			column=c;
			msg=message;
		};
		void show(){
			switch(type){
			case TOKEN_ERROR:
				cout<<"Error";
				break;
			case TOKEN_WARNING:
				cout<<"Warning";
				break;
			}
			cout<<"( "<<row<<" , "<<column<<" >: "<<message<<endl;
		}
		int GetType(){return type;}
		int GetRow(){return row;}
		int GetColumn(){return column;}
		string what(){return message;}
	private:
		int type;
		int row;
		int column;
		string message;
};

#endif