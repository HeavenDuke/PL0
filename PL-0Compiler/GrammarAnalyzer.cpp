#include "GrammarAnalyzer.h"

GrammarAnalyzer::GrammarAnalyzer(){
	analyzer.Run();
}

GrammarAnalyzer::~GrammarAnalyzer(){

}

void GrammarAnalyzer::Item(){
	Factor();
	for (Atoken token = analyzer.GetToken(); token.Flag == MUL_OPERAND || token.Flag == DIV_OPERAND;){
		analyzer.Run();
		Expression();
		token = analyzer.GetToken();
	}
}

void GrammarAnalyzer::Factor(){
	if (analyzer.GetToken().Flag == LBRACKET_OPERAND){
		analyzer.Run();
		Expression();
		if (analyzer.GetToken().Flag != RBRACKET_OPERAND){
			analyzer.Run();
		}
	}
	else if (analyzer.GetToken().Flag == CONST_NUMBER){
		analyzer.Run();
	}
	else if (analyzer.GetToken().Flag == IDENTIFIER){
		analyzer.Run();
	}
	else{
		//cout << 1 << endl;
		//Error!
	}
}

void GrammarAnalyzer::Expression(){
	Atoken token = analyzer.GetToken();
	if (token.Flag == PLUS_OPERAND || token.Flag == MINUS_OPERAND){
		analyzer.GetToken();
	}
	Item();
	for (Atoken token = analyzer.GetToken(); token.Flag == PLUS_OPERAND || token.Flag == MINUS_OPERAND;){
		analyzer.Run();
		Item();
		token = analyzer.GetToken();
	}
}

void GrammarAnalyzer::Condition(){
	Atoken token = analyzer.GetToken();
	if (token.Flag == ODD_RESERVED){
		analyzer.Run();
		Expression();
	}
	else{
		Expression();
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
		Expression();
	}
}

void GrammarAnalyzer::Sentence(){
	switch (analyzer.GetToken().Flag){
		case IDENTIFIER:
			analyzer.Run();
			if (analyzer.GetToken().Flag == SET_OPERAND){
				analyzer.Run();
				Expression();
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
			Sentence();
			if (analyzer.GetToken().Flag == END_RESERVED){
				analyzer.Run();
			}
			else if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
				while (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
					analyzer.Run();
					Sentence();
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
			Condition();
			if (analyzer.GetToken().Flag == THEN_RESERVED){
				analyzer.Run();
				Sentence();
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
			Condition();
			if (analyzer.GetToken().Flag == DO_RESERVED){
				analyzer.Run();
				Sentence();
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

void GrammarAnalyzer::SubProcedure(){
	if (analyzer.GetToken().Flag == CONST_RESERVED){
		analyzer.Run();
		if (analyzer.GetToken().Flag == IDENTIFIER){
			analyzer.Run();
			if (analyzer.GetToken().Flag == EQUAL_OPERAND){
				analyzer.Run();
				if (analyzer.GetToken().Flag == CONST_NUMBER){
					analyzer.Run();
					while (analyzer.GetToken().Flag == COMMA_OPERAND){
						analyzer.Run();
						if (analyzer.GetToken().Flag == IDENTIFIER){
							analyzer.Run();
							if (analyzer.GetToken().Flag == EQUAL_OPERAND){
								analyzer.Run();
								if (analyzer.GetToken().Flag == CONST_NUMBER){
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
	if (analyzer.GetToken().Flag == VAR_RESERVED){
		analyzer.Run();
		if (analyzer.GetToken().Flag == IDENTIFIER){
			analyzer.Run();
			while (analyzer.GetToken().Flag == COMMA_OPERAND){
				analyzer.Run();
				if (analyzer.GetToken().Flag == IDENTIFIER){
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
			analyzer.Run();
			if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
				analyzer.Run();
				SubProcedure();
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
	Sentence();
	//analyzer.Run();
}