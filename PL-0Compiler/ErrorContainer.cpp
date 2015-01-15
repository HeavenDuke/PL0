#include "ErrorContainer.h"

ErrorContainer::ErrorContainer(){
	Clear();
}

ErrorContainer::~ErrorContainer(){
	Clear();           //构造函数
}

void ErrorContainer::EnQueue(MyException e){
	list.push(e);      //析构函数
}

MyException ErrorContainer::DeQueue(){ 
	MyException e = list.front();
	list.pop();
	return e;          //出队并返回出队元素。
}

MyException ErrorContainer::GetFront(){
	return list.front(); //在不出队的条件下获取队首元素
}

void ErrorContainer::Clear(){    //清空队列
	while (list.empty() == false){
		list.pop();
	}
}

bool ErrorContainer::IsEmpty(){  //判断队列是否为空
	return list.empty();
}

void ErrorContainer::Display(){  //按照约定格式输出错误信息，每行一个错误，提示位置与错误出现位置对应
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