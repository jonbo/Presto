// @author John Bartel
// @file parser.cpp
#include "parser.h"
using namespace std;


Parser::Parser(string fname, bool ppt) {
    //Lexeme *t = new Lexeme(ID,"this");
    //populate(t,environment,environment);
    fileName = fname;
    i = new Lexer(fileName);
    token = i->lex();
    
    lineNum = 1;
    pTree = statement();
    
    match(END_OF_INPUT);
    en = new Evaluator();
    if (!ppt) {
        en->eval(pTree,en->newEnvironment());
    }
    else {
        en->pp(pTree);
    }

}
Lexeme* Parser::cons(Lexeme *a, Lexeme *b) {
    Lexeme *n = new Lexeme();
    n->left = a;
    n->right = b;
    return n;
}
// Written one tiring night when things didn't work :|
void Parser::debug(string s) {
    sixDebugFn = fiveDebugFn;
    fiveDebugFn = fourDebugFn;
    fourDebugFn = threDebugFn;
    threDebugFn = prevDebugFn;
    prevDebugFn = lastDebugFn;
    lastDebugFn = s;
}

bool Parser::check(Types t) {
    return token.getType() == t; 
}

Lexeme* Parser::match(Types t) {
    if (!check(t)) { 
        cout << "illegal syntax error on line " << i->lineCount << ": unexpected "; token.print(); cout << "expected type " << t << " (debug:" << lastDebugFn << " " << prevDebugFn << " " << threDebugFn << " " << fourDebugFn << " " << fiveDebugFn << " " << sixDebugFn << ")\n"; 
        exit(1); 
    }
    Lexeme *n = NULL;
    if (t == INT) {
        n = new Lexeme(INT,token.ival);
    }
    else if (t == REAL) {
        n = new Lexeme(REAL,token.dval);
    }
    else if (t == STRING) {
        n = new Lexeme(STRING,token.sval);
    }
    else if (t == ID) {
        n = new Lexeme(ID,token.sval);
    }
    else if (t == UNKNOWN) {
        n = new Lexeme(UNKNOWN,token.sval);
    }
    else {
        n = new Lexeme(t);
    }
    advance();
    return n; 
}
void Parser::advance() {
    token = i->lex();
}

bool Parser::statementPending() {
    return exprPending();
}
Lexeme* Parser::statement() {
    debug("statement");
    Lexeme *exp = expr();
    Lexeme *ns = NULL;
    if (check(NEWLINE)) { 
        match(NEWLINE); 
        if (statementPending()) { ns = statement(); }
    }
    if (check(PIPE)) { 
        match(PIPE); 
        if (statementPending()) { ns = statement(); }
    }
    //if (check(END_OF_INPUT)) { return match(END_OF_INPUT); }
    Lexeme *s = cons(exp,ns);
    s->type = STMT; 
    return s;
}

bool Parser::exprPending() {
    return primaryPending();
}
Lexeme* Parser::expr() {
    debug("expr");
    Lexeme *p = primary();
    Lexeme *o = NULL,*ex = NULL;
    Lexeme *e=NULL; bool t=false;
    if (opPending()) { 
        o = op(); ex = expr(); t = true;
        e = cons(p,cons(o,ex));
    }
    else e = cons(p,cons(NULL,NULL));
    if (t) e->type = EXPR;
    else e->type = EXPR_NO_OP;
    return e;
}
bool Parser::primaryPending() { // NF
    return (check(NEWLINE) || check(INT) || check(REAL) || check(STRING) || functionDefPending() || varDefPending() || idCallPending() || listDefPending() || check(OPAREN) || blockPending() || conditionBlockPending() || loopBlockPending() || elseBlockPending());
}
Lexeme* Parser::primary() { // NF
    debug("primary");
    Lexeme *v = NULL;
    if (check(NEWLINE)) { 
        match(NEWLINE); 
        if (primaryPending()) { v = primary(); }
        else { v = new Lexeme(NUL); }
    }
    else if (check(INT)) { v = match(INT); }
    else if (check(REAL)) { v = match(REAL); }
    else if (check(STRING)) { v = match(STRING); }
    else if (check(NOT)) { 
        v = match(NOT);
        v->left = primary();
    }
    else if (functionDefPending()) { v = functionDef(); }
    else if (varDefPending()) { v = varDef(); }
    else if (idCallPending()) { v = idCall(); }
    else if (listDefPending()) { v = listDef(); }
    else if (check(OPAREN)) { 
        match(OPAREN);
        v = expr();
        if (check(NEWLINE)) { match(NEWLINE); }
        match(CPAREN);
        if (v->type == EXPR) 
            v->type = EXPR_PAREN;
        if (v->type == EXPR_NO_OP) 
            v->type = EXPR_PAREN_NO_OP;
    }
    else if (blockPending()) { v = block(); }
    else if (conditionBlockPending()) { v = conditionBlock(); }
    else if (loopBlockPending()) { v = loopBlock(); }
    else { v = elseBlock(); }
    return v;
}
bool Parser::opPending() {
    return check(PLUS) || check(MINUS) || check(TIMES) || check(EQ) || check(DIVIDE) || check(MOD) || check(POW) || check(LT) || check(LTE) || check(EQEQ) || check(NTEQ) || check(GTE) || check(GT) || check(AND) || check(OR); 
}
Lexeme* Parser::op() {
    debug("op");
    Lexeme *o = NULL;
    if (check(PLUS)) { o = match(PLUS); }
    else if (check(MINUS)) { o = match(MINUS); }
    else if (check(TIMES)) { o = match(TIMES); }
    else if (check(EQ)) { o = match(EQ); } // Assignment
    else if (check(DIVIDE)) { o = match(DIVIDE); }
    else if (check(MOD)) { o = match(MOD); }
    else if (check(POW)) { o =  match(POW); }
    else if (check(LT)) { o = match(LT); }
    else if (check(LTE)) { o = match(LTE); }
    else if (check(EQEQ)) { o = match(EQEQ); }
    else if (check(NTEQ)) { o = match(NTEQ); }
    else if (check(GTE)) { o = match(GTE); }
    else if (check(GT)) { o = match(GT); }
    else if (check(AND)) { o = match(AND); }
    else { o = match(OR); }
    return o;
}
bool Parser::uniOpPending() {
    return check(NOT);
}
void Parser::uniOp() {
    debug("uniOp");
    match(NOT);
}

bool Parser::idCallPending() {
    return check(ID);
}
Lexeme* Parser::idCall() {
    debug("idCall");
    Lexeme *name = match(ID);
    Lexeme *opt = NULL;
    if (optCallPending()) { opt = optCall(); }
    Lexeme *i = cons(name,opt);
    i->type = IDCALL;
    return i;
}
bool Parser::optCallPending() {
    return functionCallPending() || listCallPending();
}
Lexeme* Parser::optCall() {
    Lexeme *t=NULL,*o = NULL;
    if (functionCallPending()) { 
        t = functionCall();
        if (optCallPending()) { o = optCall(); }
    }
    else { 
        t = listCall();
        if (optCallPending()) { o = optCall(); }
    }
    Lexeme *c = cons(t,o);
    c->type = OPTCALL;
    return c;        
}

bool Parser::varDefPending() { return check(VAR); }
Lexeme* Parser::varDef() { 
    debug("varDef");
    Lexeme* vd = match(VAR); Lexeme* name = match(ID); match(EQ); Lexeme *e = expr();
    vd->left = name;
    vd->right = e;
    return vd;
}

bool Parser::functionDefPending() { return check(FUNC); }
Lexeme* Parser::functionDef() {
    debug("functionDef");
    match(FUNC);
    Lexeme *name = NULL;
    if (check(ID)) { name = match(ID); }
    match(VAR);
    Lexeme *params = NULL;
    if (fnParamPending()) { params = fnParam(); }
    Lexeme* actions = block();
    Lexeme* s = cons(name,cons(params, actions));
    s->type = FUNC;
    return s;
}
bool Parser::functionCallPending() { return check(OPAREN); }
Lexeme* Parser::functionCall() {
    debug("functionCall");
    match(OPAREN);
    if (check(NEWLINE)) { match(NEWLINE); }
    Lexeme *p = NULL;
    if (fnArgPending()) { p = fnArg(); }
    if (check(NEWLINE)) { match(NEWLINE); }
    match(CPAREN);
    Lexeme *fc = cons(p, NULL);
    fc->type = FNCALL;
    return fc;
}
bool Parser::fnArgPending() { return exprPending(); }
Lexeme* Parser::fnArg() { //argument values
    debug("fnArg");
    Lexeme *e = expr();
    Lexeme *op = NULL;
    if (check(NEWLINE)) { match(NEWLINE); }
    if (check(COMMA)) { match(COMMA); op = fnArg(); }
    Lexeme *p = cons(e,op);
    p->type = FNARGLIST;
    return p;
}

bool Parser::fnParamPending() { return check(ID); }
Lexeme* Parser::fnParam() {
    debug("fnParam");
    Lexeme *name = match(ID);
    Lexeme *np = NULL;
    if (check(COMMA)) { match(COMMA); np = fnParam();  }
    Lexeme *p = cons(name,np);
    p->type = FNPARAMLIST;
    return p;
}

bool Parser::blockPending() { return check(COLON) || check(QMARK); }
Lexeme* Parser::block() { 
    debug("block");
    Lexeme* blk=NULL;
    if (check(COLON)) { 
        match(COLON); blk = statement(); match(SMCOLON);
    }
    else {
        match(QMARK); blk = statement(); match(SMCOLON);
    }
    return blk; 
}
bool Parser::conditionBlockPending() { return check(IF); }
Lexeme* Parser::conditionBlock() {
    debug("conditionBlock");
    Lexeme *cb = match(IF);
    cb->left = expr(); 
    cb->right = block(); 
    return cb;
}
bool Parser::loopBlockPending() { return check(WHILE); }
Lexeme* Parser::loopBlock() {
    debug("loopBlock");
    Lexeme *lb = match(WHILE);
    lb->left = expr(); 
    lb->right = block();
    return lb;
}
bool Parser::elseBlockPending() { return check(ELSE); }
Lexeme* Parser::elseBlock() { 
    debug("elseBlock");
    Lexeme *el = match(ELSE);
    el->left = NULL;
    el->right = block(); 
    return el;
}

bool Parser::listDefPending() { return check(OLIST); }
Lexeme* Parser::listDef() {
    debug("listDef");
    Lexeme* ls = match(OLIST); 
    if (exprPending()) { ls->left = expr();
        if (check(COMMA)) {
            match(COMMA);
            if (listItemDefPending()) { ls->right = listItemDef(); }
        }
    }
    match(CLIST);
    return ls;
}
bool Parser::listItemDefPending() { return exprPending(); }
Lexeme* Parser::listItemDef() {
    debug("listItemDef");
    Lexeme *l = cons(expr(), NULL);
    if (check(COMMA)) { 
        match(COMMA); 
        l->right = listItemDef();
    }
    l->type = LIST;
    return l;
}
bool Parser::listCallPending() { return check(OLIST); }
Lexeme* Parser::listCall() { 
    debug("listCall");
    match(OLIST);
    Lexeme *ls = cons(expr(), NULL);
    ls->type = LSCALL;
    match(CLIST);
    return ls;
}

//bool Parser:: { }
//void Parser:: { }

