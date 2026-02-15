#include "parser.h"

#include "../utils.h"
#include "lexer.h"

#include <math.h>
#include <stdlib.h>

#define discard_const(s) (char*) s

static void
next (Parser* p)
{
	p->previous = p->current;
	p->current	= tokenize (p->lexer);
}

static int
match (Parser* p, TokenType type)
{
	if (p->current.type == type)
		{
			next (p);
			return 1;
		}

	return 0;
}

static Precedence
getprc (TokenType type)
{
	switch (type)
		{
		case TPlus:
		case TMinus: return PrecTerm;
		case TMul:
		case TDiv:
		case TMod:
		case TXor:
		case TAnd:
		case TOr: return PrecFactor;
		case TValue:
		case TX:
		case TV:
		case TM:
		case TY: return PrecLiteral;
		case TInvert: return PrecUnary;
		case TLparen:
		case TRparen:
		case TEOF: return PrecNone;
		default: __builtin_unreachable ();
		}
}

#define make_expr() malloc (sizeof (Expr))
#define make_exprk(k)                                                     \
	Expr* expr = make_expr ();                                            \
	expr->kind = k;

static Expr*
x_expr (Parser* p)
{
	make_exprk (KXExpr);

	next (p);
	return expr;
}

static Expr*
y_expr (Parser* p)
{
	make_exprk (KYExpr);

	next (p);
	return expr;
}

static Expr*
v_expr (Parser* p)
{
	make_exprk (KVExpr);

	next (p);
	return expr;
}

static Expr*
m_expr (Parser* p)
{
	make_exprk (KMExpr);

	next (p);
	return expr;
}

static Expr*
value_expr (Parser* p)
{
	Expr* expr = make_expr ();

	expr->kind		 = KValueExpr;
	expr->data.value = (ValueExpr) {.value = p->current.opt_value};

	next (p);
	return expr;
}

static UnaryOp
tt_2unop (TokenType type)
{
	switch (type)
		{
		case TInvert: return UnNot;
		case TMinus: return UnNegate;
		default: error ("expected unary operator");
		}
}

static BinaryOp
tt_2binop (TokenType type)
{
	switch (type)
		{
		case TPlus: return BiAdd;
		case TMinus: return BiSub;
		case TMul: return BiMul;
		case TDiv: return BiDiv;
		case TMod: return BiMod;
		case TXor: return BiXor;
		case TAnd: return BiAnd;
		case TOr: return BiOr;
		default: error ("expected binary operator");
		}
}

static Expr*
pexpr (Parser* p, Precedence prec);

static Expr*
prefix (Parser* p)
{
	Expr*	exr = make_expr ();
	UnaryOp op	= tt_2unop (p->current.type);
	next (p);

	exr->kind				 = KUnaryExpr;
	exr->data.unary.expr	 = pexpr (p, PrecUnary);
	exr->data.unary.operator = op;

	return exr;
}

static Expr*
grouping (Parser* p)
{
	next (p);

	Expr* exr = pexpr (p, PrecNone);
	if (!match (p, TRparen))
		{
			error ("expected ')'");
		}

	return exr;
}

static Expr*
binary (Parser* p, Expr* left)
{
	BinaryOp op = tt_2binop (p->current.type);
	next (p); // skip operator

	Expr* right	 = pexpr (p, PrecNone);
	Expr* binary = make_expr ();

	binary->kind				 = KBinaryExpr;
	binary->data.binary.lhs		 = left;
	binary->data.binary.rhs		 = right;
	binary->data.binary.operator = op;

	return binary;
}

static Expr*
pexpr (Parser* p, Precedence prec)
{
	Expr* exr;
	switch (p->current.type)
		{
		case TValue: exr = value_expr (p); break;
		case TX: exr = x_expr (p); break;
		case TY: exr = y_expr (p); break;
		case TV: exr = v_expr (p); break;
		case TM: exr = m_expr (p); break;
		case TLparen: exr = grouping (p); break;
		default: exr = prefix (p);
		}

	while (prec < getprc (p->current.type))
		{
			exr = binary (p, exr);
		}

	return exr;
}

#define MAX_EXPRS 256

void
parse (Parser* p)
{
	next (p);
	Expr* expr	 = pexpr (p, PrecNone);
	p->main_expr = expr;
	p->ran		 = 1;
}

void
parser_init (Parser* p, const char* formula)
{
	Lexer* lexer = malloc (sizeof (Lexer));
	lexer_init (lexer, discard_const (formula));

	p->lexer  = lexer;
	p->ran	  = 0;
	p->inited = 1;
}

void
expr_destroy (Expr* expr)
{
	switch (expr->kind)
		{
		case KUnaryExpr:
			{
				expr_destroy (expr->data.unary.expr);
				break;
			}
		case KBinaryExpr:
			{
				BinaryExpr* bin = &expr->data.binary;
				expr_destroy (bin->lhs);
				expr_destroy (bin->rhs);
				break;
			}
		default: break;
		}

	free (expr);
}

void
parser_destroy (Parser* parser)
{
	free (parser->lexer);
	expr_destroy (parser->main_expr);
}

#define roundi(v) ((int) round (v))

static double
evaluate_expr (Expr* expr, long x, long y, double value, long max);

static double
evaluate_binary (BinaryExpr* expr, long x, long y, double value, long max)
{
	switch (expr->operator)
		{
#define binop(variant, op)                                                \
	case variant:                                                         \
		return evaluate_expr (expr->lhs, x, y, value, max)                \
			op evaluate_expr (expr->rhs, x, y, value, max);

#define bin_binop(variant, op)                                            \
	case variant:                                                         \
		return roundi (evaluate_expr (expr->lhs, x, y, value, max))       \
			op roundi (evaluate_expr (expr->rhs, x, y, value, max));

			binop (BiAdd, +);
			binop (BiSub, -);
			binop (BiMul, *);
			binop (BiDiv, /);
			bin_binop (BiAnd, &);
			bin_binop (BiOr, |);
			bin_binop (BiXor, ^);

		case BiMod:
			return remainder (evaluate_expr (expr->lhs, x, y, value, max),
							  evaluate_expr (expr->rhs, x, y, value, max));
		default: __builtin_unreachable ();
		}

#undef binop
}

static double
evaluate_unary (UnaryExpr* expr, long x, long y, double value, long max)
{
	switch (expr->operator)
		{
		case UnNot:
			return ~roundi (evaluate_expr (expr->expr, x, y, value, max));
		case UnNegate:
			return -(evaluate_expr (expr->expr, x, y, value, max));
		default: __builtin_unreachable ();
		}
}

static double
evaluate_value (ValueExpr* expr)
{
	return expr->value;
}

static double
evaluate_expr (Expr* expr, long x, long y, double value, long max)
{
	switch (expr->kind)
		{
		case KValueExpr: return evaluate_value (&expr->data.value);
		case KUnaryExpr:
			return evaluate_unary (&expr->data.unary, x, y, value, max);
		case KBinaryExpr:
			return evaluate_binary (&expr->data.binary, x, y, value, max);
		case KXExpr: return x;
		case KYExpr: return y;
		case KVExpr: return value;
		case KMExpr: return max;
		default: __builtin_unreachable ();
		}
}

static double
evaluate (Parser* p, long x, long y, double value, long max)
{
	return evaluate_expr (p->main_expr, x, y, value, max);
}

double
expr (Parser* p, long x, long y, double value, long max,
	  const char* formula)
{
	if (p->ran == 1)
		{
			return evaluate (p, x, y, value, max);
		}
	else
		{
			parser_init (p, formula);
			parse (p);

			return evaluate (p, x, y, value, max);
		}
}
