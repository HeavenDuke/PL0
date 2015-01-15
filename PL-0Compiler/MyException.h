#include "Header.h"
#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H

class MyException{
	public:
		MyException(int t,int r,int c,string msg){   //构造函数
			type=t;
			row=r;
			column=c;
			message = msg;
		};

		//GETTER
		int GetType(){return type;}
		int GetRow(){return row;}
		int GetColumn(){return column;}
		string what(){return message;}
	private:
		int type;                  //错误类型，存储错误码
		int row;                   //错误出现的行号
		int column;                //错误出现的列号
		string message;            //错误提示信息
};

#endif