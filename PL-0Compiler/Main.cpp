#include "GrammarAnalyzer.h"
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<fstream>

int main(){
	GrammarAnalyzer analyzer;
	try{
		analyzer.SubProcedure();
	}catch(exception e){
		cout<<e.what()<<endl;
	}
	return 0;
}