#include "ErrorContainer.h"

ErrorContainer::ErrorContainer(){
	Clear();
}

ErrorContainer::~ErrorContainer(){
	Clear();           //���캯��
}

void ErrorContainer::EnQueue(MyException e){
	list.push(e);      //��������
}

MyException ErrorContainer::DeQueue(){ 
	MyException e = list.front();
	list.pop();
	return e;          //���Ӳ����س���Ԫ�ء�
}

MyException ErrorContainer::GetFront(){
	return list.front(); //�ڲ����ӵ������»�ȡ����Ԫ��
}

void ErrorContainer::Clear(){    //��ն���
	while (list.empty() == false){
		list.pop();
	}
}

bool ErrorContainer::IsEmpty(){  //�ж϶����Ƿ�Ϊ��
	return list.empty();
}

void ErrorContainer::Display(){  //����Լ����ʽ���������Ϣ��ÿ��һ��������ʾλ����������λ�ö�Ӧ
	while (list.empty() == false){
		MyException e = list.front();
		cout << "****        ";
		for (int i = 0; i < e.GetColumn(); i++){
			cout << ' ';
		}
		cout << "^" << e.GetType() << endl;
		list.pop();
	}
}