#include "GrammarAnalyzer.h"
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<fstream>

int main(){
	GrammarAnalyzer analyzer;
	try{
		analyzer.SubProcedure(0, true, "", 0);
	}catch(exception e){
		cout<<e.what()<<endl;
		analyzer.Show();
	}
	return 0;
}