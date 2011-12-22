// @author John Bartel
// @file parser.h
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include "lexer.h"
#include "lexeme.h"
#include "types.h"
#include "evaluator.h"
using namespace std;

class Parser {
    private:
        Lexeme *pTree;
	Evaluator *en;
        Lexer *i;
        Lexeme token;
        string sixDebugFn;
        string fiveDebugFn;
        string fourDebugFn;
        string threDebugFn;
        string prevDebugFn;
        string lastDebugFn;
        string fileName;
        int lineNum;
    
    public:

        Parser(string fname, bool pp);
	Lexeme* cons(Lexeme *a, Lexeme *b);
        
        void debug(string s);
        bool check(Types t);
        Lexeme* match(Types t);
        void advance();
        bool statementPending();
        Lexeme* statement();
        
        bool exprPending();
        Lexeme* expr();
        bool primaryPending();
        Lexeme* primary();
        bool opPending();
        Lexeme* op();
        bool uniOpPending();
        void uniOp();

        bool idCallPending();
        Lexeme* idCall();
        bool optCallPending();
        Lexeme* optCall();

        bool varDefPending();
        Lexeme* varDef();

        bool functionDefPending();
        Lexeme* functionDef();
        bool functionCallPending();
        Lexeme* functionCall();
        bool fnArgPending();
        Lexeme* fnArg(); // argument values
        bool fnParamPending();
        Lexeme* fnParam();

        bool blockPending();
        Lexeme* block();
        bool conditionBlockPending();
        Lexeme* conditionBlock();
        bool loopBlockPending();
        Lexeme* loopBlock();
        bool elseBlockPending();
        Lexeme* elseBlock();

        bool listDefPending();
        Lexeme* listDef();
        bool listItemDefPending();
        Lexeme* listItemDef();
        bool listCallPending();
        Lexeme* listCall();

};


#endif
