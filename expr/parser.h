#ifndef txture_expr_engine
#define txture_expr_engine

#include "lexer.h"

#include <stdint.h>
typedef struct Expr Expr;

typedef enum
{
	UnNot,
	UnNegate,
	UnFact,
} UnaryOp;

typedef struct
{
	Expr*	expr;
	UnaryOp operator;
} UnaryExpr;

typedef enum
{
	BiAdd,
	BiSub,
	BiMul,
	BiDiv,
	BiMod,
	BiAnd,
	BiOr,
	BiBAnd,
	BiBOr,
	BiXor,
	BiPow,
	BiRShift,
	BiLShift,
	BiGt,
	BiGe,
	BiLt,
	BiLe,
	BiEq,
	BiNEq,
} BinaryOp;

typedef struct
{
	Expr*	 lhs;
	Expr*	 rhs;
	BinaryOp operator;
} BinaryExpr;

typedef struct
{
	double value;
} ValueExpr;

typedef enum
{
	KValueExpr,
	KUnaryExpr,
	KBinaryExpr,
	KXExpr,
	KYExpr,
	KVExpr,
	KMExpr,
} ExprKind;

typedef struct Expr
{
	ExprKind kind;
	union
	{
		ValueExpr  value;
		UnaryExpr  unary;
		BinaryExpr binary;
	} data;
} Expr;

typedef struct
{
	Expr** exprs;
	int	   count;
	int	   capacity;
} ExprList;

typedef struct
{
	Lexer*	lexer;
	Expr*	main_expr;
	Token	current;
	Token	previous;
	uint8_t ran;
	uint8_t inited;
} Parser;

void
parser_init (Parser* parser, const char* formula);

void
parser_destroy (Parser* parser);

void
parser_reset (Parser* parser);

void
print_ast (Parser* parser);

double
expr (Parser* parser, long x, long y, double value, long max,
	  const char* formulae);

#define def_parser(name) Parser name = (Parser) {.inited = 0};

#endif // !txture_expr_engine
