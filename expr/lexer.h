#ifndef txture_expr_engine_lexer
#define txture_expr_engine_lexer

typedef struct
{
	char* formula;
	char* c; // current
} Lexer;

typedef enum
{
	TPlus,	 // +
	TMinus,	 // -
	TMul,	 // *
	TDiv,	 // /
	TMod,	 // %
	TXor,	 // ^
	TAnd,	 // &
	TOr,	 // |
	TLparen, // (
	TRparen, // )
	TInvert, // ~
	TValue,	 // double value
	TX,		 // x coordinat
	TY,		 // y coordinat
	TV,		 // value provided by the generator
	TM,
	TEOF,
} TokenType;

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
