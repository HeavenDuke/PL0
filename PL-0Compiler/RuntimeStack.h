#include "Header.h"
#include "SymTable.h"
#include "PCodeGenerator.h"
#ifndef RUNTIMESTACK_H
#define RUNTIMESTACK_H

//����ջ�е�һԪ�ص�����
#define DATA 1
#define RA 2
#define DL 3
#define SL 4
#define UN 5

//����ջ�е�һԪ�ص����ݽṹ
typedef struct Piece{
	int bias;
	int type;
	int value;
	Piece(int b, int t, int v){
		bias = b;
		type = t;
		value = v;
	}
	Piece(){
		bias = 0;
		type = 0;
		value = 0;
	}
};

/***************************************************************
����ʱ����ջ����ģ��
���������������ջ����
����������ʱִ�С�
���ܣ�
(1)���﷨�����в�����������������£�����ִ�д��﷨����ģ��õ���P-code
(2)ִ�й�����ά�����������������������ջʽ�洢
(3)��ִ�й��̳����쳣ʱ���׳��쳣�����������

***************************************************************/


class RuntimeStack{
	public:
		RuntimeStack();          //���캯��
		~RuntimeStack();         //��������
		void Push(Piece p);      //������ѹ������ջ
		Piece Pop();             //������ջ�е�������
		int GetSize();           //��ȡ����ջ����
		void Show();             //��ʾ��ǰ����ջ�������ڲ���
		void Run(PCodeGenerator generator, SymTable table);    //��������ʱ����ģ��
		int Base(int l, int b);                                //��ȡ��̬������һ����Ԫ�Ļ�ַ
		bool is_mul_overflow(int a,int b);                     //���˷��Ƿ����
		bool is_add_overflow(int a,int b);                     //���ӷ��Ƿ����
		bool is_sub_overflow(int a,int b);                     //�������Ƿ����
	private:
		int pc,    //ָ��ǰ����ִ�е�P-Code
			bp,    //ָ��ǰ���¼�Ļ���ַ
			sp;    //ָ��ջ��
		Piece dst[MAXHASH];  //����ջ
};

#endif