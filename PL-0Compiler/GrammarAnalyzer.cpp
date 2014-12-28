#include "GrammarAnalyzer.h"

GrammarAnalyzer::GrammarAnalyzer(){
	analyzer=TokenAnalyzer();
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
	const Atoken token = analyzer.GetToken();
	if (token.Flag == LBRACKET_OPERAND){
		analyzer.Run();
		Expression();
		const Atoken token2 = analyzer.GetToken();
		if (token2.Flag != RBRACKET_OPERAND){
			
		}
	}
	else if (token.Flag == CONST_NUMBER){

	}
	else if (token.Flag == IDENTIFIER){

	}
	else{

	}
	analyzer.Run();
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
				//ERROR!
				break;
		}
		analyzer.Run();
		Expression();
	}
}

void GrammarAnalyzer::Sentence(){
	Atoken token = analyzer.GetToken();
	switch (token.Flag){
		case IDENTIFIER:
			analyzer.Run();
			if (analyzer.GetToken().Flag == SET_OPERAND){
				Expression();
			}
			else{
				//ERROR!
			}
		case CALL_RESERVED:
			analyzer.Run();
			if (analyzer.GetToken().Flag == IDENTIFIER){

			}
			else{
				//ERROR!
			}
			analyzer.Run();
		case BEGIN_RESERVED:
			analyzer.Run();
			Sentence();
			while (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
				analyzer.Run();
				Sentence();
			}
			if (analyzer.GetToken().Flag!=END_RESERVED){
				//ERROR!
			}
			analyzer.Run();
			break;
		case IF_RESERVED:
			analyzer.Run();
			Condition();
			if (analyzer.GetToken().Flag == THEN_RESERVED){
				analyzer.Run();
				Sentence();
			}
			else{
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
						//ERROR!
					}
				}
				else{
					//ERROR!
				}
			}
			else{
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
						//ERROR!
					}
				}
				else{
					//ERROR!
				}
			}
			else{
				//ERROR!
			}
			break;
		default:
			break;
	}
	
}

void GrammarAnalyzer::SubProcedure(){
	Atoken token = analyzer.GetToken();
	switch (token.Flag){
		case CONST_RESERVED:
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
										//ERROR!
									}
								}
								else{
									//ERROR!
								}
							}
							else{
								//ERROR!
							}
						}
						if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
							analyzer.Run();
						}
						else{
							//ERROR!
						}
					}
					else{
						//ERROR!
					}
				}
				else{
					//ERROR!
				}
			}
			else{
				//ERROR!
			}
			break;
		case VAR_RESERVED:
			analyzer.Run();
			if (analyzer.GetToken().Flag == IDENTIFIER){
				analyzer.Run();
				while (analyzer.GetToken().Flag == COMMA_OPERAND){
					analyzer.Run();
					if (analyzer.GetToken().Flag == IDENTIFIER){
						analyzer.Run();
					}
					else{
						//ERROR!
					}
				}
				if (analyzer.GetToken().Flag == SEMICOLON_OPERAND){
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
		case PROCEDURE_RESERVED:
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
						//ERROR!
					}
				}
				else{
					//ERROR!
				}
			}
			else{
				//ERROR!
			}
			break;
	}
	Sentence();
	analyzer.Run();
}