#include "PCodeGenerator.h"

void PCodeGenerator::Add(int Type, int Gap, int Value){
	codelist.push_back(PCode(Type, Gap, Value));
}

void PCodeGenerator::Show(){
	cout << setw(15) << right << "COMMAND" << setw(15) << right << "GAP" << setw(15) << right << "VALUE" << endl;
	for (list<PCode>::iterator iter = codelist.begin(); iter != codelist.end(); iter++){
		cout << setw(15) << right << iter->Type << setw(15) << right << iter->Gap << setw(15) << right << iter->Value << endl;
	}
}