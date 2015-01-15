#include "Header.h"
#include "MyException.h"
#ifndef ERRORCONTAINER_H
#define ERRORCONTAINER_H

/***********************************************
ErrorContainer-����������
�����洢��������г��ֵĴ��󣨲���������ʱ�쳣��
��Ӵ���������� �����ڴʷ�����ģ���У������ɴʷ����﷨���������ģ�����
��ʾ�������﷨����ģ���е��ã��ڱ�����������ȫ������
***********************************************/

class ErrorContainer{
	public:
		ErrorContainer();                 //���캯��
		~ErrorContainer();                //��������
		void EnQueue(MyException e);      //���
		MyException DeQueue();			  //����
		MyException GetFront();           //�������������Ԫ��
		void Clear();                     //�������
		bool IsEmpty();                   //�ж������Ƿ�Ϊ��
		void Display();                   //��ʾ������ȫ������

	private:
		queue<MyException> list;          //���ݳ���ʱ���룬��Ҫ���ʱ���������ʱ��˳����������������������Ϊ���е����ݽṹ
};

#endif