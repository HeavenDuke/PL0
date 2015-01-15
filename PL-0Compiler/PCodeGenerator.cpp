#include "PCodeGenerator.h"

void PCodeGenerator::Add(int Type, int Gap, int Value){//添加新指令到容器中
	//cout << endl << setw(15) << right << Select(Type) << setw(15) << right << Gap << setw(15) << right << Value << endl;
	codelist.push_back(PCode(Type, Gap, Value));
}

void PCodeGenerator::Show(){//输出全部P-CODE指令
	int i = 0;
	cout << setw(15) << right << "INDEX" << setw(15) << right << "COMMAND" << setw(15) << right << "GAP" << setw(15) << right << "VALUE" << endl;
	for (vector<PCode>::iterator iter = codelist.begin(); iter != codelist.end(); iter++){
		cout << setw(15) << right << i++ << setw(15) << right << Select(iter->Type) << setw(15) << right << iter->Gap << setw(15) << right << iter->Value << endl;
	}
}
const char *PCodeGenerator::Select(int value){//将指令代码转换为指令名称
	switch (value){
		case LIT:
			return "LIT";
			break;
		case OPR:
			return "OPR";
			break;
		case LOD:
			return "LOD";
			break;
		case STO:
			return "STO";
			break;
		case CAL:
			return "CAL";
			break;
		case INT:
			return "INT";
			break;
		case JMP:
			return "JMP";
			break;
		case JPC:
			return "JPC";
			break;
		case RED:
			return "RED";
			break;
		case WRT:
			return "WRT";
			break;
	}
}

PCode PCodeGenerator::GetCommand(int index){//获取特定位置的指令
	return codelist[index];
}

void PCodeGenerator::AdjustJump(int index, int addr){ //重定位特定分程序
	codelist[index].Value = addr;
}

void PCodeGenerator::AdjustEntry(){   //重定位程序入口
	codelist[0].Value = codelist.size();
}

int PCodeGenerator::GetSize(){
	return codelist.size();
}

int PCodeGenerator::Redirect(int index){  //重新定位跳转位置（用于while/repeat/if结构）
	return codelist[index].Value;
}