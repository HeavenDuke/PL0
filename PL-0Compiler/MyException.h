#include "Header.h"
#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H

class MyException{
	public:
		MyException(int t,int r,int c,string msg){   //���캯��
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
		int type;                  //�������ͣ��洢������
		int row;                   //������ֵ��к�
		int column;                //������ֵ��к�
		string message;            //������ʾ��Ϣ
};

#endif