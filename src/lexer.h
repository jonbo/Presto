// @author John Bartel
// @file lexer.h

#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "lexeme.h"
using namespace std;

class Lexer {
    private:
        char ch;
        char nextChar;
        bool cmtFlag;
        bool skipCharFlag;
        ifstream in;
        string tmpValue;
    public:
        Lexer();
        ~Lexer();
        Lexer(string fname);
        Lexeme lex();
        
        int lineCount;
        
        Lexeme lexSymbols();
        Lexeme lexInteger(int i);
        Lexeme lexDouble(double d);
        Lexeme lexVariable(string s);
        Lexeme lexString(string s);
        Lexeme lexUnknown(string s);
        bool isWhiteSpace();
        void next();
};

#endif
