// @author John Bartel
// @file lexeme.cpp

#include <stdlib.h>
#include "lexeme.h"
using namespace std;

Lexeme::Lexeme() {
    type = INT;
    ival = 0;
    dval = 0.0;
    left = right = 0;
}
Lexeme::Lexeme(Types t) {
    type = t;
    left = right = 0;
}
Lexeme::Lexeme(Types t, int i) {
    type = t;
    ival = i;

    dval = (double) i;
    ostringstream oss;
    oss << i;
    sval = oss.str();
    left = right = 0;
}
Lexeme::Lexeme(Types t, double d) {
    type = t;
    dval = d;

    ival = (int) d;
    ostringstream oss;
    oss << d;
    sval = oss.str();
    left = right = 0;
}
Lexeme::Lexeme(Types t, string s) {
    type = t;
    sval = s;
    left = right = 0;
}

/*Lexeme::Lexeme(const Lexeme &other) {
    Lexeme();
    operator =(other);
}*/

//const Lexeme &Lexeme::operator =(const Lexeme &other) {
    //ival = other.ival;
    //dval = other.dval;
    //type = other.type;
    //left = new Lexeme();
    //*left = *(other.left);
    //right = new Lexeme();
    //*right = *(other.right);

  //  return *this;
//}

void Lexeme::print() {
    switch (type) {
        case 0: cout << "OPAREN" << endl;
                break;
        case 1: cout << "CPAREN" << endl;
                break;
        case 2: cout << "INTEGER : " << ival << endl;
                break;
        case 3: cout << "REAL : " << dval << endl;
                break;
        case 4: cout << "STRING : " << sval << endl;
                break;
        case 5: cout << "OLIST" << endl;
                break;
        case 6: cout << "PLUS" << endl;
                break;
        case 7: cout << "MINUS" << endl;
                break;
        case 8: cout << "TIMES" << endl;
                break;
        case 9: cout << "DIVIDE" << endl;
                break;
        case 10: cout << "MOD" << endl;
                break;
        case 11: cout << "POW" << endl;
                break;
        case 12: cout << "LT" << endl;
                break;
        case 13: cout << "LTE" << endl;
                break;
        case 14: cout << "EQEQ" << endl;
                break;
        case 15: cout << "GTE" << endl;
                break;
        case 16: cout << "GT" << endl;
                break;
        case 17: cout << "AND" << endl;
                break;
        case 18: cout << "OR" << endl;
                break;
        case 19: cout << "NOT" << endl;
                break;
        case 20: cout << "IF" << endl;
                break;
        case 21: cout << "WHILE" << endl;
                break;
        case 22: cout << "VAR" << endl;
                break;
        case 23: cout << "ID : " << sval << endl;
                break;
        case 24: cout << "NIL" << endl;
                break;
        case 25: cout << "COMMA" << endl;
                break;
        case 26: cout << "COLON" << endl;
                break;
        case 27: cout << "SMCOLON" << endl;
                break;
        case 28: cout << "ELSE" << endl;
                break;
        case 29: cout << "NEWLINE" << endl;
                break;
        case 30: cout << "END_OF_INPUT" << endl;
                break;
        case 31: cout << "FUNC" << endl;
                break;
        case 32: cout << "CLIST" << endl;
                break;
        case 33: cout << "UNKNOWN : " << sval << endl;
                break;
        case 34: cout << "EQ" << endl;
                break;
        case 35: cout << "NOT EQ" << endl;
                break;
        case 36: cout << "PIPE" << endl;
                break;
        case 37: cout << "QMARK" << endl;
                break;
        default: break;
    }
}
Types Lexeme::getType() {
    return type;
}
