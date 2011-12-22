// @author John Bartel
// @file lexeme.h

#ifndef LEXEME_H
#define LEXEME_H

#include "types.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Lexeme {
    private:
    public:
        //string name;
        int ival;
        double dval;
        string sval;
        vector<Lexeme*> aval;
        Types type;
        Lexeme *left;
        Lexeme *right;
        Lexeme();
        Lexeme(Types type);
        Lexeme(Types type, int i);
        Lexeme(Types type, double d);
        Lexeme(Types type, string s);
        //Lexeme(const Lexeme &other);
        //const Lexeme &operator =(const Lexeme &other);
        void print();
        Types getType();
};

#endif
