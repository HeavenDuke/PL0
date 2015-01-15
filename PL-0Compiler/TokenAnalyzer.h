#include "Header.h"
#include "ErrorContainer.h"
#include "MyException.h"
#ifndef TOKENANALYZER_H
#define TOKENANALYZER_H

//���ڸ��ʷ����������﷨������������Ϣ�Ľṹ��
typedef struct{
	int Flag;
	int Number;
	char Value[MAXN];
}Atoken;

/********************************************************************
TokenAnalyzer-�ʷ�������
���ڶ�ȡԴ���򣬲���Դ�����л��һ�����ĵ��ʣ��洢�ڴʷ��������ڲ���
�������﷨��������Ҫ������Щ�������ĵ��ʣ����﷨��������������õ��ĵ��ʡ�
��Ҫ���ܣ�
��1��˳���ȡԴ����ʶ��Դ�����еĵ��ʣ����߱���
��2�����Խ��ܵ���ĸ��a-zA-Z0-9 := = > >= <= <> < + - * / \n space \t .
��3�������ֱ�begin,call,const,do,else,end,if,odd,procedure,read,repeat,then,until,var,while,write
��4����ʶ�𣺳���2������κ��ַ��������򱨴���Ϸֽ�����ɲ��ʶ��
��5�����Զ�̬�������ݣ�ͬʱ�ܹ�һ��������һ���ַ����Ӷ����IOЧ�ʡ�
��6����������洢ģ�飬�﷨��������ʱͨ�����ø�ģ����д��󴫵ݡ�
********************************************************************/

class TokenAnalyzer
{
	public:
		TokenAnalyzer& operator=(const TokenAnalyzer& analyzer);   //���ظ�ֵ�����
		TokenAnalyzer(const TokenAnalyzer& analyzer);              //�������캯��
		TokenAnalyzer();                                           //���캯��
		~TokenAnalyzer();                                          //��������

		void Run();                                                //���дʷ�����ģ�飬����һ������
		void TestRun();                                            //�����ú���������ʹ��
		void Retreat();                                            //����һλ��

		const Atoken GetToken();                                   //��ȡ�������ݣ��������﷨����ģ�鴫������
		
		bool IsEndOfFile();										   //�ж��Ƿ��������ļ�β
		
		void Error(MyException m, bool canrun);                    //�������ڴʷ�������
		void Error(int type, bool canrun, char* msg);              //���������﷨������

		void DisplayResult();									   //����������������Դ���������ģ��
		bool HasError();                                           //�ж��Ƿ������������������󣬽��������������У�P-CODE�����ᱻִ��
	private:
		ErrorContainer global_container;						   //����洢ģ��
		bool ContainError;                                         //�����ж��Ƿ���ڴ���
		int linenum;                                               //��ǰ�к�
		int charindex;                                             //��ǰ������ȡ�����ַ����

		char token[MAXN];                                          //���ڴ洢��ʶ�������֣�����ó���255
		int num;                                                   //���ڴ洢������һ�������������ǲ��ó���INT_MAX
		int id_code;                                               //���ڴ洢��ǰ�洢�ĵ��ʵ���𣬾�������Header.h�е�Լ��
		
		int index;												   //��ǰ���ַ����
		string code;                                               //���ڴ洢����Ƭ���ַ���
		char ch;                                                   //��ǰ�ַ�

		void Clear();                                              //�����һ�ֵĴʷ����������token��Ϊ�մ���num��Ϊ0��id_code��ΪĬ��ֵ
		void GetChar();											   //��ȡ��һ���ַ�
		void LoadFile();                                           //�����ļ����ض���IO���û���������뼰����ļ�
		void Error(char *);                                        //�����ñ�����

		bool IsNum();			//�жϵ�ǰ�ַ��Ƿ�Ϊ����
		bool IsAlpha();         //�жϵ�ǰ�ַ��Ƿ�Ϊ��ĸ
		bool IsPunc();          //�жϵ�ǰ�ַ��Ƿ����Ϊʶ����
		int IsReserved();       //�жϵ�ǰ��ʶ���Ƿ�Ϊ������
		bool IsSpace();         //�жϵ�ǰ�ַ��Ƿ�Ϊ�ո񣨰���space��\t��
		bool IsLineEnd();       //�жϵ�ǰ�ַ��Ƿ�Ϊ��ĩ
		
		void ParseNum();         //��������
		void ParseString();      //�����ַ�����ʶ���ʶ��
		void ParsePunctuation(); //�����ֽ��
};

#endif