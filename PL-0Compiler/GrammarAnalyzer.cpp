#include "GrammarAnalyzer.h"

GrammarAnalyzer::GrammarAnalyzer(){
	declbegsys.insert(CONST_RESERVED);
	declbegsys.insert(VAR_RESERVED);
	declbegsys.insert(PROCEDURE_RESERVED);

	statbegsys.insert(BEGIN_RESERVED);
	statbegsys.insert(CALL_RESERVED);
	statbegsys.insert(IF_RESERVED);
	statbegsys.insert(WHILE_RESERVED);
	statbegsys.insert(REPEAT_RESERVED);

	facbegsys.insert(IDENTIFIER);
	facbegsys.insert(CONST_NUMBER);
	facbegsys.insert(LBRACKET_OPERAND);
}

GrammarAnalyzer::~GrammarAnalyzer(){

}

set<int> GrammarAnalyzer::Union(set<int> set1, set<int> set2){
	set<int> res;
	for each (int var in set1)
	{
		res.insert(var);
	}
	for each (int var in set2)
	{
		res.insert(var);
	}
	return res;
}

void GrammarAnalyzer::Procedure(){
	set<int> nxtlev;
	nxtlev = Union(nxtlev, declbegsys);
	nxtlev = Union(nxtlev, statbegsys);
	nxtlev.insert(DOT_OPERAND);

	analyzer.Run();
	generator.Add(JMP, 0, 0);
	SubProcedure(0, true, "", -1, nxtlev);
	if (analyzer.GetToken().Flag == DOT_OPERAND){
		generator.Add(OPR, 0, 0);
	}
	else{
		analyzer.Error(SHOULD_BE_DOT, true, "应该为句号");
		analyzer.Display();
		//ERROR!
	}
}

void GrammarAnalyzer::Item(int level, set<int> fsys){
	set<int> nxtlev = fsys;
	nxtlev.insert(MUL_OPERAND);
	nxtlev.insert(DIV_OPERAND);

	Factor(level, nxtlev);
	while (analyzer.GetToken().Flag == MUL_OPERAND || analyzer.GetToken().Flag == DIV_OPERAND){
		int value = analyzer.GetToken().Flag;
		switch (value){
			case MUL_OPERAND:
				value = MUL;
				break;
			case DIV_OPERAND:
				value = DIV;
				break;
		}
		analyzer.Run();
		Factor(level, nxtlev);
		generator.Add(OPR, 0, value);
	}
}

void GrammarAnalyzer::Factor(int level, set<int> fsys){
	Test(facbegsys, fsys, CANNOT_START_WITH_THIS);
	if (facbegsys.find(analyzer.GetToken().Flag) != facbegsys.end()){
		if (analyzer.GetToken().Flag == LBRACKET_OPERAND){
			set<int> nxtlev = fsys;
			nxtlev.insert(RBRACKET_OPERAND);
			analyzer.Run();
			Expression(level, nxtlev);
			if (analyzer.GetToken().Flag == RBRACKET_OPERAND){
				analyzer.Run();
			}
			else{
				analyzer.Error(MISSING_RIGHT_BRACKET, false, "漏右括号");
			}
		}
		else if (analyzer.GetToken().Flag == CONST_NUMBER){
			generator.Add(LIT, 0, analyzer.GetToken().Number);
			analyzer.Run();
		}
		else if (analyzer.GetToken().Flag == IDENTIFIER){
			int index = table.Locate(analyzer.GetToken().Value);
			if (index >= 0){
				Symbol sym = table.GetSymbol(index);
				switch (sym.kind){
				case CONSTANT:
					generator.Add(LIT, 0, sym.value);
					break;
				case VARIABLE:
					generator.Add(LOD, level - sym.level, sym.adr);
					break;
				case PROCEDURE:
					analyzer.Error(CANNOT_HAVE_PROCEDURE, false, "表达式内不能有标识符");
					break;
				}
			}
			else{
				analyzer.Error(UNKNOWN_IDENTIFIER, false, "标识符未说明");
			}
			analyzer.Run();
		}
		else{
			Test(fsys, facbegsys, CANNOT_AFTER_FACTOR);
			//补救措施
		}
	}
}

void GrammarAnalyzer::Expression(int level, set<int> fsys){
	int shift = 0;
	if (analyzer.GetToken().Flag == PLUS_OPERAND || analyzer.GetToken().Flag == MINUS_OPERAND){
		if (analyzer.GetToken().Flag == MINUS_OPERAND){
			shift = 1;
		}
		analyzer.Run();
	}
	set<int> nxtlev = fsys;
	nxtlev.insert(PLUS_OPERAND);
	nxtlev.insert(MINUS_OPERAND);
	Item(level, nxtlev);
	if (shift == 1){
		generator.Add(OPR, 0, 1);
	}
	while(analyzer.GetToken().Flag == PLUS_OPERAND || analyzer.GetToken().Flag == MINUS_OPERAND){
		int value = analyzer.GetToken().Flag;
		switch (value){
			case PLUS_OPERAND:
				value = ADD;
				break;
			case MINUS_OPERAND:
				value = SUB;
				break;
		}
		analyzer.Run();
		set<int> nxtlev = fsys;
		nxtlev.insert(PLUS_OPERAND);
		nxtlev.insert(MINUS_OPERAND);
		Item(level, nxtlev);
		generator.Add(OPR, 0, value);
	}
}

void GrammarAnalyzer::Condition(int level, set<int> fsys){
	int relation = 0;
	if (analyzer.GetToken().Flag == ODD_RESERVED){
		analyzer.Run();
		Expression(level, fsys);
		generator.Add(OPR, 0, ODD);
	}
	else{
		set<int> nxtlev = fsys;
		nxtlev.insert(EQUAL_OPERAND);
		nxtlev.insert(NEQUAL_OPERAND);
		nxtlev.insert(LESS_OPERAND);
		nxtlev.insert(MORE_OPERAND);
		nxtlev.insert(MEQUAL_OPERAND);
		nxtlev.insert(LEQUAL_OPERAND);
		Expression(level, nxtlev);
		switch (analyzer.GetToken().Flag){
			case EQUAL_OPERAND:
				relation = EQL;
				break;
			case NEQUAL_OPERAND:
				relation = NEQ;
				break;
			case LESS_OPERAND:
				relation = LSS;
				break;
			case LEQUAL_OPERAND:
				relation = LEQ;
				break;
			case MORE_OPERAND:
				relation = GTR;
				break;
			case MEQUAL_OPERAND:
				relation = GEQ;
				break;
			default:
				relation = -1;
				break;
		}
		if (relation != -1){
			analyzer.Run();
			Expression(level, fsys);
			generator.Add(OPR, 0, relation);
		}
		else{
			analyzer.Error(SHOULD_BE_RELATION, false, "应为关系运算符");
		}
	}
}

void GrammarAnalyzer::Sentence(int level, int begin, set<int> fsys){
	int index;
	int addr1;
	switch (analyzer.GetToken().Flag){
		case IDENTIFIER:
			AssignDeclaration(level ,fsys);
			break;
		case CALL_RESERVED:
			CallDeclaration(level, fsys);
			break;
		case BEGIN_RESERVED:
			BeginDeclaration(level, begin, fsys);
			break;
		case IF_RESERVED:
			IfDeclaration(level, begin, fsys);
			break;
		case READ_RESERVED:
			ReadDeclaration(level, fsys);
			break;
		case REPEAT_RESERVED:
			RepeatDeclaration(level, begin, fsys);
			break;
		case WHILE_RESERVED:
			WhileDeclaration(level, begin, fsys);
			break;
		case WRITE_RESERVED:
			WriteDeclaration(level, fsys);
			break;
		default:
			break;
	}
	
}

void GrammarAnalyzer::BeginDeclaration(int level, int begin, set<int> fsys){
	analyzer.Run();
	set<int> nxtlev = fsys;
	nxtlev.insert(SEMICOLON_OPERAND);
	nxtlev.insert(END_RESERVED);
	Sentence(level, begin, nxtlev);
	while (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
		if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
			analyzer.Run();
		}
		else{
			analyzer.Error(MISSING_SEMI_BETWEEN, false, "语句之间漏分号");
		}
		Sentence(level, begin, nxtlev);
	}
	if (analyzer.GetToken().Flag == END_RESERVED){
		analyzer.Run();
	}
	else{
		analyzer.Error(SHOULD_BE_END_OR_SEMI, false, "应为分号或end");
	}
}

void GrammarAnalyzer::CallDeclaration(int level, set<int> fsys){
	analyzer.Run();
	if (analyzer.GetToken().Flag != IDENTIFIER){
		analyzer.Error(IDENTIFIER_AFTER_CALL, false, "call后应为标识符");
	}
	else{
		int index = table.Locate(analyzer.GetToken().Value);
		if (index < 0){
			analyzer.Error(UNKNOWN_IDENTIFIER, false, "标识符未说明");
		}
		else{
			Symbol s = table.GetSymbol(index);
			if (s.kind == PROCEDURE){
				generator.Add(CAL, level - s.level, generator.Redirect(s.adr));
			}
			else{
				analyzer.Error(CANNOT_REFER, false, "不可调用常量或变量");
				//ERROR
			}
		}
		analyzer.Run();
	}
}

void GrammarAnalyzer::IfDeclaration(int level, int begin, set<int> fsys){
	analyzer.Run();
	set<int> nxtlev = fsys;
	nxtlev.insert(THEN_RESERVED);
	nxtlev.insert(DO_RESERVED);
	Condition(level, nxtlev);
	generator.Add(JPC, 0, 0);
	int index = generator.GetSize() - 1;
	if (analyzer.GetToken().Flag == THEN_RESERVED){
		analyzer.Run();
	}
	else{
		analyzer.Error(SHOULD_BE_THEN, false, "应为then");
	}
	Sentence(level, begin, fsys);
	if (analyzer.GetToken().Flag == ELSE_RESERVED){
		generator.Add(JMP, 0, 0);
		generator.AdjustJump(index, generator.GetSize());
		index = generator.GetSize() - 1;
		analyzer.Run();
		Sentence(level, begin, fsys);
	}
	generator.AdjustJump(index, generator.GetSize());
}

void GrammarAnalyzer::RepeatDeclaration(int level, int begin, set<int> fsys){
	analyzer.Run();
	set<int> nxtlev = fsys;
	nxtlev.insert(SEMICOLON_OPERAND);
	nxtlev.insert(UNTIL_RESERVED);
	int index = generator.GetSize();
	Sentence(level, begin, fsys);
	while (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
		if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
			analyzer.Run();
		}
		else{
			analyzer.Error(MISSING_SEMI_BETWEEN, false, "语句之间漏分号");
		}
	}
	Sentence(level, begin, nxtlev);
	if (analyzer.GetToken().Flag == UNTIL_RESERVED){
		analyzer.Run();
		Condition(level, fsys); 
		generator.Add(JPC, 0, index);
	}
	else{
		//ERROR!
	}
}

void GrammarAnalyzer::ReadDeclaration(int level, set<int> fsys){
	analyzer.Run();
	if (analyzer.GetToken().Flag == LBRACKET_OPERAND){
		do{
			analyzer.Run();
			if (analyzer.GetToken().Flag == IDENTIFIER){
				int index = table.Locate(analyzer.GetToken().Value);
				if (index >= 0){
					Symbol s = table.GetSymbol(index);
					if (s.kind == VARIABLE){
						generator.Add(RED, level - s.level, s.adr);
					}
					else{
						//ERROR!
					}
				}
				else{
					analyzer.Error(UNKNOWN_IDENTIFIER, false, "标识符未说明");
				}
				analyzer.Run();
			}
			else{

			}
		} while (analyzer.GetToken().Flag == COMMA_OPERAND);
	}
	else{
		analyzer.Error(SHOULD_BE_LEFT_BRACKET, false, "应为左括号");
	}
	if (analyzer.GetToken().Flag == RBRACKET_OPERAND){
		analyzer.Run();
	}
	else{
		analyzer.Error(MISSING_RIGHT_BRACKET, false, "应为右括号");
	}
}

void GrammarAnalyzer::WhileDeclaration(int level, int begin, set<int> fsys){
	analyzer.Run();
	int addr1 = generator.GetSize();
	set<int> nxtlev = fsys;
	nxtlev.insert(DO_RESERVED);
	Condition(level, nxtlev);
	generator.Add(JPC, 0, 0);
	if (analyzer.GetToken().Flag == DO_RESERVED){
		analyzer.Run();
	}
	else{
		analyzer.Error(SHOULD_BE_DO, false, "应为do");
	}
	int index = generator.GetSize() - 1;
	Sentence(level, begin, fsys);
	generator.Add(JMP, 0, addr1);
	generator.AdjustJump(index, generator.GetSize());
}

void GrammarAnalyzer::AssignDeclaration(int level, set<int> fsys){
	int index = table.Locate(analyzer.GetToken().Value);
	if (index >= 0){
		Symbol s = table.GetSymbol(index);
		if (s.kind == VARIABLE){
			analyzer.Run();
			if (analyzer.GetToken().Flag != SET_OPERAND){
				analyzer.Error(SHOULD_BE_SET, true, "应该为赋值运算符:=");
			}
			else{
				analyzer.Run();
			}
			set<int> nxtlev = fsys;
			Expression(level, nxtlev);
			generator.Add(STO, level - s.level, s.adr);
		}
		else{
			analyzer.Error(CANNOT_ASSIGN, false, "不可向常量或过程赋值");
		}
	}
	else{
		analyzer.Error(UNKNOWN_IDENTIFIER, false, "标识符未说明");
	}
}

void GrammarAnalyzer::WriteDeclaration(int level, set<int> fsys){
	analyzer.Run();
	if (analyzer.GetToken().Flag == LBRACKET_OPERAND){
		do{
			analyzer.Run();
			set<int> nxtlev = fsys;
			nxtlev.insert(RBRACKET_OPERAND);
			nxtlev.insert(COMMA_OPERAND);
			Expression(level, nxtlev);
			generator.Add(WRT, 0, 0);
		} while (analyzer.GetToken().Flag == COMMA_OPERAND);
		if (analyzer.GetToken().Flag == RBRACKET_OPERAND){
			analyzer.Run();
		}
		else{
			analyzer.Error(MISSING_RIGHT_BRACKET, false, "漏右括号");
		}
	}
	else{
		analyzer.Error(SHOULD_BE_LEFT_BRACKET, false, "应为左括号");
	}
}

int GrammarAnalyzer::SubProcedure(int level, bool isroot, char *name, int prev, set<int> fsys){
	set<int> nxtlev;
	int index = 0;
	if (level > MAXLEVEL){
		analyzer.Error(TOODEEP, false, "嵌套层次过高");
	}
	if (!isroot){
		generator.Add(JMP, 0, generator.GetSize() + 1);
		index = generator.GetSize() - 1;
	}
	int variablenum = 3;
	if (analyzer.GetToken().Flag == CONST_RESERVED){
		analyzer.Run();
		ConstDeclaration(level);
		while (analyzer.GetToken().Flag == COMMA_OPERAND){
			analyzer.Run();
			ConstDeclaration(level);
		}
		if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
			analyzer.Run();
		}
		else{
			analyzer.Error(MISSING_COMMA_OR_SEMI, false, "漏掉逗号或分号");
		}
	}
	int addr = 3;
	if (analyzer.GetToken().Flag == VAR_RESERVED){
		analyzer.Run();
		VarDeclaration(level, addr, variablenum);
		while (analyzer.GetToken().Flag == COMMA_OPERAND){
			analyzer.Run();
			VarDeclaration(level, addr, variablenum);
		}
		if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
			analyzer.Run();
		}
		else{
			analyzer.Error(MISSING_COMMA_OR_SEMI, false, "漏掉逗号或分号");
		}
	}
	int addr0 = 0;
	while (analyzer.GetToken().Flag == PROCEDURE_RESERVED){
		analyzer.Run();
		if (analyzer.GetToken().Flag == IDENTIFIER){
			Symbol s(analyzer.GetToken().Value, 0, generator.GetSize(), PROCEDURE, level);
			if (table.Check(s) == false){
				table.Add(s);
				analyzer.Run();
			}
			else{
				throw exception("nanni!");
			}
		}
		else{
			analyzer.Error(IDENTIFIER_AFTER_CVP, false, "const,var,procedure后应为标识符");
		}
		if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
			analyzer.Run();
		}
		else{
			analyzer.Error(MISSING_COMMA_OR_SEMI, false, "漏掉逗号或分号");
		}
		nxtlev = fsys;
		nxtlev.insert(SEMICOLON_OPERAND);
		addr0 = SubProcedure(level + 1, false, "", index, nxtlev);
		if (!isroot){
			generator.AdjustJump(prev, addr0 + 1);
		}
		if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
			generator.Add(OPR, 0, 0);
			analyzer.Run();
			nxtlev = statbegsys;
			nxtlev.insert(IDENTIFIER);
			nxtlev.insert(PROCEDURE_RESERVED);
			Test(nxtlev, fsys, INCORRECT_SYMBOL_AFTER_PROCEDURE);
		}
		else{
			analyzer.Error(MISSING_COMMA_OR_SEMI, false, "漏掉逗号或分号");
		}
	}

	nxtlev = statbegsys;
	nxtlev.insert(IDENTIFIER);
	Test(nxtlev, declbegsys, SHOULD_BE_SENTENCE);
			
	if (isroot){
		generator.AdjustEntry();
	}
	generator.Add(INT, 0, variablenum);
	Sentence(level, variablenum, nxtlev);
	if (prev >= 0){
		generator.AdjustJump(prev, generator.GetSize() + 1);
	}

	nxtlev.clear();
	Test(fsys, nxtlev, INCORRECT_AFTER_SENTENCE_INSIDE_PROCEDURE);
	return generator.GetSize();
}

void GrammarAnalyzer::ConstDeclaration(int level){
	if (analyzer.GetToken().Flag == IDENTIFIER){
		char name[MAXN];
		memset(name, 0, sizeof(name));
		strcpy(name, analyzer.GetToken().Value);
		analyzer.Run();
		if (analyzer.GetToken().Flag == SET_OPERAND || analyzer.GetToken().Flag == EQUAL_OPERAND){
			if (analyzer.GetToken().Flag == SET_OPERAND){
				analyzer.Error(EQUAL_NOT_SET, true, "应该是=而不是:=");
			}
			analyzer.Run();
			if (analyzer.GetToken().Flag == CONST_NUMBER){
				Symbol s(name, analyzer.GetToken().Number, 0, CONSTANT, level);
				if (table.Check(s) == false){
					table.Add(s);
					analyzer.Run();
				}
				else{
					throw exception("nanni!");
				}
			}
			else{
				analyzer.Error(NUMBER_AFTER_EQUAL, false, "=后应为数");
			}
		}
		else{
			analyzer.Error(EQUAL_AFTER_IDENTIFIER, false, "标识符后应为=");
		}
	}
	else{
		analyzer.Error(IDENTIFIER_AFTER_CVP, false, "const,var,procedure后应为标识符");
	}
}

void GrammarAnalyzer::VarDeclaration(int level, int &addr, int &variablenum){
	if (analyzer.GetToken().Flag == IDENTIFIER){
		Symbol s(analyzer.GetToken().Value, 0, 0, VARIABLE, level);
		if (table.Check(s) == false){
			s.adr = addr++;
			table.Add(s);
			variablenum++;
			analyzer.Run();
		}
		else{
			throw exception("nanni!");
		}
	}
}

void GrammarAnalyzer::Analysis(){
	//cout << "START PL/0" << endl;
	rstack.Run(generator, table);
	//cout << "END PL/0" << endl;
}

char *GetMessage(int errcode){
	char *res = new char[MAXN];
	switch (errcode){
		
	}
	return res;
}

void GrammarAnalyzer::Test(set<int>s1,set<int>s2,int errcode){
	if (s1.find(analyzer.GetToken().Flag) == s1.end()){
		analyzer.Error(errcode, false, GetMessage(errcode));
		s1 = Union(s1, s2);
		while (s1.find(analyzer.GetToken().Flag) == s1.end()){
			analyzer.Run();
		}
	}
}
