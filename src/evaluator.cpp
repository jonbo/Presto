// @author John Bartel
// @file evaluator.cpp
#include "evaluator.h"
using namespace std;

Evaluator::Evaluator() {
    indent = -1; // for PP
}

Lexeme* Evaluator::cons(Lexeme *a, Lexeme *b) {
    Lexeme *n = new Lexeme();
    n->left = a;
    n->right = b;
    return n;
}
Lexeme* Evaluator::newEnvironment() {
    Lexeme* e = cons(NULL, cons(NULL,NULL));
    e->type = ENV;
    return cons(e, NULL);
}
Lexeme* Evaluator::populate(Lexeme *var, Lexeme *value, Lexeme *env) {
    Lexeme *local = env->left;
    local->left = cons(var, local->left);
    local->right->left = cons(value, local->right->left);
    return value;
}
Lexeme* Evaluator::lookup(Lexeme *var, Lexeme *env) {
    Lexeme *local, *vars, *vals;
    while (env != NULL) {
        local = env->left;
        vars = local->left;
        vals = local->right->left;
        while (vars != NULL) {
            if (eqVar(var, vars->left)) { return vals->left; }
            vars = vars->right;
            vals = vals->right;
        }
        env = env->right;
    }
    // Throw exception (later)
    return NULL;
}
Lexeme* Evaluator::update(Lexeme *var, Lexeme *val, Lexeme *env) {
    Lexeme *local, *vars, *vals;
    while (env != NULL) {
        local = env->left;
        vars = local->left;
        vals = local->right->left;
        while (vars != NULL) {
            // Need to GC?
            if (eqVar(var, vars->left)) { vals->left = val; return val; }
            vars = vars->right;
            vals = vals->right;
        }
        env = env->right;
    }
    return NULL;
}
void Evaluator::display(Lexeme *env) {
    cout << "The environment is:" << endl;
    cout << "-------------------------------" << endl;    
    while (env != NULL) {
        printPAT(env->left->left, env->left->right->left);
        env = env->right;
        if (env != NULL) cout << "|" << endl;
    }
    cout << "-------------------------------" << endl;    
}
Lexeme* Evaluator::extend(Lexeme *vars, Lexeme *vals, Lexeme *env) {
    Lexeme* e = cons(vars, cons(vals,NULL));
    e->type = ENV;
    Lexeme *en = cons(e,env);
    populate(new Lexeme(ID,"this"),en,en);
    return en;
}
bool Evaluator::eqVar(Lexeme *a, Lexeme *b) {
    return a->sval == b->sval;
}

Lexeme* Evaluator::eval(Lexeme *tree, Lexeme *env) {
    switch (tree->type) {
        case INT: 
        case REAL:
        case STRING: return tree;
        case VAR: return evalVarDef(tree,env);
        case FUNC: return evalFuncDef(tree,env);
        case ID: return lookup(tree,env);
        case IDCALL: return evalIDCALL(tree,env);
        case STMT: return evalSTMT(tree,env);
        case EXPR_NO_OP:
        case EXPR_PAREN_NO_OP:
        case EXPR_PAREN:
        case EXPR: return evalEXPR(tree,env); 
        case IF: return evalIF(tree,env);
        case WHILE: return evalWHILE(tree,env);
        case ELSE: return evalELSE(tree,env);
        case OLIST: return evalLIST(tree,env);
        case NOT: return evalNOT(tree,env);

        default: break;
    }
    return new Lexeme(INT,0);
}
void Evaluator::pp(Lexeme *tree) {
    switch (tree->type) {
        case INT: cout << tree->ival; break;
        case REAL: 
            cout << tree->dval;
            double i;
            if (modf(tree->dval,&i) == 0.0) cout << ".0"; // Fixes .0 from getting truncated
            break;
        case STRING: cout << '"' << tree->sval << '"'; break;
        case ID: cout << tree->sval; break; // name
        case IDCALL: ppIDCALL(tree); break; // possible fn or list/array call
        case VAR:
            cout << "#";
            pp(tree->left); //ID
            cout << " = ";
            pp(tree->right); //EXPR
            break;
        case FUNC:
            ppFUNC(tree); 
            break;

        case STMT: ppSTMT(tree); break;
        case EXPR_NO_OP: pp(tree->left); break;
        case EXPR_PAREN_NO_OP:
            cout << "(";
            pp(tree->left);
            cout << ")";
            break;

        case EXPR: 
            pp(tree->left); // PRIMARY
            pp(tree->right->left); //OP
            pp(tree->right->right); //EXPR
            break;
        case EXPR_PAREN:
            cout << "(";
            pp(tree->left);
            if (tree->right->left) pp(tree->right->left); //OP
            if (tree->right->right) pp(tree->right->right); //OP
            cout << ")";
            break;
        case IF:
            cout << "if ";
            pp(tree->left);
            cout << ":\n";
            pp(tree->right);
            ppIndent(indent);
            cout << ";";
            break;
        case WHILE:
            cout << "wl ";
            pp(tree->left);
            cout << ":\n";
            pp(tree->right);
            ppIndent(indent);
            cout << ";";
            break;
        case ELSE:
            cout << "el?\n";
            pp(tree->right);
            ppIndent(indent);
            cout << ";";
            break;


        case OLIST:
            cout << "[";
            while (tree && tree->left) {
                pp(tree->left);
                tree = tree->right;
                if (tree) cout << ", ";
            }
            cout << "]";
            break;

        case NOT:
            cout << "!";
            pp(tree->left);
            break;


        // OPs
        case PLUS: cout << " + "; break;
        case MINUS: cout << " - "; break;
        case EQ: cout << " = "; break;
        case TIMES: cout << " * "; break;
        case DIVIDE: cout << " / "; break;
        case MOD: cout << " % "; break;
        case POW: cout << " ^ "; break;
        case LT: cout << " < "; break;
        case LTE: cout << " <= "; break;
        case EQEQ: cout << " == "; break;
        case NTEQ: cout << " != "; break;
        case GTE: cout << " >= "; break;
        case GT: cout << " > "; break;
        case AND: cout << " && "; break;
        case OR: cout << " || "; break;
        
        default: break;
    }
}
void Evaluator::ppSTMT(Lexeme *tree) {
    indent++;
    while (tree) {
        ppIndent(indent);
        pp(tree->left); //EXPR
        cout << "\n";
        tree = tree->right;
    }
    indent--;
}
void Evaluator::ppFUNC(Lexeme *tree) {
    cout << "@";
    if (tree->left) pp(tree->left);
    cout << " #";
    Lexeme *param = tree->right->left;
    while (param != NULL) {
        pp(param->left);
        param = param->right;
        if (param) cout << ",";
    }
    cout << ":\n";
    pp(tree->right->right);
    ppIndent(indent);
    cout << ";";

}
void Evaluator::ppIDCALL(Lexeme *tree) {
    // normal variables
    if (tree->right == NULL) {
        pp(tree->left); //ID
    }
    else if (tree->right->left->type == FNCALL) {
        pp(tree->left); //ID
        cout << "(";
        Lexeme *tmp = tree->right->left->left;
        while (tmp != NULL) {
            pp(tmp->left); // EXPR 
            tmp = tmp->right;
            if (tmp != NULL) {
                cout << ", ";
            }
        }
        cout << ")";
    }
    // LSCALL
    else {
        pp(tree->left); //ID
        Lexeme *ls = tree->right;
        while (ls != NULL) {
            cout << "[";
            pp(ls->left->left); // EXPR 
            ls = ls->right;
            cout << "]";
        }
    }
}
void Evaluator::ppIndent(int count) {
    while (count--) {
        cout << "    ";
    }
}
Lexeme* Evaluator::evalSTMT(Lexeme *tree, Lexeme *env) {
    while (tree) {
        // Special case to handle IF/ELSES
        if (tree->left && tree->left->left && tree->left->left->left && (tree->left->left->type == IF) && (tree->right && tree->right->left && tree->right->left->left && (tree->right->left->left->type == ELSE))) {
                //cout << "+" << tree->right->left->left->type << "+" << tree->left->left->type << "+";
                 tree->right->left->left->left = tree->left->left->left;
           
        }
        last = eval(tree->left, env);
        tree = tree->right;
    }
    return last;
}
Lexeme* Evaluator::evalEXPR(Lexeme *tree, Lexeme *env) {
    if (tree->type == EXPR_NO_OP || tree->type == EXPR_PAREN_NO_OP) {
        return eval(tree->left,env);
    }
    else { // with op
        Lexeme *op = tree->right->left;
        if (op->type == PLUS) { return evalPLUS(tree,env); } 
        if (op->type == MINUS) { return evalMINUS(tree,env); } 
        if (op->type == TIMES) { return evalTIMES(tree,env); } 
        if (op->type == DIVIDE) { return evalDIVIDE(tree,env); } 
        if (op->type == MOD) { return evalMOD(tree,env); } 
        if (op->type == POW) { return evalPOW(tree,env); } 
        if (op->type == LT) { return evalLT(tree,env); } 
        if (op->type == LTE) { return evalLTE(tree,env); } 
        if (op->type == EQEQ) { return evalEQEQ(tree,env); } 
        if (op->type == NTEQ) { return evalNTEQ(tree,env); } 
        if (op->type == GTE) { return evalGTE(tree,env); } 
        if (op->type == GT) { return evalGT(tree,env); } 
        if (op->type == AND) { return evalAND(tree,env); } 
        if (op->type == OR) { return evalOR(tree,env); } 
        if (op->type == EQ) { return evalASSIGN(tree,env); }

        return eval(tree->left,env);
    }
}
Lexeme* Evaluator::evalIDCALL(Lexeme *tree, Lexeme *env) {
    if (tree->right) {
        if (tree->right->left->type == FNCALL) {
            return evalFUNCCALL(tree,env);
        }
        else {
            // LIST CALL
            return evalLISTCALL(tree,env);
        }
    }
    else return eval(tree->left,env);
}
Lexeme* Evaluator::evalFUNCCALL(Lexeme *tree, Lexeme *env) {
    Lexeme *args = evalList(tree->right->left->left,env);
    Lexeme *unargs = tree->right->left->left;
    if (tree->left->sval == "env") { display(env); return new Lexeme(); }
    if (tree->left->sval == "print") {
        Lexeme *lastarg = new Lexeme(STRING, "");
        while (args) {
            lastarg = args;
            print(args->left);    
            args = args->right;
        }
        return lastarg->left;
    }
    if (tree->left->sval == "println") { 
        Lexeme *lastarg = new Lexeme(STRING, "");
        while (args) {
            lastarg = args;
            print(args->left);    
            args = args->right;
        }
        cout << endl;
        return lastarg->left;
    }
    if (tree->left->sval == "pp") { 
        pp(unargs->left); return args->left; 
    }
    if (tree->left->sval == "len") { 
        return new Lexeme(INT,(int)args->left->aval.size());
    }
    if (tree->left->sval == "remove") {
        int i = args->right->left->ival;
        if (args->left->aval.empty() || args->left->aval.size() == 0) {
            return new Lexeme(NUL);
        }
        Lexeme *tmp = args->left->aval.at(i);
        args->left->aval.erase(args->left->aval.begin() + i);
        return tmp; 
    }
    if (tree->left->sval == "insert") {
        int i = args->right->right->left->ival;
        args->left->aval.insert(args->left->aval.begin() + i, args->right->left);
        return args->left;
    }
    if (tree->left->sval == "call") {
        Lexeme *func = args->left;
        Lexeme *oparams = func->right->left;
        Lexeme *oargs = args->right;
        Lexeme *evalenv = extend(oparams,oargs,func->left);
        return eval(func->right->right,evalenv);
    }
    if (tree->left->sval == "apply") {
        Lexeme *func = args->left;
        Lexeme *oparams = func->right->left;
        Lexeme *oargs = args->right->left;
        Lexeme *n = cons(oargs->aval.at(0), NULL); // from list to arguments
        Lexeme *tmp = n;
        for (int i = 1, j = oargs->aval.size(); i < j; i++) {
            tmp->right = cons(oargs->aval.at(i), NULL);
            tmp = tmp->right;
        }
        Lexeme *evalenv = extend(oparams,n,func->left);
        return eval(func->right->right,evalenv);
    }
    if (tree->left->sval == "time") {
        struct timeb now;
        ftime(&now);
        args->left->aval.insert(args->left->aval.begin() + 0, new Lexeme(INT,(int)now.time));
        args->left->aval.insert(args->left->aval.begin() + 1, new Lexeme(INT,(int)now.millitm));
        return args->left;
    }
    Lexeme *func = lookup(tree->left,env);
    if (!func) {
        cout << "** unknown variable/function: " << tree->left->sval << "\n** ignoring error and attempting to continue\n";
        return last; 
    }

    Lexeme *params = func->right->left;
    Lexeme *evalenv = extend(params,args,func->left);
    return eval(func->right->right,evalenv);
}
Lexeme* Evaluator::evalLISTCALL(Lexeme *tree, Lexeme *env) {
    Lexeme *list = lookup(tree->left,env);
    Lexeme *call = tree->right;
    Lexeme *arg;
    int i;
    while (call && call->left->type == LSCALL) {
        arg = eval(call->left->left,env);
        i = arg->ival;
        if (i >= 0) list = list->aval.at(i);
        call = call->right;
    }
    return list; // Doesn't have to be list
}
void Evaluator::printPA(Lexeme *p, Lexeme *a) {
    while (p != NULL) {
        //cout << "((" << p->left->type << "))";
        print(p->left);
        cout << ":";
        print(a->left);
        p = p->right;
        a = a->right;
        if (p != NULL) cout << " and "; 
        else cout << endl;
    }
}
void Evaluator::printPAT(Lexeme *p, Lexeme *a) {
    //cout << "---------------------------" << endl;
    while (p != NULL) {
        cout << "|";
        print(p->left);
        cout << "\t\t: ";
        print(a->left);
        p = p->right;
        a = a->right;
        cout << endl;
    }
    //cout << "---------------------------" << endl;
}



Lexeme* Evaluator::evalList(Lexeme *tree, Lexeme *env) {
    if (tree == NULL) return NULL;
    Lexeme *e = cons(eval(tree->left,env), evalList(tree->right,env));
    e->type = E_ARGLIST;
    return e;
}
void Evaluator::printList(Lexeme *list) {
    cout << "[";
    for (int i=0, j=list->aval.size(); i<j; i++) {
        oprint(list->aval.at(i)); 
        if (i != j-1) cout << ", ";
    }
    cout << "]";
}
void Evaluator::print(Lexeme *a) {
    if (a->type == INT) cout << a->ival;
    else if (a->type == REAL) cout << a->dval;
    else if (a->type == STRING) cout << a->sval;
    else if (a->type == ID) cout << a->sval;
    else if (a->type == LIST) printList(a);
    else cout << a;
}
void Evaluator::oprint(Lexeme *a) {
    if (a->type == INT) cout << a->ival;
    else if (a->type == REAL) cout << a->dval;
    //else if (a->type == REAL) cout << printf("%f",a->dval);
    else if (a->type == STRING) cout << "\"" << a->sval << "\"";
    else if (a->type == ID) cout << a->sval;
    else if (a->type == LIST) printList(a);
    else cout << a;
}




Lexeme* Evaluator::evalVarDef(Lexeme *tree, Lexeme *env) {
    Lexeme* name = tree->left;
    Lexeme* value = eval(tree->right,env);
    //cout << "!!"<< tree->right->left->ival;
    //cout << "Adding variable " << name->sval << " with value ";
    //print(value);
    populate(name,value,env);
    return value;
}
Lexeme* Evaluator::evalFuncDef(Lexeme *tree, Lexeme *env) {
    Lexeme *name = tree->left;
    Lexeme *pb = tree->right;
    pb->type = FUNCBODY;
    Lexeme *tmp = cons(env,pb);
    //cout << "||" << name->sval << "||";
    if (name) populate(name,tmp,env);
    return tmp; 
}
Lexeme* Evaluator::evalLIST(Lexeme *tree, Lexeme *env) {
    Lexeme *arr = new Lexeme();
    arr->type = LIST;
    while (tree) {
        if (tree->left) arr->aval.push_back( eval(tree->left,env) );
        tree = tree->right;
    }
    return arr;
}
#define evalMathOp(op) \
    Lexeme *a = eval(tree->left,env); \
    Lexeme *b = eval(tree->right->right, env); \
    if (a->type == REAL || b->type == REAL) \
        return new Lexeme(REAL, a->dval op b->dval); \
    else return new Lexeme(INT, a->ival op b->ival); 



Lexeme* Evaluator::evalPLUS(Lexeme *tree, Lexeme *env) {
    Lexeme *a = eval(tree->left,env);
    Lexeme *b = eval(tree->right->right, env);
    if (a->type == STRING || b->type == STRING) 
        return new Lexeme(STRING, a->sval += b->sval);
    else if (a->type == REAL || b->type == REAL) 
        return new Lexeme(REAL, a->dval + b->dval);
    else return new Lexeme(INT, a->ival + b->ival);
}
Lexeme* Evaluator::evalMINUS(Lexeme *tree, Lexeme *env) {
    evalMathOp(-)
}
Lexeme* Evaluator::evalTIMES(Lexeme *tree, Lexeme *env) {
    evalMathOp(*)
}
Lexeme* Evaluator::evalDIVIDE(Lexeme *tree, Lexeme *env) {
    evalMathOp(/)
}
Lexeme* Evaluator::evalMOD(Lexeme *tree, Lexeme *env) {
    Lexeme *a = eval(tree->left,env);
    Lexeme *b = eval(tree->right->right, env);
    if (a->type == REAL || b->type == REAL) 
        return new Lexeme(REAL, fmod(a->dval, b->dval)); 
    else return new Lexeme(INT, a->ival % b->ival);
}
Lexeme* Evaluator::evalPOW(Lexeme *tree, Lexeme *env) {
    Lexeme *a = eval(tree->left,env);
    Lexeme *b = eval(tree->right->right, env);
    if (a->type == REAL || b->type == REAL) 
        return new Lexeme(REAL, pow(a->dval, b->dval)); 
    else return new Lexeme(INT, pow(a->ival, b->ival));
}
Lexeme* Evaluator::evalLT(Lexeme *tree, Lexeme *env) {
    evalMathOp(<)
}
Lexeme* Evaluator::evalLTE(Lexeme *tree, Lexeme *env) {
    evalMathOp(<=)
}
#define EPSILON 0.00001
Lexeme* Evaluator::evalEQEQ(Lexeme *tree, Lexeme *env) {
    Lexeme *a = eval(tree->left,env);
    Lexeme *b = eval(tree->right->right, env);
    if (a->type == LIST) return new Lexeme(INT, listEqv(a,b));
    else if (a->type == STRING || b->type == STRING)
        return new Lexeme(INT, a->sval == b->sval);
    else if (a->type == REAL || b->type == REAL)
        return new Lexeme(INT, fabs(a->dval - b->dval) < EPSILON);
    else return new Lexeme(INT, a->ival == b->ival);
}
bool Evaluator::listEqv(Lexeme *a, Lexeme *b) {
    if (a->aval.size() == b->aval.size()) {
        int i = 0;
        int j = a->aval.size();
        while (i < j) {
            if (a->aval[i]->type != b->aval[i]->type) {
                return false;
            }
            else {
                switch (a->aval[i]->type) {
                    case INT:
                        if (a->aval[i]->ival != b->aval[i]->ival) return false;
                        break;
                    case REAL: 
                        if (a->aval[i]->dval != b->aval[i]->dval) return false;
                        break;
                    case STRING:
                        if (a->aval[i]->sval != b->aval[i]->sval) return false;
                        break;
                    case LIST: 
                        if (!listEqv(a->aval[i], b->aval[i])) return false;
                        break;
                    default:
                        if (a->aval[i] != b->aval[i]) return false;
                        break;
                }
            }
            i++;
        }
        return true;
    }
    return false;
}
Lexeme* Evaluator::evalNTEQ(Lexeme *tree, Lexeme *env) {
    Lexeme *a = eval(tree->left,env);
    Lexeme *b = eval(tree->right->right, env);
    if (a->type == LIST || b->type == LIST) return new Lexeme(INT, !listEqv(a,b));
    else if (a->type == STRING || b->type == LIST) 
        return new Lexeme(INT, a->sval != b->sval);
    else if (a->type == REAL || b->type == REAL) 
        return new Lexeme(INT, fabs(a->dval - b->dval) >= EPSILON);
    else return new Lexeme(INT, a->ival != b->ival);
}
Lexeme* Evaluator::evalGTE(Lexeme *tree, Lexeme *env) {
    evalMathOp(>=)
}
Lexeme* Evaluator::evalGT(Lexeme *tree, Lexeme *env) {
    evalMathOp(>)
}
Lexeme* Evaluator::evalAND(Lexeme *tree, Lexeme *env) {
    Lexeme *a = eval(tree->left,env);
    if (!isTrue(a)) return new Lexeme(INT, 0); 
    Lexeme *b = eval(tree->right->right, env);
    if (!isTrue(b)) return new Lexeme(INT, 0);
    return new Lexeme(INT, 1);
}
Lexeme* Evaluator::evalOR(Lexeme *tree, Lexeme *env) {
    Lexeme *a = eval(tree->left,env);
    if (isTrue(a)) return new Lexeme(INT, 1); 
    Lexeme *b = eval(tree->right->right, env);
    if (isTrue(b)) return new Lexeme(INT, 1); 
    return new Lexeme(INT, 0);
}


Lexeme* Evaluator::evalASSIGN(Lexeme *tree, Lexeme *env) {
    Lexeme *id = tree->left->left;
    Lexeme *val = eval(tree->right->right, env);
    Lexeme *optCall = tree->left->right; // [] or ()
    // Normal Variable Assignment
    if (!optCall) { 
        return update(id,val,env);
    }
    // List reassignment
    else {
        //cout << "Ignoring";
        int i;
        Lexeme *ret = lookup(tree->left->left,env);
        Lexeme *arg;
        while (optCall) {
            if (optCall->left->type == LSCALL) {
                arg = eval(optCall->left->left,env);
                i = arg->ival;
                if (i >= 0) {
                    // Moving down the chain..
                    if (optCall->right) ret = ret->aval.at(i);
                    // Set the name value a[5] = 1
                    else {
                        if ((int)ret->aval.size() > i) {
                            ret->aval.at(i) = val;
                        }
                        else {
                            int j = ret->aval.size();
                            while (i > j) {
                                ret->aval.insert(ret->aval.begin() + j, new Lexeme(INT,0));
                                j++;
                            }
                            ret->aval.insert(ret->aval.begin() + i, val);
                        }
                    }
                }
            }
            else { }
            
            optCall = optCall->right;
        }
        return val;
    }
}

Lexeme* Evaluator::evalNOT(Lexeme *tree, Lexeme *env) {
    Lexeme *tmp = eval(tree->left,env);
    if ((tmp->type == INT) && (tmp->ival == 0)) 
        return new Lexeme(INT,1);
    else if ((tmp->type == REAL) && (tmp->dval == 0)) 
        return new Lexeme(INT,1);
    else if ((tmp->type == STRING) && (tmp->sval == "")) 
        return new Lexeme(INT,1);
    else 
        return new Lexeme(INT,0);
}

Lexeme* Evaluator::evalIF(Lexeme *tree, Lexeme *env) {
    if (isTrue(eval(tree->left, env))) {
        last = eval(tree->right,env);
    }
    return last;
}
Lexeme* Evaluator::evalWHILE(Lexeme *tree, Lexeme *env) {
    while (isTrue(eval(tree->left, env))) {
        last = eval(tree->right,env);
    } 
    return last;
}
Lexeme* Evaluator::evalELSE(Lexeme *tree, Lexeme *env) {
    if (!isTrue(eval(tree->left, env))) {
        last = eval(tree->right,env);
    }
    return last;
}

bool Evaluator::isTrue(Lexeme *a) {
    if (a->type == INT) { return (bool) a->ival; }
    else if (a->type == NUL) { return false; }
    else if (a->type == REAL) { return (bool) a->dval; }
    else if (a->type == STRING) {
        if (a->sval == "") return false;
        else return true;
    }
    else return (bool) a;
}
