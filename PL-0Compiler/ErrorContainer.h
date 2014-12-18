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
		void Display(){
			int index=0;
			for(vector<MyException>::iterator iter=list.begin();iter!=list.end();iter++){
				for(int j=0;j<iter->GetColumn()-index-1;j++){
					cout<<' ';
				}
				cout<<'^'<<iter->GetType()<<endl;
			}
		}

	private:
		vector<MyException> list;
};

#endif