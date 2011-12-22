// @author John Bartel
// @file evaluator.h
#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/timeb.h>
#include <math.h>
#include "lexer.h"
#include "lexeme.h"
#include "types.h"
using namespace std;

class Evaluator {
 private:
       	Lexeme *environment;
        int lineNum;
        int indent; 
        Lexeme *last;
 public: 
	Evaluator();
	Lexeme* cons(Lexeme *a, Lexeme *b);
        Lexeme* newEnvironment();
        Lexeme* populate(Lexeme *var, Lexeme *value, Lexeme *env);
        Lexeme* lookup(Lexeme *var, Lexeme *env);
        Lexeme* extend(Lexeme *vars, Lexeme *vals, Lexeme *env);
        Lexeme* update(Lexeme *var, Lexeme *val, Lexeme *env);
        void display(Lexeme *env);
        bool eqVar(Lexeme *a, Lexeme *b);

        Lexeme* eval(Lexeme *tree, Lexeme *env);
        void pp(Lexeme *tree);
        void ppSTMT(Lexeme *tree);
        void ppFUNC(Lexeme *tree);
        void ppIDCALL(Lexeme *tree);
        void ppIndent(int count);

        Lexeme* evalSTMT(Lexeme *tree, Lexeme *env);
        Lexeme* evalEXPR(Lexeme *tree, Lexeme *env);
        Lexeme* evalIDCALL(Lexeme *tree, Lexeme *env);
        Lexeme* evalFUNCCALL(Lexeme *tree, Lexeme *env);
        Lexeme* evalLISTCALL(Lexeme *tree, Lexeme *env);
        void printPA(Lexeme *p, Lexeme *a);
        void printPAT(Lexeme *p, Lexeme *a);
        Lexeme* evalList(Lexeme *tree, Lexeme *env);
        void printList(Lexeme *list);
        void print(Lexeme *a);
        void oprint(Lexeme *a);
        Lexeme* evalVarDef(Lexeme *tree, Lexeme *env);
        Lexeme* evalFuncDef(Lexeme *tree, Lexeme *env);
        Lexeme* evalLIST(Lexeme *tree, Lexeme *env);

        Lexeme* evalPLUS(Lexeme *tree, Lexeme *env);
        Lexeme* evalMINUS(Lexeme *tree, Lexeme *env);
        Lexeme* evalTIMES(Lexeme *tree, Lexeme *env);
        Lexeme* evalDIVIDE(Lexeme *tree, Lexeme *env);
        Lexeme* evalMOD(Lexeme *tree, Lexeme *env);
        Lexeme* evalPOW(Lexeme *tree, Lexeme *env);
        Lexeme* evalLT(Lexeme *tree, Lexeme *env);
        Lexeme* evalLTE(Lexeme *tree, Lexeme *env);
        Lexeme* evalEQEQ(Lexeme *tree, Lexeme *env);
        bool listEqv(Lexeme *a, Lexeme *b);
        Lexeme* evalNTEQ(Lexeme *tree, Lexeme *env);
        Lexeme* evalGTE(Lexeme *tree, Lexeme *env);
        Lexeme* evalGT(Lexeme *tree, Lexeme *env);
        Lexeme* evalAND(Lexeme *tree, Lexeme *env);
        Lexeme* evalOR(Lexeme *tree, Lexeme *env);
        Lexeme* evalASSIGN(Lexeme *tree, Lexeme *env);
        Lexeme* evalNOT(Lexeme *tree, Lexeme *env);

        Lexeme* evalIF(Lexeme *tree, Lexeme *env);
        Lexeme* evalWHILE(Lexeme *tree, Lexeme *env);
        Lexeme* evalELSE(Lexeme *tree, Lexeme *env);
        bool isTrue(Lexeme *a);

};

#endif
