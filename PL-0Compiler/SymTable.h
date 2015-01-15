#include "Header.h"
#ifndef SYMTABLE
#define SYMTABLE


//����kind�ֶε�ֵ
#define CONSTANT 1
#define VARIABLE 2
#define PROCEDURE 3


//������ŵ����ݽṹ
typedef struct Symbol{
	char name[LEN];  //��������
	int value;       //ֵ����
	int kind;        //��������
	int adr;         //��ַƫ������
	int level;       //�㼶����
	Symbol(const char *n, const int v, int a, int k, int l){
		strcpy(name, n);
		value = v;
		kind = k;
		adr = a;
		level = l;
	}
	Symbol(){
		memset(name, 0, sizeof(name));
		value = 0;
		adr = 0;
		kind = VARIABLE;
		level = 0;
	}
	
	bool operator==(const Symbol &item) const
	{
		if (strcmp(name, item.name) == 0 && level == item.level){
			return 1;
		}
		else{
			return 0;
		}
	}
};

/***************************************************************************************
SymTable-���ű����ģ��
��������������Ĺ����н��в��ҡ���λ����ȡ���޸ġ�����Ȳ���
��Ҫ���ܣ�
(1)�����Ƿ��Ѿ�����һ����������ʶ����ͬ�Ҵ��ڶ�Ӧ������ı�ʶ����Procedure�������ֱ�Դ���
(2)������������ű�
(3)������Ҫ��ȡ�ض�����
(4)�����ض���Ҫ�޸��ض����ŵ�����ֵ
***************************************************************************************/

class SymTable{
	public:
		SymTable();				//���캯��
		~SymTable();            //��������
		void Add(Symbol s);     //����·���
		bool Check(Symbol s);   //�������Ƿ��Ѵ���
		void Remove();          //ɾ������
		Symbol GetSymbol(int index){ return SymbolTable[index]; }   //Getter
		int Locate(const char *name,int level); //��λĳһ��
		void Display();         //��ʾ���ű�
	private:
		vector<Symbol> SymbolTable;  //���ű�ʹ��vector���д洢��������ң�
};

#endif