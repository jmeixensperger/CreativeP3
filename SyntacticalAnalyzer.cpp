/*******************************************************************************
 * Assignment: Project 3 - Syntactical Analyzer for Scheme program	        *
 * Author: Team Creative					                *
 * Date: 12-16-2017                                                             *
 * File: SyntacticalAnaylzer.cpp                                                *
 *                                                                              *
 * Description: This file contains the implementations for the Syntactical      *
 * 		Analyzer class. This includes our rule-lookup table, for       	*
 * 		deciding which rule to use depending on our current position in	*
 * 		parsing, and our non-terminal functions, which define the     	*
 * 		behavior and rules for each type of non-terminal.            	*
 *******************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

// table: each entry stores the rule to be used for the current non-terminal
// 	  (row) and current token (column). 0 values are either unreachable
// 	  or represent an error (it will throw off the rest of parsing).
static int table [][36] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,4,0},
	{0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,6,5,0,0,0},
	{0,8,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,-2,7,0,0,0},
	{0,-2,10,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-2,-2,12,0,0,0},
	{0,13,13,13,13,13,0,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,-2,13,0,0,0},
	{0,14,14,14,14,14,0,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,15,14,0,0,0},
	{0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,17,0,0,0,0},
	{0,18,18,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,19,18,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,20,-2,0,0,0,0},
	{0,22,23,23,22,22,22,22,22,22,22,22,22,0,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,21,-2,23,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,24,25,0,0,0,0},
	{0,49,0,0,29,26,27,50,51,28,30,31,32,0,33,34,35,36,37,38,43,39,40,41,42,44,45,46,47,48,0,-2,0,0,0,0},
	{0,53,54,55,56,57,0,58,59,60,61,62,63,64,65,66,67,68,69,70,75,71,72,73,74,76,77,78,79,80,52,-2,81,0,0,0}
};

/*******************************************************************************
 * Function: SyntacticalAnalyzer(char* filename)                                *
 *                                                                              *
 * Parameters: char* object                                                     *
 * Return value: none                                                           *
 * Description: This function will create a SyntacticalAnalyzer object and open *
 * 	       our .p2 output file to write to. It also initializes the         *
 * 	       parsing process by calling the Program function.                 *
 *******************************************************************************/
SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	lex = new LexicalAnalyzer (filename);
	string name = filename;
	string p2name = name.substr (0, name.length()-3) + ".p2"; 
	p2file.open (p2name.c_str());
	token = lex->GetToken();
	Program ();
}

/*******************************************************************************
 * Function: ~SyntacticalAnalyzer()                                             *
 *                                                                              *
 * Parameters: none                                                             *
 * Return value: none                                                           *
 * Description: This function will free memory used by the class object's       *
 * 	        LexicalAnalyzer instance and closes the output stream before    *
 * 	        deleting the SytacticalAnalyzer.                                *
 *******************************************************************************/
SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
	p2file.close ();
}

/*******************************************************************************
 * Function: Program()                                                          *
 *                                                                              *
 * Parameters: none                                                             *
 * Return value: an integer value, the number of errors                         *
 * Description: This fucntion handles all cases which involve the               *
 *			"program" rule of our grammar.				*
 *******************************************************************************/
int SyntacticalAnalyzer::Program ()
{
	p2file << "Entering Program function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	// token should be in firsts of Program
	// Body of function goes here.
	p2file << "Using Rule 1" << endl;
	errors += Define ();
	errors += More_Defines ();
	if (token != EOF_T)
	{
		errors++;
		lex->ReportError ("Missing end of file at end of program");
	}
	// token should be in follows of Program
	p2file << "Exiting Program function; current token is: "
		   << lex->GetTokenName(token) /*<<  ", lexeme: " << lex->GetLexeme()*/ << endl;
	return errors;
}

/*******************************************************************************
 * Function: Define()                                                           *
 *                                                                              *
 * Parameters: none                                                             *
 * Return value: an integer value, the number of errors                         *
 * Description: This fucntion handles all cases which involve the               *
 *			"define" rules of our grammar.				*
 *******************************************************************************/
int SyntacticalAnalyzer::Define()
{
	p2file << "Entering Define funcion; current token is: "
           << lex->GetTokenName(token) <<  ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	p2file << "Using Rule 2" << endl;
	if (token != LPAREN_T)
	{
		errors++;
		lex->ReportError("Missing left paren at start of define");
	}
	if ((token = lex->GetToken()) != DEFINE_T)
	{
		errors++;
		lex->ReportError("Missing define token in define");
	}
	if ((token = lex->GetToken()) != LPAREN_T)
	{
		errors++;
		lex->ReportError("Missing second left paren from define");
	}
	if ((token = lex->GetToken()) != IDENT_T)
	{
		errors++;
		lex->ReportError("Missing identifier from define");
	}
	token = lex->GetToken();
	errors += Param_List();
	if (token != RPAREN_T)
	{
		errors++;
		lex->ReportError("Missing first right paren from define");
	}
	token = lex->GetToken();
	errors += Stmt();
	errors += Stmt_List();
	if (token != RPAREN_T)
	{
		errors++;
		lex->ReportError("Missing second right paren from define");
	}
	token = lex->GetToken();
	p2file << "Exiting Define function; current token is: "
		   << lex->GetTokenName(token) /*<< ", lexeme: " << lex->GetLexeme()*/ << endl;
	return errors;
}

/*******************************************************************************
 * Function: More_Defines()                                                     *
 *                                                                              *
 * Parameters: none                                                             *
 * Return value: an integer value, the number of errors                         *
 * Description: This fucntion handles all cases which involve the               *
 *			"more defines" rules of our grammar.			*
 *******************************************************************************/
int SyntacticalAnalyzer::More_Defines()
{
	p2file << "Entering More_Defines function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	int rule = table[2][token];
	p2file << "Using Rule " << rule << endl;
	if (rule == 3)
	{
		errors += Define();
		errors += More_Defines();
		token = lex->GetToken();
	}
	p2file << "Exiting Stmt_List function; current token is: "
		   << lex->GetTokenName(token) /*<< ", lexeme: " << lex->GetLexeme()*/ << endl;
	return errors;
}

/*******************************************************************************
 * Function: Stmt_List()                                                        *
 *                                                                              *
 * Parameters: none                                                             *
 * Return value: an integer value, the number of errors                         *
 * Description: This fucntion handles all cases which involve the               *
 *			"statement list" rules of our grammar.			*
 *******************************************************************************/
int SyntacticalAnalyzer::Stmt_List()
{
	p2file << "Entering Stmt_List function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	int rule = table[3][token];
	p2file << "Using Rule " << rule << endl;
	if (rule == 5)
	{
		errors += Stmt();
		errors += Stmt_List();
	}
	p2file << "Exiting Stmt_List function; current token is: "
		   << lex->GetTokenName(token) /*<<  ", lexeme: " << lex->GetLexeme()*/ << endl;
	return errors;
}

/*******************************************************************************
 * Function: Stmt()                                                             *
 *                                                                              *
 * Parameters: none                                                             *
 * Return value: an integer value, the number of errors                         *
 * Description: This fucntion handles all cases which involve the               *
 *			"statement" rules of our grammar.			*
 *******************************************************************************/
int SyntacticalAnalyzer::Stmt()
{
	p2file << "Entering Stmt function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	int rule = table[4][token];
	p2file << "Using Rule " << rule << endl;
	switch (rule)
	{
		case 7:
			errors += Literal();
			break;
		case 8:
			if (token != IDENT_T)
			{
				errors++;
				lex->ReportError("Missing identifier in stmt");
			}
			token = lex->GetToken();
			break;
		case 9:
			if (token != LPAREN_T)
			{
				errors++;
				lex->ReportError("Missing left paren in stmt");
			}
			token = lex->GetToken();
			errors += Action();
			if (token != RPAREN_T)
			{
				errors++;
				lex->ReportError("Missing right paren in stmt");
			}
			token = lex->GetToken();
			break;
	}
	p2file << "Exiting Stmt function; current token is: "
		   << lex->GetTokenName(token) /*<<  ", lexeme: " << lex->GetLexeme()*/ << endl;
	return errors;
}

/*******************************************************************************
 * Function: Literal()                                                          *
 *                                                                              *
 * Parameters: none                                                             *
 * Return value: an integer value, the number of errors                         *
 * Description: This fucntion handles all cases which involve any               *
 *			"literal" rules of our grammar.				*
 *******************************************************************************/
int SyntacticalAnalyzer::Literal()
{
	p2file << "Entering Literal function; current token is: "
		   << lex->GetTokenName(token) /*<<  ", lexeme: " << lex->GetLexeme()*/ << endl;
	int errors = 0;
	int rule = table[5][token];
	p2file << "Using Rule " << rule << endl;
	switch (rule)
	{
		case 10:
			if (token != NUMLIT_T)
			{
				errors++;
				lex->ReportError("Missing number for literal");
			}
			token = lex->GetToken();
			break;
		case 11:
			if (token != STRLIT_T)
			{
				errors++;
				lex->ReportError("Missing string for literal");
			}
			token = lex->GetToken();
			break;
		case 12:
			if (token != QUOTE_T)
			{
				errors++;
				lex->ReportError("Missing quote for literal");
			}
			token = lex->GetToken();
			errors += Quoted_Lit();
			break;
	}
	p2file << "Exiting Literal function; current token is: "
		   << lex->GetTokenName(token) /*<<  ", lexeme: " << lex->GetLexeme()*/ << endl;
	return errors;
}

/*******************************************************************************
 * Function: Quoted_Lit()                                                       *
 *                                                                              *
 * Parameters: none                                                             *
 * Return value: an integer value, the number of errors                         *
 * Description: This fucntion handles all cases which involve the               *
 *			"quoted list" rules of our grammar.			*
 *******************************************************************************/
int SyntacticalAnalyzer::Quoted_Lit()
{
	p2file << "Entering Quoted_Lit function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	p2file << "Using Rule 13" << endl;
	errors += Any_Other_Token();
	p2file << "Exiting Quoted_Lit function; current token is: "
		   << lex->GetTokenName(token) /*<<  ", lexeme: " << lex->GetLexeme()*/ << endl;
	return errors;
}

/*******************************************************************************
 * Function: More_Tokens()                                                      *
 *                                                                              *
 * Parameters: none                                                             *
 * Return value: an integer value, the number of errors                         *
 * Description: This fucntion handles all cases which involve the               *
 *			"more tokens" rules of our grammar.			*
 *******************************************************************************/
int SyntacticalAnalyzer::More_Tokens()
{
	p2file << "Entering More_Tokens function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	int rule = table[7][token];
	p2file << "Using Rule " << rule << endl;
	if (rule == 14)
	{
		errors += Any_Other_Token();
		errors += More_Tokens();
		token = lex->GetToken();
	}
	p2file << "Exiting More_Tokens function; current token is: "
		   << lex->GetTokenName(token) /*<<  ", lexeme: " << lex->GetLexeme()*/ << endl;
	return errors;
}

/*******************************************************************************
 * Function: Param_List()                                                       *
 *                                                                              *
 * Parameters: none                                                             *
 * Return value: an integer value, the number of errors                         *
 * Description: This fucntion handles all cases which involve the               *
 *			"parameter list" rules of our grammar.			*
 *******************************************************************************/
int SyntacticalAnalyzer::Param_List()
{
	p2file << "Entering Param_List function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	int rule = table[8][token];
	p2file << "Using Rule " << rule << endl;
	if (rule == 16)
	{
		if (token != IDENT_T)
		{
			errors++;
			lex->ReportError("Missing identifier at start of \
					param_list");
		}
		token = lex->GetToken();
		errors += Param_List();
	}
	return errors;
}

/*******************************************************************************
 * Function: Else_Part()                                                        *
 *                                                                              *
 * Parameters: none                                                             *
 * Return value: an integer value, the number of errors                         *
 * Description: This fucntion handles all cases which involve the               *
 *			"else part" rules of our grammar.			*
 *******************************************************************************/
int SyntacticalAnalyzer::Else_Part()
{
	p2file << "Entering Else_Part function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	int rule = table[9][token];
	p2file << "Using Rule " << rule << endl;
	if (rule == 18)
		errors += Stmt();
	p2file << "Exiting Else_Part function; current token is: "
		   << lex->GetTokenName(token) /*<<  ", lexeme: " << lex->GetLexeme()*/ << endl;
	return errors;
}

/*******************************************************************************
 * Function: Stmt_Pair()                                                        *
 *                                                                              *
 * Parameters: none                                                             *
 * Return value: an integer value, the number of errors                         *
 * Description: This fucntion handles all cases which involve the               *
 *			"statement pair" rules of our grammar.			*
 *******************************************************************************/
int SyntacticalAnalyzer::Stmt_Pair()
{
	p2file << "Entering Stmt_Pair function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	p2file << "Using Rule 20" << endl;
	if (token != LPAREN_T)
	{
		errors++;
		lex->ReportError("Missing left paren from stmt_pair");
	}
	token = lex->GetToken();
	errors += Stmt_Pair_Body();
	if (token != RPAREN_T)
	{
		errors++;
		lex->ReportError("Missing right paren from stmt_pair");
	}
	token = lex->GetToken();
	p2file << "Exiting Stmt_Pair function; current token is: "
		   << lex->GetTokenName(token) /*<<  ", lexeme: " << lex->GetLexeme()*/ << endl;
	return errors;
}

/*******************************************************************************
 * Function: Stmt_Pair_Body()                                                   *
 *                                                                              *
 * Parameters: none                                                             *
 * Return value: an integer value, the number of errors                         *
 * Description: This fucntion handles further cases of "statment pair"		*
 *			found in rules 21, 22, and 23.				*
 *******************************************************************************/
int SyntacticalAnalyzer::Stmt_Pair_Body()
{
	p2file << "Entering Stmt_Pair_Body function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	int rule = table[11][token];
	p2file << "Using Rule " << rule << endl;
	switch (rule)
	{
		case 21:
			if (token != LPAREN_T)
			{
				errors++;
				lex->ReportError("Missing left paren for stmt_pair_\
						body");
			}
			token = lex->GetToken();
			errors += Action();
			if (token != RPAREN_T)
			{
				errors++;
				lex->ReportError("Missing right paren for stmt_pair_\
						body");
			}
			token = lex->GetToken();
			errors += Stmt();
			break;
		case 22:
			errors += Action();
			break;
		case 23:
			errors += Literal();
			break;
	}
	p2file << "Exiting Stmt_Pair_Body function; current token is: "
		/*<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme()*/ << endl;
	return errors;
}

/*******************************************************************************
 * Function: More_Pairs()                                                       *
 *                                                                              *
 * Parameters: none                                                             *
 * Return value: an integer value, the number of errors                         *
 * Description: This fucntion handles all cases which involve the               *
 *			"more pairs" rules of our grammar.			*
 *******************************************************************************/
int SyntacticalAnalyzer::More_Pairs()
{
	p2file << "Entering More_Pairs function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	int rule = table[12][token];
	p2file << "Using Rule " << rule << endl;
	if (rule == 24)
	{
		errors += Stmt_Pair();
		errors += More_Pairs();
	}
	p2file << "Exiting More_Pairs function; current token is: "
		   << lex->GetTokenName(token) /*<<  ", lexeme: " << lex->GetLexeme()*/ << endl;
	return errors;
}

/*******************************************************************************
 * Function: Action()                                                           *
 *                                                                              *
 * Parameters: none                                                             *
 * Return value: an integer value, the number of errors                         *
 * Description: This fucntion handles all cases which involve the               *
 *			"action" rules of our grammar.				*
 *******************************************************************************/
int SyntacticalAnalyzer::Action()
{
	p2file << "Entering Action function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	int rule = table[13][token];
	p2file << "Using Rule " << rule << endl;
	switch (rule)
	{
		case 26:
			if (token != IF_T)
			{
				errors++;
				lex->ReportError("Missing if token at start\
						of action");
			}
			token = lex->GetToken();
			errors += Stmt();
			errors += Stmt();
			errors += Else_Part(); 
			break;
		case 27:
			if (token != COND_T)
			{
				errors++;
				lex->ReportError("Missing cond token at start\
						of action");
			}
			token = lex->GetToken();
			errors += Stmt_Pair();
			errors += More_Pairs();
			break;
		case 28:
			if (token != LISTOP_T)
			{
				errors++;
				lex->ReportError("Missing listop at start\
						of action");
			}
			token = lex->GetToken();
			errors += Stmt();
			break;
		case 29:
			if (token != CONS_T)
			{
				errors++;
				lex->ReportError("Missing cons at start\
						of action");
			}
			token = lex->GetToken();
			errors += Stmt();
			break;
		case 30:
			if (token != AND_T)
			{
				errors++;
				lex->ReportError("Missing and at start\
						of action");
			}
			token = lex->GetToken();
			errors += Stmt_List();
			break;
		case 31:
			if (token != OR_T)
			{
				errors++;
				lex->ReportError("Missing or at start\
						of action");
			}
			token = lex->GetToken();
			errors += Stmt_List();
			break;
		case 32:
			if (token != NOT_T)
			{
				errors++;
				lex->ReportError("Missing not at start\
						of action");
			}
			token = lex->GetToken();
			errors += Stmt();
			break;
		case 33:
			if (token != NUMBERP_T)
			{
				errors++;
				lex->ReportError("Missing number predicate\
					 	at start of action");
			}
			token = lex->GetToken();
			errors += Stmt();
			break;
		case 34:
			if (token != SYMBOLP_T)
			{
				errors++;
				lex->ReportError("Missing symbol predicate\
					 	at start of action");
			}
			token = lex->GetToken();
			errors += Stmt();
			break;
		case 35:
			if (token != LISTP_T)
			{
				errors++;
				lex->ReportError("Missing list predicate\
					 	at start of action");
			}
			token = lex->GetToken();
			errors += Stmt();
			break;
		case 36:
			if (token != ZEROP_T)
			{
				errors++;
				lex->ReportError("Missing zero predicate\
					 	at start of action");
			}
			token = lex->GetToken();
			errors += Stmt();
			break;
		case 37:
			if (token != NULLP_T)
			{
				errors++;
				lex->ReportError("Missing null predicate\
					 	at start of action");
			}
			token = lex->GetToken();
			errors += Stmt();
			break;
		case 38:
			if (token != STRINGP_T)
			{
				errors++;
				lex->ReportError("Missing string predicate\
					 	at start of action");
			}
			token = lex->GetToken();
			errors += Stmt();
			break;
		case 39:
			if (token != PLUS_T)
			{
				errors++;
				lex->ReportError("Missing plus at start\
					 	of action");
			}
			token = lex->GetToken();
			errors += Stmt_List();
			break;
		case 40:
			if (token != MINUS_T)
			{
				errors++;
				lex->ReportError("Missing minus at start\
						of action");
			}
			token = lex->GetToken();
			errors += Stmt();
			errors += Stmt_List();
			break;
		case 41:
			if (token != DIV_T)
			{
				errors++;
				lex->ReportError("Missing division symbol\
						at start of action");
			}
			token = lex->GetToken();
			errors += Stmt();
			errors += Stmt_List();
			break;
		case 42:
			if (token != MULT_T)
			{
				errors++;
				lex->ReportError("Missing multiplication symbol\
						at start of action");
			}
			token = lex->GetToken();
			errors += Stmt_List();
			break;
		case 43:
			if (token != MODULO_T)
			{
				errors++;
				lex->ReportError("Missing modulo symbol\
						at start of action");
			}
			token = lex->GetToken();
			errors += Stmt();
			errors += Stmt();
			break;
		case 44:
			if (token != EQUALTO_T)
			{
				errors++;
				lex->ReportError("Missing equalto symbol\
						at start of action");
			}
			token = lex->GetToken();
			errors += Stmt_List();
			break;
		case 45:
			if (token != GT_T)
			{
				errors++;
				lex->ReportError("Missing greater than at\
						start of action");
			}
			token = lex->GetToken();
			errors += Stmt_List();
			break;
		case 46:
			if (token != LT_T)
			{
				errors++;
				lex->ReportError("Missing less than at\
						start of action");
			}
			token = lex->GetToken();
			errors += Stmt_List();
			break;
		case 47:
			if (token != GTE_T)
			{
				errors++;
				lex->ReportError("Missing greater than or\
					equals symbol at start of action");
			}
			token = lex->GetToken();
			errors += Stmt_List();
			break;
		case 48:
			if (token != LTE_T)
			{
				errors++;
				lex->ReportError("Missing less than or\
					equals symbol at start of action");
			}
			token = lex->GetToken();
			errors += Stmt_List();
			break;
		case 49:
			if (token != IDENT_T)
			{
				errors++;
				lex->ReportError("Missing identifier at start\
						of action");
			}
			token = lex->GetToken();
			errors += Stmt_List();
			break;
		case 50:
			if (token != DISPLAY_T)
			{
				errors++;
				lex->ReportError("Missing display at start\
						of action");
			}
			token = lex->GetToken();
			errors += Stmt();
			break;
		case 51:
			if (token != NEWLINE_T)
			{
				errors++;
				lex->ReportError("Missing newline in action");
			}
			token = lex->GetToken();
			break;
	}
	p2file << "Exiting Action function; current token is: "
		   << lex->GetTokenName(token) /*<<  ", lexeme: " << lex->GetLexeme()*/ << endl;
	return errors;
}

/*******************************************************************************
 * Function: Any_Other_Token()                                                  *
 *                                                                              *
 * Parameters: none                                                             *
 * Return value: an integer value, the number of errors                         *
 * Description: This fucntion handles all cases which involve the               *
 *			"any other token" rules of our grammar.			*
 *******************************************************************************/
int SyntacticalAnalyzer::Any_Other_Token()
{
	p2file << "Entering Any_Other_Token function; current token is: "
		<< lex->GetTokenName(token) << ", lexeme: " << lex->GetLexeme() << endl;
	int errors = 0;
	int rule = table[14][token];
	p2file << "Using Rule " << rule << endl;
	switch (rule)
	{
		case 52:
			if (token != LPAREN_T)
			{
				errors++;
				lex->ReportError("Missing Left Paren token at start\
						of any other token");
			}
			token = lex->GetToken();
			errors += More_Tokens();
			if (token != RPAREN_T)
			{
				errors++;
				lex->ReportError("Missing Right Paren token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 53:
			if (token != IDENT_T)
			{
				errors++;
				lex->ReportError("Missing Ident token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 54:
			if (token != NUMLIT_T)
			{
				errors++;
				lex->ReportError("Missing num lit token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 55:
			if (token != STRLIT_T)
			{
				errors++;
				lex->ReportError("Missing str lit token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 56:
			if (token != CONS_T)
			{
				errors++;
				lex->ReportError("Missing Cons token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 57:
			if (token != IF_T)
			{
				errors++;
				lex->ReportError("Missing if token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 58:
			if (token != DISPLAY_T)
			{
				errors++;
				lex->ReportError("Missing Display token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 59:
			if (token != NEWLINE_T)
			{
				errors++;
				lex->ReportError("Missing newline token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 60:
			if (token != LISTOP_T)
			{
				errors++;
				lex->ReportError("Missing list op token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 61:
			if (token != AND_T)
			{
				errors++;
				lex->ReportError("Missing and token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 62:
			if (token != OR_T)
			{
				errors++;
				lex->ReportError("Missing or token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 63:
			if (token != NOT_T)
			{
				errors++;
				lex->ReportError("Missing not token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 64:
			if (token != DEFINE_T)
			{
				errors++;
				lex->ReportError("Missing define token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 65:
			if (token != NUMBERP_T)
			{
				errors++;
				lex->ReportError("Missing number p token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 66:
			if (token != SYMBOLP_T)
			{
				errors++;
				lex->ReportError("Missing symbol p token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 67:
			if (token != LISTP_T)
			{
				errors++;
				lex->ReportError("Missing List p token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 68:
			if (token != ZEROP_T)
			{
				errors++;
				lex->ReportError("Missing zero p token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 69:
			if (token != NULLP_T)
			{
				errors++;
				lex->ReportError("Missing null p token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 70:
			if (token != STRINGP_T)
			{
				errors++;
				lex->ReportError("Missing string p token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 71:
			if (token != PLUS_T)
			{
				errors++;
				lex->ReportError("Missing plus token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 72:
			if (token != MINUS_T)
			{
				errors++;
				lex->ReportError("Missing minus token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 73:
			if (token != DIV_T)
			{
				errors++;
				lex->ReportError("Missing div token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 74:
			if (token != MULT_T)
			{
				errors++;
				lex->ReportError("Missing multi token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 75:
			if (token != MODULO_T)
			{
				errors++;
				lex->ReportError("Missing mod token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 76:
			if (token != EQUALTO_T)
			{
				errors++;
				lex->ReportError("Missing == token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 77:
			if (token != GT_T)
			{
				errors++;
				lex->ReportError("Missing > token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 78:
			if (token != LT_T)
			{
				errors++;
				lex->ReportError("Missing < token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 79:
			if (token != GTE_T)
			{
				errors++;
				lex->ReportError("Missing >= token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 80:
			if (token != LTE_T)
			{
				errors++;
				lex->ReportError("Missing <= token at start\
						of any other token");
			}
			token = lex->GetToken();
			break;
		case 81:
			if (token != QUOTE_T)
			{
				errors++;
				lex->ReportError("Missing qoute token at start\
						of any other token");
			}
			token = lex->GetToken();
			errors += Any_Other_Token();
			break;
	}
	p2file << "Exiting Any_Other_Token function; current token is: "
		   << lex->GetTokenName(token) /*<<  ", lexeme: " << lex->GetLexeme()*/ << endl;
	return errors;
}
