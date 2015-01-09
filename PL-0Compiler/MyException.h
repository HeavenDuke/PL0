#include "Header.h"
#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H

class MyException{
	public:
		MyException(int t,int r,int c,string msg){
			type=t;
			row=r;
			column=c;
			message = msg;
		};
		void show(){
			
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