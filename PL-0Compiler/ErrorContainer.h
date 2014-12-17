#include "Header.h"
#include "MyException.h"
#ifndef ERRORCONTAINER_H
#define ERRORCONTAINED_H

class ErrorContainer{
	public:
		ErrorContainer(){
			Clear();
		}
		~ErrorContainer(){
			Clear();
		}
		void Push(MyException e){
			list.push_back(e);
		}
		bool Remove(){
			if(list.empty()==false){
				list.erase(list.begin());
				return true;
			}
			return false;
		}
		void Clear(){
			list.clear();
		}

	private:
		vector<MyException> list;
};

#endif