#include "GrammarAnalyzer.h"

GrammarAnalyzer::GrammarAnalyzer(){
	
}

GrammarAnalyzer::~GrammarAnalyzer(){

}

void GrammarAnalyzer::Procedure(){
	analyzer.Run();
	generator.Add(JMP, 0, 0);
	SubProcedure(0, true, "", -1);
	if (analyzer.GetToken().Flag == DOT_OPERAND){
		generator.Add(OPR, 0, 0);
	}
	else{
		analyzer.Error(SHOULD_BE_DOT, true, "应该为句号");
		analyzer.Display();
		//ERROR!
	}
}

void GrammarAnalyzer::Item(int level){
	Factor(level);
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
		Factor(level);
		generator.Add(OPR, 0, value);
	}
}

void GrammarAnalyzer::Factor(int level){
	if (analyzer.GetToken().Flag == LBRACKET_OPERAND){
		analyzer.Run();
		Expression(level);
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
			//ERROR!
		}
		analyzer.Run();
	}
	else{
		//补救措施
	}
}

void GrammarAnalyzer::Expression(int level){
	int shift = 0;
	if (analyzer.GetToken().Flag == PLUS_OPERAND || analyzer.GetToken().Flag == MINUS_OPERAND){
		if (analyzer.GetToken().Flag == MINUS_OPERAND){
			shift = 1;
		}
		analyzer.Run();
	}
	Item(level);
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
		Item(level);
		generator.Add(OPR, 0, value);
	}
}

void GrammarAnalyzer::Condition(int level){
	int relation = 0;
	if (analyzer.GetToken().Flag == ODD_RESERVED){
		analyzer.Run();
		Expression(level);
		generator.Add(OPR, 0, ODD);
	}
	else{
		Expression(level);
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
			Expression(level);
			generator.Add(OPR, 0, relation);
		}
		else{
			analyzer.Error(SHOULD_BE_RELATION, false, "应为关系运算符");
		}
	}
}

void GrammarAnalyzer::Sentence(int level,int begin){
	int index;
	int addr1;
	switch (analyzer.GetToken().Flag){
		case IDENTIFIER:
			AssignDeclaration(level);
			break;
		case CALL_RESERVED:
			CallDeclaration(level);
			break;
		case BEGIN_RESERVED:
			BeginDeclaration(level, begin);
			break;
		case IF_RESERVED:
			IfDeclaration(level, begin);
			break;
		case READ_RESERVED:
			ReadDeclaration(level);
			break;
		case REPEAT_RESERVED:
			RepeatDeclaration(level, begin);
			break;
		case WHILE_RESERVED:
			WhileDeclaration(level,begin);
			break;
		case WRITE_RESERVED:
			WriteDeclaration(level);
			break;
		default:
			break;
	}
	
}

void GrammarAnalyzer::BeginDeclaration(int level,int begin){
	analyzer.Run();
	Sentence(level, begin);
	while (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
		if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
			analyzer.Run();
		}
		else{
			analyzer.Error(MISSING_SEMI_BETWEEN, false, "语句之间漏分号");
		}
		Sentence(level, begin);
	}
	if (analyzer.GetToken().Flag == END_RESERVED){
		analyzer.Run();
	}
	else{
		analyzer.Error(SHOULD_BE_END_OR_SEMI, false, "应为分号或end");
	}
}

void GrammarAnalyzer::CallDeclaration(int level){
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

void GrammarAnalyzer::IfDeclaration(int level,int begin){
	analyzer.Run();
	Condition(level);
	generator.Add(JPC, 0, 0);
	int index = generator.GetSize() - 1;
	if (analyzer.GetToken().Flag == THEN_RESERVED){
		analyzer.Run();
	}
	else{
		analyzer.Error(SHOULD_BE_THEN, false, "应为then");
	}
	Sentence(level, begin);
	if (analyzer.GetToken().Flag == ELSE_RESERVED){
		generator.Add(JMP, 0, 0);
		generator.AdjustJump(index, generator.GetSize());
		index = generator.GetSize() - 1;
		analyzer.Run();
		Sentence(level, begin);
	}
	generator.AdjustJump(index, generator.GetSize());
}

void GrammarAnalyzer::RepeatDeclaration(int level,int begin){
	analyzer.Run();
	int index = generator.GetSize();
	Sentence(level, begin);
	while (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
		if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
			analyzer.Run();
		}
		else{
			analyzer.Error(MISSING_SEMI_BETWEEN, false, "语句之间漏分号");
		}
	}
	Sentence(level, begin);
	if (analyzer.GetToken().Flag == UNTIL_RESERVED){
		analyzer.Run();
		Condition(level);
		generator.Add(JPC, 0, index);
	}
	else{
		//ERROR!
	}
}

void GrammarAnalyzer::ReadDeclaration(int level){
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

void GrammarAnalyzer::WhileDeclaration(int level,int begin){
	analyzer.Run();
	int addr1 = generator.GetSize();
	Condition(level);
	generator.Add(JPC, 0, 0);
	if (analyzer.GetToken().Flag == DO_RESERVED){
		analyzer.Run();
	}
	else{
		analyzer.Error(SHOULD_BE_DO, false, "应为do");
	}
	int index = generator.GetSize() - 1;
	Sentence(level, begin);
	generator.Add(JMP, 0, addr1);
	generator.AdjustJump(index, generator.GetSize());
}

void GrammarAnalyzer::AssignDeclaration(int level){
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
			Expression(level);
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

void GrammarAnalyzer::WriteDeclaration(int level){
	analyzer.Run();
	if (analyzer.GetToken().Flag == LBRACKET_OPERAND){
		do{
			analyzer.Run();
			Expression(level);
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

int GrammarAnalyzer::SubProcedure(int level, bool isroot, char *name,int prev){
	int index = 0;
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
			char *sname = new char[255];
			strcpy(sname, analyzer.GetToken().Value);
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
		addr0 = SubProcedure(level + 1, false, "", index);
		if (!isroot){
			generator.AdjustJump(prev, addr0 + 1);
		}
		if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
			generator.Add(OPR, 0, 0);
			analyzer.Run();
		}
		else{
			//cout << 25 << endl;
			//ERROR!
		}
	}
			
	if (isroot){
		generator.AdjustEntry();
	}
	generator.Add(INT, 0, variablenum);
	Sentence(level, variablenum);
	if (prev >= 0){
		generator.AdjustJump(prev, generator.GetSize() + 1);
	}
	Test(set<int>(), set<int>(), SUBPROCEDURE);
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
void GrammarAnalyzer::Test(set<int>s1,set<int>s2,int Type){
	switch (Type){
		case SUBPROCEDURE:
			
			break;
		case SENTENCE:
			if (strcmp(analyzer.GetToken().Value, ".") == 0){

			}
			else if (strcmp(analyzer.GetToken().Value, ";") == 0){

			}
			else if (strcmp(analyzer.GetToken().Value, "end") == 0){

			}
			else if (strcmp(analyzer.GetToken().Value, "else") == 0){

			}
			else{

			}
			break;
		case CONDITION:
			if (strcmp(analyzer.GetToken().Value, "then") == 0){

			}
			else if (strcmp(analyzer.GetToken().Value, "do") == 0){

			}
			else{

			}
			break;
		case EXPRESSION:
			if (strcmp(analyzer.GetToken().Value, "") == 0){

			}
			else if (strcmp(analyzer.GetToken().Value, ".") == 0){

			}
			else{

			}
			break;
		case FACTOR:
			if (strcmp(analyzer.GetToken().Value, ".") == 0){

			}
			else if (strcmp(analyzer.GetToken().Value, ".") == 0){

			}
			else{

			}
			break;
		case ITEM:
			if (strcmp(analyzer.GetToken().Value, ".") == 0){

			}
			else if (strcmp(analyzer.GetToken().Value, ".") == 0){

			}
			else{

			}
			break;
	}
}
