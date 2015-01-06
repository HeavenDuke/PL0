#include "PCodeGenerator.h"

void PCodeGenerator::Add(int Type, int Gap, int Value){
	//cout << endl << setw(15) << right << Select(Type) << setw(15) << right << Gap << setw(15) << right << Value << endl;
	codelist.push_back(PCode(Type, Gap, Value));
}

void PCodeGenerator::Show(){
	int i = 0;
	cout << setw(15) << right << "INDEX" << setw(15) << right << "COMMAND" << setw(15) << right << "GAP" << setw(15) << right << "VALUE" << endl;
	for (vector<PCode>::iterator iter = codelist.begin(); iter != codelist.end(); iter++){
		cout << setw(15) << right << i++ << setw(15) << right << Select(iter->Type) << setw(15) << right << iter->Gap << setw(15) << right << iter->Value << endl;
	}
}
const char *PCodeGenerator::Select(int value){
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