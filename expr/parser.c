#include "parser.h"

#include "../utils.h"
#include "lexer.h"
#include "operators_list.h"

#include <math.h>
#include <stdlib.h>

#define discard_const(s) (char*) s

static void
next (Parser* p)
{
	p->previous = p->current;
	p->current	= tokenize (p->lexer);
	debug ("%s ", tt2str (p->current.type));
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

#define T(un, t) t,
static TokenType unop_2tt[] = {UNOP_LIST (T)};
#undef T

#define unop_2str(un) tt2str (unop_2tt[un])

#define T(un, t)	  t,
static TokenType binop_2tt[] = {BINOP_LIST (T)};
#undef T

#define binop_2str(bin) tt2str (binop_2tt[bin])

static UnaryOp
tt_2unop (TokenType type)
{
#define T(un, t)                                                          \
	case t: return un;
	switch (type)
		{
			UNOP_LIST (T);
		default:
			lnerror ("expected unary operator, got `%s`", tt2str (type));
		}
#undef T
}

static BinaryOp
tt_2binop (TokenType type)
{
#define T(un, t)                                                          \
	case t: return un;
	switch (type)
		{
			BINOP_LIST (T);
		default:
			lnerror ("expected binary operator, got `%s`", tt2str (type));
		}
#undef T
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
	Precedence prec = getprc (p->current.type);
	BinaryOp   op	= tt_2binop (p->current.type);

	next (p); // skip operator

	Expr* right	 = pexpr (p, prec);
	Expr* binary = make_expr ();

	binary->kind				 = KBinaryExpr;
	binary->data.binary.lhs		 = left;
	binary->data.binary.rhs		 = right;
	binary->data.binary.operator = op;

	return binary;
}

static Expr*
fact_expr (Parser* p, Expr* subexpr)
{
	make_exprk (KUnaryExpr);
	expr->data.unary.operator = UnFact;
	expr->data.unary.expr	  = subexpr;
	next (p);

	return expr;
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
			if (p->current.type == TBang)
				{
					exr = fact_expr (p, exr);
				}
			else
				{
					exr = binary (p, exr);
				}
		}

	return exr;
}

#define MAX_EXPRS 256

void
parse (Parser* p)
{
	next (p);
	Expr* expr = pexpr (p, PrecNone);

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

int offset = 0;

void
parser_reset (Parser* p)
{
	p->inited = 0;
	p->ran	  = 0;
	offset	  = 0;
	parser_destroy (p);
}

static void
print_offset ()
{
	for (int i = 0; i < offset; ++i)
		{
			printf (" ");
		}
}

static void
print_node (Expr* expr)
{
	print_offset ();
	switch (expr->kind)
		{
		case KUnaryExpr:
			{
				offset += 2;
				UnaryExpr* un = &expr->data.unary;

				println ("Unary(%s)", unop_2str (un->operator));
				print_node (un->expr);
				offset -= 2;
				break;
			}
		case KBinaryExpr:
			{
				offset += 2;
				BinaryExpr* bin = &expr->data.binary;

				println ("Binary(%s)", binop_2str (bin->operator));
				print_node (bin->lhs);
				print_node (bin->rhs);
				offset -= 2;
				break;
			}

		case KValueExpr:
			println ("Value(%g)", expr->data.value.value);
			break;
		case KXExpr: println ("X"); break;
		case KYExpr: println ("Y"); break;
		case KVExpr: println ("V"); break;
		case KMExpr: println ("M"); break;
		}
}

void
print_ast (Parser* parser)
{
	print_node (parser->main_expr);
}

static void
expr_destroy (Expr* expr);

void
parser_destroy (Parser* parser)
{
	free (parser->lexer);
	expr_destroy (parser->main_expr);
}

static void
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

#define roundi(v) ((int) round (v))

static double
fact (double v)
{
	long result = 1;
	long n		= (long) floor (v);
	for (int i = 1; i <= n; ++i)
		{
			result *= i;
		}

	return result;
}

static double
evaluate_expr (Expr* expr, long x, long y, double value, long max);

#define evaluate_subexpr(EXPR) evaluate_expr (EXPR, x, y, value, max)

static double
evaluate_binary (BinaryExpr* expr, long x, long y, double value, long max)
{
	switch (expr->operator)
		{
#define evaluate_left()	 evaluate_subexpr (expr->lhs)
#define evaluate_right() evaluate_subexpr (expr->rhs)

#define binop(variant, op)                                                \
	case variant: return evaluate_left () op evaluate_right ();

#define bin_binop(variant, op)                                            \
	case variant:                                                         \
		return (roundi (evaluate_left ())) op (roundi (evaluate_right ()));

#define func_binop(variant, func)                                         \
	case variant: return func (evaluate_left (), evaluate_right ());

			binop (BiAdd, +);
			binop (BiSub, -);
			binop (BiMul, *);
			binop (BiDiv, /);
			binop (BiGt, >);
			binop (BiGe, >=);
			binop (BiLt, <);
			binop (BiLe, <=);
			binop (BiEq, ==);
			binop (BiNEq, !=);
			binop (BiAnd, &&);
			binop (BiOr, ||);

			bin_binop (BiBAnd, &);
			bin_binop (BiBOr, |);
			bin_binop (BiXor, ^);
			bin_binop (BiRShift, >>);
			bin_binop (BiLShift, <<);

			func_binop (BiMod, fmod);
			func_binop (BiPow, pow);

		default: __builtin_unreachable ();
		}

#undef binop
#undef bin_binop
#undef func_binop
#undef evaluate_left
#undef evaluate_right
}

static double
evaluate_unary (UnaryExpr* expr, long x, long y, double value, long max)
{
#define evaluate_un() evaluate_subexpr (expr->expr)
	switch (expr->operator)
		{
		case UnNot: return ~roundi (evaluate_un ());
		case UnNegate: return -(evaluate_un ());
		case UnFact: return fact (evaluate_un ());

		default: __builtin_unreachable ();
		}
#undef evaluate_un
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
			debugln ("\nParsed formula %s",
					 formula); // end the print sequence

			return evaluate (p, x, y, value, max);
		}
}
