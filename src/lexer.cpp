// @author John Bartel
// @file lexer.cpp

#include "lexer.h"
using namespace std;

Lexer::Lexer() {
    cmtFlag = false;
}
Lexer::~Lexer() {
    in.close();
}

Lexer::Lexer(string fname) {
    lineCount = 1;
    in.open(fname.c_str(), ifstream::in);
    if (!in) { cout << "File does not exist" << endl; exit(0); }
}
Lexeme Lexer::lex() {
    next(); // Next character

    // For Lexeme that have 2 chars (&& || <= >=)
    if (skipCharFlag && ch != EOF) { 
        skipCharFlag = false;
        next();
    }
     
    tmpValue = "";
    
    // skip initial whitespace
    while (isWhiteSpace()) { next(); }

    //keywords
    if (ch == 'i' && nextChar == 'f') { 
        skipCharFlag = true;
        return Lexeme(IF);
    }
    if (ch == 'w' && nextChar == 'l') { 
        skipCharFlag = true;
        return Lexeme(WHILE);
    }
    if (ch == 'e' && nextChar == 'l') { 
        skipCharFlag = true;
        return Lexeme(ELSE);
    }
    
    // handle numbers
    if (isdigit(ch) || ch == '.') {
        tmpValue += ch;
        while (isdigit(nextChar) || nextChar == '.') { 
            next();
            tmpValue += ch;
        } 
        // detect if int or double
        int isDoubleAt = -1;
        for (int i=0, j=tmpValue.length(); i < j; i++) {
            if (tmpValue.at(i) == '.') { isDoubleAt = i; break; }
        }

        // handle integers
        if (isDoubleAt == -1) {
            return lexInteger(atoi(tmpValue.c_str()));
        }
        // handle doubles/real
        else {
            return lexDouble(strtod(tmpValue.c_str(),NULL));
        }
    }
    // handle variables
    else if (isalpha(ch) || ch == '_') {
        tmpValue += ch;
        while (isalnum(nextChar) || nextChar == '_') {
            next();
            tmpValue += ch;
        }
        return lexVariable(tmpValue);
    }
    // handle strings
    else if (ch == '"') {
        next();
        do {
            if (ch != '"') {
                tmpValue += ch;
                next();
            }
        } while (ch != '"');
        
        return lexString(tmpValue);
    }
    // handle symbols
    else {
        return lexSymbols();
    }
}



Lexeme Lexer::lexSymbols() {
    if (ch == '(') { 
        return Lexeme(OPAREN);
    }
    if (ch == ')') { 
        return Lexeme(CPAREN);
    }
    if (ch == '[') { 
        return Lexeme(OLIST);
    }
    if (ch == ']') {
        return Lexeme(CLIST);
    }
    if (ch == '+') { 
        return Lexeme(PLUS);
    }
    if (ch == '-') { 
        return Lexeme(MINUS);
    }
    if (ch == '*') { 
        return Lexeme(TIMES);
    }
    if (ch == '/') { 
        return Lexeme(DIVIDE);
    }
    if (ch == '%') { 
        return Lexeme(MOD);
    }
    if (ch == '^') { 
        return Lexeme(POW);
    }
    if (ch == '<' && nextChar != '=') {  
        return Lexeme(LT);
    }
    if (ch == '<' && nextChar == '=') {  
        skipCharFlag = true;
        return Lexeme(LTE);
    }
    if (ch == '=' && nextChar != '=') {
        return Lexeme(EQ);
    }
    if (ch == '!' && nextChar == '=') { 
        skipCharFlag = true;
        return Lexeme(NTEQ);
    }
    if (ch == '=' && nextChar == '=') { 
        skipCharFlag = true;
        return Lexeme(EQEQ);
    }
    if (ch == '>' && nextChar == '=') { 
        skipCharFlag = true;
        return Lexeme(GTE);
    }
    if (ch == '>' && nextChar != '=') { 
        return Lexeme(GT);
    }
    if (ch == '&' && nextChar == '&') { 
        skipCharFlag = true;
        return Lexeme(AND);
    }
    if (ch == '|' && nextChar != '|') { 
        return Lexeme(PIPE);
    }
    if (ch == '|' && nextChar == '|') { 
        skipCharFlag = true;
        return Lexeme(OR);
    }
    if (ch == '!') { 
        return Lexeme(NOT);
    }
    if (ch == '#') { 
        return Lexeme(VAR);
    }
    if (ch == '@') { 
        return Lexeme(FUNC);
    }
    if (ch == '?') { 
        return Lexeme(QMARK);
    }
    /*//if (ch == ) { // 
    //    return Lexeme(NIL);
    //    return le;
    //}*/
    if (ch == ',') { 
        return Lexeme(COMMA);
    }
    if (ch == ':') { 
        return Lexeme(COLON);
    }
    if (ch == ';') { 
        return Lexeme(SMCOLON);
    }
    if (ch == '\n') { 
        return Lexeme(NEWLINE);
    }
    if (ch == EOF) { 
        return Lexeme(END_OF_INPUT);
    }
    string s;
    s += ch;
    return lexUnknown(s);
}
Lexeme Lexer::lexInteger(int i) {
    return Lexeme(INT,i);
}
Lexeme Lexer::lexDouble(double d) {
    return Lexeme(REAL,d);
}
Lexeme Lexer::lexVariable(string s) {
    return Lexeme(ID,s);
}
Lexeme Lexer::lexString(string s) {
    return Lexeme(STRING, s);
}
Lexeme Lexer::lexUnknown(string s) {
    return Lexeme(UNKNOWN,s);
}

bool Lexer::isWhiteSpace() {
    // \n (newlines) have special meaning
    if (ch == '\n') {
        lineCount++;
        if (cmtFlag) {
            cmtFlag = false;
            return true;
        }
        else { return false; }
    }
    // "//" starts new comment
    if (ch == '/' && nextChar == '/') { cmtFlag = true; return true; }
    
    if (isspace(ch) || cmtFlag) {
        return true;
    }
    return false;
}
void Lexer::next() {
    ch = (char) in.get(); 
    if (!in.eof()) nextChar = (char) in.peek();
}
