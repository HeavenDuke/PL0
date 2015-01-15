#include "Header.h"
#include "MyException.h"
#ifndef ERRORCONTAINER_H
#define ERRORCONTAINER_H

/***********************************************
ErrorContainer-错误容器类
用来存储编译过程中出现的错误（不包括运行时异常）
添加错误进入容器 包含在词法分析模块中，可以由词法、语法、语义分析模块调用
显示错误在语法分析模块中调用，在编译结束后输出全部错误
***********************************************/

class ErrorContainer{
	public:
		ErrorContainer();                 //构造函数
		~ErrorContainer();                //析构函数
		void EnQueue(MyException e);      //入队
		MyException DeQueue();			  //出队
		MyException GetFront();           //不出队输出队首元素
		void Clear();                     //清空容器
		bool IsEmpty();                   //判断容器是否为空
		void Display();                   //显示容器中全部错误

	private:
		queue<MyException> list;          //根据出错时加入，需要输出时按错误出现时间顺序依次输出，定义错误容器为队列的数据结构
};

#endif