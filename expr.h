#ifndef _EXPR_H
#define _EXPR_H

typedef struct ExprType Expr;

typedef struct Expr_vtable Vtable;

typedef struct NumberType Number;

typedef struct BinaryType Binary;

typedef struct MinusTerm Minus;

typedef struct PlusTerm Plus;

typedef struct TimesTerm Times;

typedef struct OverTerm Over;

//----------------------------

struct ExprType* plus(struct ExprType *, struct ExprType *);

void Plus_print(struct PlusTerm*, FILE*);

double Plus_eval(struct PlusTerm*);

int Plus_precedence();

//----------------------------

struct ExprType* times(struct ExprType *, struct ExprType *);

void Times_print(struct TimesTerm* , FILE*);

double Times_eval(struct TimesTerm*);

int Times_precedence();

//----------------------------

struct ExprType* minus(struct ExprType *, struct ExprType *);

void Minus_print(struct MinusTerm*, FILE*);

double Minus_eval(struct MinusTerm*);

int Minus_precedence();

//----------------------------

struct ExprType* over(struct ExprType *e1, struct ExprType *e2);

void Over_print(struct OverTerm*, FILE*);

double Over_eval(struct OverTerm*);

int Over_precedence();

//----------------------------

void print(struct ExprType*, FILE*);

double eval(struct ExprType*);

int precedence(struct ExprType*);


#endif
