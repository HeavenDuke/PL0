#include "GrammarAnalyzer.h"

GrammarAnalyzer::GrammarAnalyzer(){
	analyzer.Run();
}

GrammarAnalyzer::~GrammarAnalyzer(){

}

void GrammarAnalyzer::Item(int level, int indexnumber){
	Factor(level, indexnumber);
	for (Atoken token = analyzer.GetToken(); token.Flag == MUL_OPERAND || token.Flag == DIV_OPERAND;){
		int value = token.Flag;
		switch (value){
			case MUL_OPERAND:
				value = MUL;
				break;
			case DIV_OPERAND:
				value = DIV;
				break;
		}
		analyzer.Run();
		Factor(level, indexnumber);
		generator.Add(OPR, 0, value);
		token = analyzer.GetToken();
	}
}

void GrammarAnalyzer::Factor(int level, int indexnumber){
	if (analyzer.GetToken().Flag == LBRACKET_OPERAND){
		analyzer.Run();
		Expression(level, indexnumber);
		if (analyzer.GetToken().Flag == RBRACKET_OPERAND){
			analyzer.Run();
		}
	}
	else if (analyzer.GetToken().Flag == CONST_NUMBER){
		generator.Add(LIT, 0, analyzer.GetToken().Number);
		analyzer.Run();
	}
	else if (analyzer.GetToken().Flag == IDENTIFIER){
		int index = table.Locate(analyzer.GetToken().Value, level, indexnumber);
		if (index != -1){
			Symbol sym = table.GetSymbol(index);
			switch (sym.kind){
			case CONSTANT:
				generator.Add(LIT, 0, sym.value);
				break;
			case VARIABLE:
				generator.Add(LOD, level - sym.level, sym.adr);
				break;
			}
			analyzer.Run();
		}
		else{
			//ERROR!
		}
	}
	else{
		//cout << 1 << endl;
		//Error!
	}
}

void GrammarAnalyzer::Expression(int level, int indexnumber){
	if (analyzer.GetToken().Flag == PLUS_OPERAND || analyzer.GetToken().Flag == MINUS_OPERAND){
		int value = analyzer.GetToken().Flag;
		switch (value){
			case PLUS_OPERAND:
				value = ADD;
				break;
			case MINUS_OPERAND:
				value = SUB;
				break;
		}
		generator.Add(OPR, 0, value);
		analyzer.Run();
	}
	else{
		Item(level, indexnumber);
	}
	for (Atoken token = analyzer.GetToken(); token.Flag == PLUS_OPERAND || token.Flag == MINUS_OPERAND;){
		int value = token.Flag;
		switch (value){
			case PLUS_OPERAND:
				value = ADD;
				break;
			case MINUS_OPERAND:
				value = SUB;
				break;
		}
		analyzer.Run();
		Item(level, indexnumber);
		generator.Add(OPR, 0, value);
		token = analyzer.GetToken();
	}
}

void GrammarAnalyzer::Condition(int level, int indexnumber){
	Atoken token = analyzer.GetToken();
	if (token.Flag == ODD_RESERVED){
		analyzer.Run();
		Expression(level, indexnumber);
	}
	else{
		Expression(level, indexnumber);
		Atoken token2 = analyzer.GetToken();
		switch (token2.Flag){
			case EQUAL_OPERAND:
				break;
			case NEQUAL_OPERAND:
				break;
			case LESS_OPERAND:
				break;
			case LEQUAL_OPERAND:
				break;
			case MORE_OPERAND:
				break;
			case MEQUAL_OPERAND:
				break;
			default:
				//cout << 2 << endl;
				//ERROR!
				break;
		}
		analyzer.Run();
		Expression(level, indexnumber);
	}
}

void GrammarAnalyzer::Sentence(int level, int indexnumber){
	switch (analyzer.GetToken().Flag){
		case IDENTIFIER:
			analyzer.Run();
			if (analyzer.GetToken().Flag == SET_OPERAND){
				analyzer.Run();
				Expression(level, indexnumber);
			}
			else{
				//cout << 3 << endl;
				//ERROR!
			}
			break;
		case CALL_RESERVED:
			analyzer.Run();
			if (analyzer.GetToken().Flag == IDENTIFIER){
				analyzer.Run();
			}
			else{
				//cout << 4 << endl;
				//ERROR!
			}
			break;
		case BEGIN_RESERVED:
			analyzer.Run();
			Sentence(level, indexnumber);
			if (analyzer.GetToken().Flag == END_RESERVED){
				analyzer.Run();
			}
			else if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
				while (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
					analyzer.Run();
					Sentence(level, indexnumber);
				}
				if (analyzer.GetToken().Flag == END_RESERVED){
					analyzer.Run();
				}
				else{
					//cout << 5 << endl;
					//ERROR!
				}
			}
			else{
				//cout << 6 << endl;
				//ERROR!
			}
			break;
		case IF_RESERVED:
			analyzer.Run();
			Condition(level, indexnumber);
			if (analyzer.GetToken().Flag == THEN_RESERVED){
				analyzer.Run();
				Sentence(level, indexnumber);
			}
			else{
				//cout << 7 << endl;
				//ERROR!
			}
			break;
		case READ_RESERVED:
			analyzer.Run();
			if (analyzer.GetToken().Flag == LBRACKET_OPERAND){
				analyzer.Run();
				if (analyzer.GetToken().Flag == IDENTIFIER){
					analyzer.Run();
					while (analyzer.GetToken().Flag == COMMA_OPERAND){
						analyzer.Run();
						if (analyzer.GetToken().Flag == IDENTIFIER){
							analyzer.Run();
						}
					}
					if (analyzer.GetToken().Flag == RBRACKET_OPERAND){
						analyzer.Run();
					}
					else{
						//cout << 8 << endl;
						//ERROR!
					}
				}
				else{
					//cout << 9 << endl;
					//ERROR!
				}
			}
			else{
				//cout << 10 << endl;
				//ERROR!
			}
			break;
		case WHILE_RESERVED:
			analyzer.Run();
			Condition(level, indexnumber);
			if (analyzer.GetToken().Flag == DO_RESERVED){
				analyzer.Run();
				Sentence(level, indexnumber);
			}
			else{
				//cout << 11 << endl;
				//ERROR!
			}
			break;
		case WRITE_RESERVED:
			analyzer.Run();
			if (analyzer.GetToken().Flag == LBRACKET_OPERAND){
				analyzer.Run();
				if (analyzer.GetToken().Flag == IDENTIFIER){
					analyzer.Run();
					while (analyzer.GetToken().Flag == COMMA_OPERAND){
						analyzer.Run();
						if (analyzer.GetToken().Flag == IDENTIFIER){
							analyzer.Run();
						}
					}
					if (analyzer.GetToken().Flag == RBRACKET_OPERAND){
						analyzer.Run();
					}
					else{
						//cout << 12 << endl;
						//ERROR!
					}
				}
				else{
					//cout << 13 << endl;
					//ERROR!
				}
			}
			else{
				//cout << 14 << endl;
				//ERROR!
			}
			break;
		default:
			break;
	}
	
}

void GrammarAnalyzer::SubProcedure(int level, bool isroot, char *name,int index){
	if (analyzer.GetToken().Flag == CONST_RESERVED){
		analyzer.Run();
		if (analyzer.GetToken().Flag == IDENTIFIER){
			char *sname = new char[255];
			strcpy(sname, analyzer.GetToken().Value);
			analyzer.Run();
			if (analyzer.GetToken().Flag == EQUAL_OPERAND){
				analyzer.Run();
				if (analyzer.GetToken().Flag == CONST_NUMBER){
					Symbol s(sname, analyzer.GetToken().Number, 0, 0, CONSTANT, level);
					if (table.Check(s) == false){
						table.Add(s);
					}
					else{
						throw exception("nanni!");
					}
					analyzer.Run();
					while (analyzer.GetToken().Flag == COMMA_OPERAND){
						analyzer.Run();
						if (analyzer.GetToken().Flag == IDENTIFIER){
							char *sname = new char[255];
							strcpy(sname, analyzer.GetToken().Value);
							analyzer.Run();
							if (analyzer.GetToken().Flag == EQUAL_OPERAND){
								analyzer.Run();
								if (analyzer.GetToken().Flag == CONST_NUMBER){
									Symbol s(sname, analyzer.GetToken().Number, 0, 0, CONSTANT, level);
									if (table.Check(s) == false){
										table.Add(s);
									}
									else{
										throw exception("nanni!");
									}
									analyzer.Run();
								}
								else{
									//cout << 15 << endl;
									//ERROR!
								}
							}
							else{
								//cout << 16 << endl;
								//ERROR!
							}
						}
						else{
							//cout << 17 << endl;
							//ERROR!
						}
					}
					if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
						analyzer.Run();
					}
					else{
						//cout << 18 << endl;
						//ERROR!
					}
				}
				else{
					//cout << 19 << endl;
					//ERROR!
				}
			}
			else{
				//cout << 20 << endl;
				//ERROR!
			}
		}
		else{
			//cout << 21 << endl;
			//ERROR!
		}
	}
	int addr = 3;
	if (analyzer.GetToken().Flag == VAR_RESERVED){
		analyzer.Run();
		if (analyzer.GetToken().Flag == IDENTIFIER){
			char *sname = new char[255];
			strcpy(sname, analyzer.GetToken().Value);
			Symbol s(sname, 0, 0, index, VARIABLE, level);
			if (table.Check(s) == false){
				s.adr = addr++;
				table.Add(s);
			}
			else{
				throw exception("nanni!");
			}
			analyzer.Run();
			while (analyzer.GetToken().Flag == COMMA_OPERAND){
				analyzer.Run();
				if (analyzer.GetToken().Flag == IDENTIFIER){
					char *sname = new char[255];
					strcpy(sname, analyzer.GetToken().Value);
					Symbol s(sname, 0, 0, index, VARIABLE, level);
					if (table.Check(s) == false){
						s.adr = addr++;
						table.Add(s);
					}
					else{
						throw exception("nanni!");
					}
					analyzer.Run();
				}
				else{
					//cout << 22 << endl;
					//ERROR!
				}
			}
			if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
				analyzer.Run();
			}
			else{
				//cout << 23 << endl;
				//ERROR!
			}
		}
		else{
			//cout << 24 << endl;
			//ERROR!
		}
	}
	while (analyzer.GetToken().Flag == PROCEDURE_RESERVED){
		analyzer.Run();
		if (analyzer.GetToken().Flag == IDENTIFIER){
			Procedure p(analyzer.GetToken().Value, level);
			analyzer.Run();
			if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
				analyzer.Run();
				int indexnumber = table.AddPro(p);
				SubProcedure(level + 1, false, p.name, indexnumber);
				if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
					analyzer.Run();
				}
				else{
					//cout << 25 << endl;
					//ERROR!
				}
			}
			else{
				//cout << 26 << endl;
				//ERROR!
			}
		}
		else{
			//cout << 27 << endl;
			//ERROR!
		}
	}
	Sentence(level, index);
	//analyzer.Run();
}