/*******************************************************************************
* Assignment: Project 1 - Lexical Analyzer for Scheme to C++ Translator        *
* Author: Dr. Watts                                                             *
* Date: Fall 2017                                                              *
* File: LexicalAnalyzer.h                                                      *
*                                                                              *
* Description: This file contains the                                          *
*******************************************************************************/

#ifndef SYN_H
#define SYN_H

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"
#include "CodeGenerator.h"

using namespace std;

class SyntacticalAnalyzer 
{
    public:
	SyntacticalAnalyzer (char * filename);
	~SyntacticalAnalyzer ();
    private:
	LexicalAnalyzer * lex;
	ofstream p2file;
	token_type token;
	CodeGen cg;
	int tabs;
	int Program();
	int Define();
	int More_Defines();
	int Stmt_List();
	int Stmt();
	int Literal();
	int Quoted_Lit();
	int More_Tokens();
	int Param_List();
	int Else_Part();
	int Stmt_Pair();
	int Stmt_Pair_Body();
	int More_Pairs();
	int Action();
	int Any_Other_Token();
};
	
#endif
