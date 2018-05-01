#include <stdio.h>
#include <stdlib.h>

#include "expr.h"

#define ALLOCATE(P, C) P* C=(P*)malloc(sizeof(P))

//Precedences
#define PREC_TERM    0
#define PREC_FACTOR  1
#define PREC_LITERAL 2

typedef struct Expr_vtable{
	void (*print)();
	double (*eval)();
	int (*precedence)();
} Vtable;

typedef struct ExprType{
	Vtable _vtable;
	int type; //Beschreibt den Typ der Expression
} Expr;

typedef struct NumberType{
	Expr _expr;
	double val;
} Number;

typedef struct BinaryType{
	Expr _expr;
	Expr* e1;
	Expr* e2;
} Binary;

typedef struct PlusTerm{
	Binary _binary;
} Plus;

typedef struct MinusTerm{
	Binary _binary;
} Minus;

typedef struct TimesTerm{
	Binary _binary;
} Times;

typedef struct OverTerm{
	Binary _binary;
} Over;

//---NUMBER FUNKTIONS---

void Number_print(Number* this, FILE *f)
{
	fprintf(f, "%f", this->val);//Virtueller Methoden Dispatcher
}

int Number_precedence()
{
	return PREC_LITERAL;
}

double Number_eval(Number* this)
{
	return (this->val);
}

//---PLUS FUNKTIONS---

void Plus_print(Plus* this, FILE *f)
{
	print(this->_binary.e1, f);//Virtueller Methoden Dispatcher
	fprintf(f, " + ");
	print(this->_binary.e2, f);
}

int Plus_precedence()
{
	return PREC_TERM;
}

double Plus_eval(Plus* this)
{
	return eval(this->_binary.e1) + eval(this->_binary.e2);
}

//---MINUS FUNKTIONS---

void Minus_print(Minus* this, FILE *f)
{
	print(this->_binary.e1, f);//Virtueller Methoden Dispatcher
	fprintf(f, " - ");
        if (precedence(this->_binary.e2) <= PREC_TERM) {
                fprintf(f, "("); print(this->_binary.e2, f); fprintf(f, ")");
        } else {
                print(this->_binary.e2, f);
        }
}

int Minus_precedence()
{
	return PREC_TERM;
}

double Minus_eval(Minus* this)
{
	return eval(this->_binary.e1) - eval(this->_binary.e2);
}

//---TIMES FUNKTIONS---

void Times_print(Times* this, FILE *f)
{
	if (precedence(this->_binary.e1) < PREC_FACTOR) {
                fprintf(f, "("); print(this->_binary.e1, f); fprintf(f, ")");
        } else {
                print(this->_binary.e1, f);
        }
	fprintf(f, " * ");
        if (precedence(this->_binary.e2) < PREC_FACTOR) {
                fprintf(f, "("); print(this->_binary.e2, f); fprintf(f, ")");
        } else {
                print(this->_binary.e2, f);
        }
}

int Times_precedence()
{
	return PREC_FACTOR;
}

double Times_eval(Times* this)
{
	return eval(this->_binary.e1) * eval(this->_binary.e2);
}

//---OVER FUNKTIONS---

void Over_print(Over* this, FILE *f)
{
	if (precedence(this->_binary.e1) < PREC_FACTOR) {
                fprintf(f, "("); print(this->_binary.e1, f); fprintf(f, ")");
        } else {
                print(this->_binary.e1, f);
        }
	fprintf(f, " / ");
        if (precedence(this->_binary.e2) <= PREC_FACTOR) {
                fprintf(f, "("); print(this->_binary.e2, f); fprintf(f, ")");
        } else {
                print(this->_binary.e2, f);
        }
}

int Over_precedence()
{
	return PREC_FACTOR;
}

double Over_eval(Over* this)
{
	return eval(this->_binary.e1) / eval(this->_binary.e2);
}

//---CREATE SPECIFIC TYPE---

Expr* number(double i)
{
	ALLOCATE(Number, r);
	r->_expr._vtable.print = Number_print;
	r->_expr._vtable.eval = Number_eval;
	r->_expr._vtable.precedence = Number_precedence;
	r->val = i;
	return &r->_expr;
}

Expr* plus(Expr *e1, Expr* e2) // Konstruktor für ein Expression Objekt
{
	ALLOCATE(Plus, r);//Speicher für r der größe Plus Allozieren
	r->_binary.e1 = e1;
	r->_binary.e2 = e2;
	r->_binary._expr._vtable.print = Plus_print;
	r->_binary._expr._vtable.eval = Plus_eval;
	r->_binary._expr._vtable.precedence = Plus_precedence;
	return &r->_binary._expr;
}

Expr* minus(Expr *e1, Expr* e2)
{
	ALLOCATE(Minus, r);//Speicher für r der größe Minus Allozieren
	r->_binary.e1 = e1;
	r->_binary.e2 = e2;
	r->_binary._expr._vtable.print = Minus_print;
	r->_binary._expr._vtable.eval = Minus_eval;
	r->_binary._expr._vtable.precedence = Minus_precedence;
	return &r->_binary._expr;
}

Expr* times(Expr *e1, Expr* e2)
{
	ALLOCATE(Times, r);//Speicher für r der größe Minus Allozieren
	r->_binary.e1 = e1;
	r->_binary.e2 = e2;
	r->_binary._expr._vtable.print = Times_print;
	r->_binary._expr._vtable.eval = Times_eval;
	r->_binary._expr._vtable.precedence = Times_precedence;
	return &r->_binary._expr;
}

Expr* over(Expr *e1, Expr* e2)
{
	ALLOCATE(Over, r);//Speicher für r der größe Minus Allozieren
	r->_binary.e1 = e1;
	r->_binary.e2 = e2;
	r->_binary._expr._vtable.print = Over_print;
	r->_binary._expr._vtable.eval = Over_eval;
	r->_binary._expr._vtable.precedence = Over_precedence;
	return &r->_binary._expr;
}

//---OPERATIONS---

void print(Expr* this, FILE* f)
{
	this->_vtable.print(this, f);
}

double eval(Expr* this)
{
	this->_vtable.eval(this);
}

int precedence(Expr* this)
{
	this->_vtable.precedence(this);
}

//---MAIN---

int main(int argc, char *argv[])
{
	Expr *p = plus(number(2), times(number(4), over(number(10), number(3))));
	print(p, stdout);
	printf(" => %g\n", eval(p));

/*  Für Zusatzaufgabe <string.h> einbinden und auskommentierten Code aktivieren
	if (argc > 1 && !strcmp(argv[1], "--zusatz")) {
		printf("p is Plus: %s\n", isinstance(p, Plus) ? "yes" : "no");
		printf("p is Expr: %s\n", isinstance(p, Expr) ? "yes" : "no");
		printf("p is Number: %s\n", isinstance(p, Number) ? "yes" : "no");
	}
*/

	return 0;
}
