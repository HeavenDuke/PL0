#include "TokenAnalyzer.h"
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<fstream>

int main(){
	TokenAnalyzer analyzer;
	try{
		analyzer.Run();
		analyzer.Run();
		analyzer.Run();
		analyzer.Run();
		analyzer.Run();
		analyzer.Run();
		analyzer.Run();
		analyzer.Run();
	}catch(exception e){
		cout<<e.what()<<endl;
	}
	return 0;
}