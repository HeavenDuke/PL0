#include "Header.h"
#include "TokenAnalyzer.h"
#include "ErrorContainer.h"
#include "SymTable.h"
#include "RuntimeStack.h"
#include "PCodeGenerator.h"
#ifndef GRAMMARANALYZER_H
#define GRAMMARANALYZER_H

/********************************************************************************
GrammarAnalyzer�﷨����ģ��
���ڽ��ʷ������������Ľ��������һϵ���﷨�ɷ֣���ת�����������ģ�顣
����ʹ������������Ʒ��������GrammarAnalyzer���﷨����ģ�顢����������м��������ģ�顢������ģ�顢���ű����ģ�顢������ģ��洢��ͬһ������
ͬʱ�����ܱ���ԭ�е�ģ�黯��ֲ���
��Ҫ���ܣ�
��1���﷨������ʶ��Դ�����Ƿ����PL0�ķ����﷨�淶��������ڲ����﷨�淶�Ĳ��֣�����Ҫ����
��2������������������м���룬������ڲ�������淶�Ĳ��֣�����Ҫ����
��3���������������ɴ�����Ϣ�����洢�ڰ����ڴʷ��������еĴ���洢ģ�飬�ȴ�Դ����������ʱ�����ͬʱ���ӳ���ĩβ����һ�Σ�ȷ������������С�
��4�����ű����ȷ���������ڱ�ʶ���ĺϷ��ԣ����Ϸ�ʱ��Ҫ����
��5�������������﷨����ģ����ȫ���֮�����û���κα���ʱ����������м���벢ִ�У��������ʱ����������㱨�����׳��쳣������������صĴ���ʱֱ��ֹͣ�������С�
********************************************************************************/

class GrammarAnalyzer{
	public:
		GrammarAnalyzer();    //���캯��
		~GrammarAnalyzer();   //��������
		
		void Procedure();    //�����ӳ���
		int SubProcedure(int level, bool isroot, char *name, int prev, set<int> fsys);//�ֳ����ӳ���
		void Sentence(int level, int begin, set<int> fsys);  //����ӳ���
		void Condition(int level, set<int> fsys);        //�����ӳ���
		void Expression(int level, set<int> fsys);       //���ʽ�ӳ���
		void Item(int level, set<int> fsys);             //���ӳ���
		void Factor(int level, set<int> fsys);           //�����ӳ���
		
		static char *GetMessage(int Type);               //���ݴ������ȡ������ʾ��Ϣ
		void Test(set<int>s1, set<int>s2, int Type);     //���ڲ��Լ�����
		void Analysis();                                 //�����м���벢ִ��
		void Run();                                      //�﷨���������У���ʵ��������ϵͳ������
		set<int> Union(set<int> set1, set<int> set2);    //���������ż�������ͬһ����
		void Show();                                     //���������
		
		void ConstDeclaration(int level);                              //��������
		void VarDeclaration(int level, int &addr, int &variablenum);   //��������
		void BeginDeclaration(int level, int begin, set<int> fsys);    //begin����
		void AssignDeclaration(int level, set<int> fsys);              //��ֵ����
		void WriteDeclaration(int level, set<int> fsys);               //write����
		void WhileDeclaration(int level, int begin, set<int> fsys);    //while����
		void ReadDeclaration(int level, set<int> fsys);                //read����
		void RepeatDeclaration(int level, int begin, set<int> fsys);   //repeat����
		void IfDeclaration(int level, int begin, set<int> fsys);       //if����
		void CallDeclaration(int level, set<int> fsys);                //call����
	private:

		set<int> declbegsys;      //������Ӧ�Ŀ�ʼ���ż�
		set<int> statbegsys;      //����Ӧ�Ŀ�ʼ���ż�
		set<int> facbegsys;       //���Ӷ�Ӧ�Ŀ�ʼ���ż�
		TokenAnalyzer analyzer;   //�ʷ�������
		SymTable table;           //���ű����
		PCodeGenerator generator; //��������м��������
		RuntimeStack rstack;      //����ʱ������
};

#endif