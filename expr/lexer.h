#ifndef txture_expr_engine_lexer
#define txture_expr_engine_lexer

#include "token_list.h"
typedef struct
{
	char* formula;
	char* c; // current
} Lexer;

typedef enum
{
	PrecNone,
	PrecOr,
	PrecAnd,
	PrecBOr,
	PrecBXor,
	PrecBAnd,
	PrecBShift,
	PrecEq,
	PrecComp,
	PrecTerm,
	PrecFactor,
	PrecUnary,
	PrecPostfix,
	PrecLiteral
} Precedence;

typedef enum
{
#define T(variant, str, prec) variant,
	TOKEN_LIST (T)
#undef T
} TokenType;

#define T(variant, str, prec) str,
static const char* token_names[] = {TOKEN_LIST (T)};
#undef T

#define T(variant, str, prec) Prec##prec,
static Precedence token_prec[] = {TOKEN_LIST (T)};
#undef T

#define tt2str(tt) token_names[tt]
#define getprc(tt) token_prec[tt]

typedef struct
{
	TokenType type;
	// optional
	double opt_value;
} Token;

void
lexer_init (Lexer* lexer, char* formula);

Token
tokenize (Lexer* lexer);

#endif // !txture_expr_engine_lexer
