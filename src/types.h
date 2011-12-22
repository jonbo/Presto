// @author John Bartel
// @file types.h
#ifndef TYPES_H
#define TYPES_H
#include <string>
using namespace std;
enum Types {
    OPAREN, CPAREN, INT, REAL, STRING, OLIST, // 0-5
    PLUS, MINUS, TIMES, DIVIDE, MOD, POW, // 6-11
    LT, LTE, EQEQ, GTE, GT, AND, OR, NOT, IF, WHILE, // 12-21
    VAR, ID, NIL, COMMA, COLON, SMCOLON, //22-27
    ELSE, NEWLINE, END_OF_INPUT, FUNC, CLIST, //28-32
    UNKNOWN, EQ, NTEQ, PIPE, QMARK, // 33-37
    FNPARAMLIST,FNARGLIST, EXPR, EXPR_NO_OP, PAREN, //38-42
    IDCALL, FNCALL, LSCALL,OPTCALL, //43-46
    STMT, E_ARGLIST, ENV, LIST, FUNCBODY, //47-51
    EXPR_PAREN, NUL, EXPR_PAREN_NO_OP // 52-54
};

/*string TypeArray[31] = {"OPAREN", "CPAREN", "INT", "REAL", "STRING", "OLIST",
    "PLUS", "MINUS", "TIMES", "DIVIDE", "MOD", "POW",
    "LT", "LTE", "EQEQ", "GTE", "GT", "AND", "OR", "NOT", "IF", "WHILE",
    "VAR", "ID", "NIL", "COMMA", "COLON", "SMCOLON",
    "TILDE", "NEWLINE", "END_OF_INPUT", "FUNC", "CLIST",
    "UNKNOWN", "EQ", "NTEQ", "PIPE", "IF", "WHILE"};*/

#endif
