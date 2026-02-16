#include "lexer.h"

#include "../utils.h"

#include <ctype.h>
#include <errno.h>
#include <stdint.h>

#define advance() lexer->c++
#define peek(i)	  *(lexer->c + i)
#define curr()	  *lexer->c

void
lexer_init (Lexer* lexer, char* formula)
{
	debugln ("Parsing an expr %s", formula);
	lexer->formula = formula;
	lexer->c	   = lexer->formula;
}

static Token
number (Lexer* lexer)
{
	char* start = lexer->c;

	errno		  = 0;
	double number = strtod (start, &lexer->c);
	if (start == lexer->c)
		{
			lnerror ("expected number");
		}
	if ((errno == ERANGE && number == DBL_MAX))
		{
			strconv ();
		}
	if ((errno == ERANGE && number == DBL_MIN))
		{
			strconv ();
		}

	return (Token) {.opt_value = number, .type = TValue};
}

static Token
operator (Lexer* lexer)
{
#define double_token(ch, t, dch, dt)                                      \
	case ch:                                                              \
		if (peek (1) == dch)                                              \
			{                                                             \
				advance ();                                               \
				type = dt;                                                \
			}                                                             \
		else                                                              \
			{                                                             \
				type = t;                                                 \
			}                                                             \
		break;

#define triple_token(ch, t, dch, dt, tch, tt)                             \
	case ch:                                                              \
		if (peek (1) == dch)                                              \
			{                                                             \
				advance ();                                               \
				type = dt;                                                \
			}                                                             \
		else if (peek (1) == tch)                                         \
			{                                                             \
				advance ();                                               \
				type = tt;                                                \
			}                                                             \
		else                                                              \
			{                                                             \
				type = t;                                                 \
			}                                                             \
		break;

	TokenType type = 0;

	switch (curr ())
		{
		case '+': type = TPlus; break;
		case '-': type = TMinus; break;
		case '/': type = TSlash; break;
		case '^': type = TCaret; break;
		case '~': type = TTilda; break;
		case '(': type = TLparen; break;
		case '%': type = TPercent; break;
		case ')':
			type = TRparen;
			break;

			double_token ('*', TStar, '*', TStarStar);
			double_token ('|', TPipe, '|', TPipePipe);
			double_token ('&', TAmp, '&', TAmpAmp);
			double_token ('!', TBang, '=', TNEq);
			double_token ('=', 0;
						  lnerror ("txture's expr does not have '='"), '=',
						  TEq);

			triple_token ('>', TGt, '>', TRShift, '=', TGe);
			triple_token ('<', TLt, '<', TLShift, '=', TLe);

		default: lnerror ("expected operator, found '%c'", *lexer->c);
		}

	advance ();
	return (Token) {.type = type};
}

Token
tokenize (Lexer* lexer)
{
	while (isspace (curr ()))
		{
			advance ();
		}

	if (isdigit (curr ()))
		{
			return number (lexer);
		}

	if (isalpha (curr ()))
		{
			switch (curr ())
				{
				case 'x': advance (); return (Token) {.type = TX};
				case 'y': advance (); return (Token) {.type = TY};
				case 'v': advance (); return (Token) {.type = TV};
				case 'm': advance (); return (Token) {.type = TM};

				default: lnerror ("unexpected character '%c'", curr ());
				}
		}

	if (curr () == '\0')
		{
			return (Token) {.type = TEOF};
		}

	return operator (lexer);
}
