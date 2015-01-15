#include "Header.h"

#ifndef PCODEGENERATOR_H
#define PCODEGENERATOR_H

//����ָ�����
#define LIT 1
#define OPR 2
#define LOD 3
#define STO 4
#define CAL 5
#define INT 6
#define JMP 7
#define JPC 8
#define RED 9
#define WRT 10

//�������OPRָ�a�Ĵ���
#define RETURN 0
#define NEG 1
#define ADD 2
#define SUB 3
#define MUL 4
#define DIV 5
#define ODD 6
#define MOD 7
#define EQL 8
#define NEQ 9
#define LSS 10
#define GEQ 11
#define GTR 12
#define LEQ 13

//P-CODE���ݽṹ
typedef struct PCode{
	int Type;
	int Gap;
	int Value;
	PCode(int t, int g, int v){
		Type = t;
		Gap = g;
		Value = v;
	}
	PCode(){
		Type = 0;
		Gap = 0;
		Value = 0;
	}
};

/***************************************************************************
PCodeGenerator-����������м��������ģ��
ʵ����Ϊ�������﷨����ģ���н���
��Ҫ���ܣ�
(1)�������������ʶ���﷨�ɷ֣�������Ӧ��P-Code
(2)���ִ�����ô�����ģ����д���
***************************************************************************/


class PCodeGenerator{
	public:
		PCodeGenerator(){};                     //���캯��
		~PCodeGenerator(){};                    //��������
		void Add(int type, int gap, int value); //�����ָ��
		void Show();                            //���ȫ��P-CODE
		int GetSize();                          //��ȡָ����������
		int Redirect(int index);                //�ض�λ�ֳ������
		void AdjustEntry();                     //���¶�λ�������
		void AdjustJump(int index, int addr);   //�ض�λ������while\if\repeat�ṹ��
		PCode GetCommand(int index);            //����ض�λ�õ�P-Code
	private:
		const char * Select(int value);         //��ָ����ת��Ϊָ����
		vector<PCode> codelist;					//ָ������
};

#endif