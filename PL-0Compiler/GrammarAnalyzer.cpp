#include "GrammarAnalyzer.h"

GrammarAnalyzer::GrammarAnalyzer(){
	analyzer.Run();
	generator.Add(JMP, 0, 0);
}

GrammarAnalyzer::~GrammarAnalyzer(){

}

void GrammarAnalyzer::Item(int level){
	Factor(level);
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
		Factor(level);
		generator.Add(OPR, 0, value);
		token = analyzer.GetToken();
	}
}

void GrammarAnalyzer::Factor(int level){
	if (analyzer.GetToken().Flag == LBRACKET_OPERAND){
		analyzer.Run();
		Expression(level);
		if (analyzer.GetToken().Flag == RBRACKET_OPERAND){
			analyzer.Run();
		}
	}
	else if (analyzer.GetToken().Flag == CONST_NUMBER){
		generator.Add(LIT, 0, analyzer.GetToken().Number);
		analyzer.Run();
	}
	else if (analyzer.GetToken().Flag == IDENTIFIER){
		int index = table.Locate(analyzer.GetToken().Value);
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
		Item(level);
		generator.Add(OPR, 0, value);
		token = analyzer.GetToken();
	}
}

void GrammarAnalyzer::Condition(int level){
	Atoken token = analyzer.GetToken();
	if (token.Flag == ODD_RESERVED){
		analyzer.Run();
		Expression(level);
		generator.Add(OPR, 0, ODD);
	}
	else{
		Expression(level);
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
		Expression(level);
		switch (token2.Flag){
			case EQUAL_OPERAND:
				generator.Add(OPR, 0, 8);
				break;
			case NEQUAL_OPERAND:
				generator.Add(OPR, 0, 9);
				break;
			case LESS_OPERAND:
				generator.Add(OPR, 0, 10);
				break;
			case LEQUAL_OPERAND:
				generator.Add(OPR, 0, 13);
				break;
			case MORE_OPERAND:
				generator.Add(OPR, 0, 12);
				break;
			case MEQUAL_OPERAND:
				generator.Add(OPR, 0, 11);
				break;
			default:
				//cout << 2 << endl;
				//ERROR!
				break;
		}
	}
}

void GrammarAnalyzer::Sentence(int level,int begin){
	int index;
	int addr1;
	switch (analyzer.GetToken().Flag){
		case IDENTIFIER:
			index = table.Locate(analyzer.GetToken().Value);
			analyzer.Run();
			if (analyzer.GetToken().Flag == SET_OPERAND){
				analyzer.Run();
				Expression(level);
				if (index != -1){
					Symbol s = table.GetSymbol(index);
					if (s.kind == VARIABLE){
						generator.Add(STO, level - s.level, s.adr);
					}
					else{
						//ERROR!
					}
				}
			}
			else{
				//cout << 3 << endl;
				//ERROR!
			}
			break;
		case CALL_RESERVED:
			analyzer.Run();
			if (analyzer.GetToken().Flag == IDENTIFIER){
				index = table.Locate(analyzer.GetToken().Value);
				if (index != -1){
					Symbol s = table.GetSymbol(index);
					if (s.kind == PROCEDURE){
						generator.Add(CAL, level - s.level, generator.Redirect(s.adr));
					}
					else{
						//ERROR
					}
				}
				else{
					//ERROR
				}
				analyzer.Run();
			}
			else{
				//cout << 4 << endl;
				//ERROR!
			}
			break;
		case BEGIN_RESERVED:
			analyzer.Run();
			Sentence(level, begin);
			if (analyzer.GetToken().Flag == END_RESERVED){
				analyzer.Run();
			}
			else if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
				while (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
					analyzer.Run();
					Sentence(level, begin);
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
			Condition(level);
			generator.Add(JPC, 0, 0);
			index = generator.GetSize()-1;
			if (analyzer.GetToken().Flag == THEN_RESERVED){
				analyzer.Run();
				Sentence(level, begin);
				if (analyzer.GetToken().Flag == ELSE_RESERVED){
					generator.Add(JMP, 0, 0);
					generator.AdjustJump(index, generator.GetSize());
					index = generator.GetSize() - 1;
					analyzer.Run();
					Sentence(level, begin);
					generator.AdjustJump(index, generator.GetSize());
				}
				else{
					generator.AdjustJump(index, generator.GetSize());
				}
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
					index = table.Locate(analyzer.GetToken().Value);
					if (index != -1){
						Symbol s = table.GetSymbol(index);
						if (s.kind == VARIABLE){
							generator.Add(RED, level - s.level, s.adr);
						}
						else{
							//ERROR!
						}
					}
					analyzer.Run();
					while (analyzer.GetToken().Flag == COMMA_OPERAND){
						analyzer.Run();
						if (analyzer.GetToken().Flag == IDENTIFIER){
							index = table.Locate(analyzer.GetToken().Value);
							if (index != -1){
								Symbol s = table.GetSymbol(index);
								if (s.kind == VARIABLE){
									generator.Add(RED, level - s.level, s.adr);
								}
								else{
									//ERROR!
								}
							}
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
		case REPEAT_RESERVED:
			analyzer.Run();
			index = generator.GetSize();
			Sentence(level, begin);
			while (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
				analyzer.Run();
				Sentence(level, begin);
			}
			if (analyzer.GetToken().Flag == UNTIL_RESERVED){
				analyzer.Run();
				Condition(level);
				generator.Add(JPC, 0, index);
			}
			else{
				//ERROR!
			}
			break;
		case WHILE_RESERVED:
			analyzer.Run();
			addr1 = generator.GetSize();
			Condition(level);
			if (analyzer.GetToken().Flag == DO_RESERVED){
				generator.Add(JPC, 0, 0);
				index = generator.GetSize() - 1;
				analyzer.Run();
				Sentence(level, begin);
				//cout << endl << addr1 << " " << index << endl;
				generator.Add(JMP, 0, addr1);
				generator.AdjustJump(index, generator.GetSize());
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
				Expression(level);
				generator.Add(WRT, 0, 0);
				while (analyzer.GetToken().Flag == COMMA_OPERAND){
					analyzer.Run();
					Expression(level);
					generator.Add(WRT, 0, 0);
				}
				if (analyzer.GetToken().Flag == RBRACKET_OPERAND){
					analyzer.Run();
				}
				else{
					//ERROR!
				}
			}
			else{
				//ERROR!
			}
			break;
				/*if (analyzer.GetToken().Flag == IDENTIFIER){
					index = table.Locate(analyzer.GetToken().Value);
					if (index != -1){
						Symbol s = table.GetSymbol(index);
						if (s.kind == VARIABLE || s.kind == CONSTANT){
							generator.Add(LOD, level - s.level, s.adr);
						}
						else{
							//ERROR!
						}
					}
					generator.Add(WRT, 0, 0);
					analyzer.Run();
					while (analyzer.GetToken().Flag == COMMA_OPERAND){
						analyzer.Run();
						if (analyzer.GetToken().Flag == IDENTIFIER){
							index = table.Locate(analyzer.GetToken().Value);
							if (index != -1){
								Symbol s = table.GetSymbol(index);
								if (s.kind == VARIABLE || s.kind == CONSTANT){
									generator.Add(LOD, level - s.level, s.adr);
								}
								else{
									//ERROR!
								}
							}
							generator.Add(WRT, 0, 0);
							analyzer.Run();
						}
						else if (analyzer.GetToken().Flag == CONST_NUMBER){
							generator.Add(LIT, 0, analyzer.GetToken().Number);
							generator.Add(WRT, 0, 0);
							analyzer.Run();
						}
						else{
							//ERROR
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
				else if (analyzer.GetToken().Flag == CONST_NUMBER){
					generator.Add(LIT, 0, analyzer.GetToken().Number);
					generator.Add(WRT, 0, 0);
					analyzer.Run();
					while (analyzer.GetToken().Flag == COMMA_OPERAND){
						analyzer.Run();
						if (analyzer.GetToken().Flag == IDENTIFIER){
							index = table.Locate(analyzer.GetToken().Value);
							if (index != -1){
								Symbol s = table.GetSymbol(index);
								if (s.kind == VARIABLE || s.kind == CONSTANT){
									generator.Add(LOD, level - s.level, s.adr);
								}
								else{
									//ERROR!
								}
							}
							generator.Add(WRT, 0, 0);
							analyzer.Run();
						}
						else if (analyzer.GetToken().Flag == CONST_NUMBER){
							generator.Add(LIT, 0, analyzer.GetToken().Number);
							generator.Add(WRT, 0, 0);
							analyzer.Run();
						}
						else{
							//ERROR
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
			break;*/
		default:
			break;
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
		if (analyzer.GetToken().Flag == IDENTIFIER){
			char *sname = new char[255];
			strcpy(sname, analyzer.GetToken().Value);
			analyzer.Run();
			if (analyzer.GetToken().Flag == EQUAL_OPERAND){
				analyzer.Run();
				if (analyzer.GetToken().Flag == CONST_NUMBER){
					Symbol s(sname, analyzer.GetToken().Number, 0, CONSTANT, level);
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
									Symbol s(sname, analyzer.GetToken().Number, 0, CONSTANT, level);
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
			Symbol s(sname, 0, 0, VARIABLE, level);
			if (table.Check(s) == false){
				s.adr = addr++;
				table.Add(s);
				variablenum++;
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
					Symbol s(sname, 0, 0, VARIABLE, level);
					if (table.Check(s) == false){
						s.adr = addr++;
						table.Add(s);
						variablenum++;
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
	int addr0 = 0;
	while (analyzer.GetToken().Flag == PROCEDURE_RESERVED){
		analyzer.Run();
		if (analyzer.GetToken().Flag == IDENTIFIER){
			char *sname = new char[255];
			strcpy(sname, analyzer.GetToken().Value);
			analyzer.Run();
			if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
				//generator.Add(JMP, 0, generator.GetSize() + 1);
				analyzer.Run();
				Symbol s(sname, 0, generator.GetSize(), PROCEDURE, level);
				if (table.Check(s) == false){
					table.Add(s);
				}
				else{
					throw exception("nanni!");
				}
				addr0 = SubProcedure(level + 1, false, s.name, index);
				if (!isroot){
					generator.AdjustJump(prev, addr0 + 1);
				}
				if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
					generator.Add(OPR, 0, 0);
					analyzer.Run();
				}
				else if (analyzer.GetToken().Flag == DOT_OPERAND){
					generator.Add(OPR, 0, 0);
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
	if (isroot){
		generator.AdjustEntry();
	}
	generator.Add(INT, 0, variablenum);
	Sentence(level, variablenum);
	if (prev >= 0){
		generator.AdjustJump(prev, generator.GetSize() + 1);
	}
	return generator.GetSize();
}

void GrammarAnalyzer::Analysis(){

}