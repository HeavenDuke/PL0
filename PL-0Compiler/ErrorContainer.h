#include "Header.h"
#include "MyException.h"
#ifndef ERRORCONTAINER_H
#define ERRORCONTAINER_H

class ErrorContainer{
	public:
		ErrorContainer(){
			Clear();
		}
		~ErrorContainer(){
			Clear();
		}
		void EnQueue(MyException e){
			list.push(e);
		}
		MyException DeQueue(){
			MyException e = list.front();
			list.pop();
			return e;
		}
		MyException GetFront(){
			return list.front();
		}
		void Clear(){
			while (list.empty() == false){
				list.pop();
			}
		}
		bool IsEmpty(){
			return list.empty();
		}
		void Display(){
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

	private:
		queue<MyException> list;
};

#endif