#include "lexer.h"

#include "../utils.h"

#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>

#define advance() lexer->c++

void
lexer_init (Lexer* lexer, char* formula)
{
	debugln ("Lexer received formula: %s", formula);
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
			error ("expected number");
		}
	if ((errno == ERANGE && number == DBL_MAX))
		{
			strconv ();
		}
	if ((errno == ERANGE && number == DBL_MIN))
		{
			strconv ();
		}

	debugln ("Lexer recognized number: %g", number);

	return (Token) {.opt_value = number, .type = TValue};
}

static Token
operator (Lexer* lexer)
{
	TokenType type = 0;
	switch (*lexer->c)
		{
		case '+': type = TPlus; break;
		case '-': type = TMinus; break;
		case '*': type = TMul; break;
		case '/': type = TDiv; break;
		case '&': type = TAnd; break;
		case '|': type = TOr; break;
		case '^': type = TXor; break;
		case '~': type = TInvert; break;
		case '(': type = TLparen; break;
		case ')': type = TRparen; break;
		default: error ("expected operator, found '%c'", *lexer->c);
		}

	debugln ("Lexer found an operator: %c", *lexer->c);
	advance ();
	return (Token) {.type = type};
}

Token
tokenize (Lexer* lexer)
{
	while (isspace (*lexer->c) && *lexer->c != EOF && *lexer->c != '\0')
		{
			advance ();
		}

	if (isdigit (*lexer->c))
		{
			return number (lexer);
		}

	if (isalpha (*lexer->c))
		{
			switch (*lexer->c)
				{
				case 'x':
					advance ();
					debugln ("Lexer found X coord");
					return (Token) {.type = TX};
				case 'y':
					advance ();
					debugln ("Lexer found Y coord");
					return (Token) {.type = TY};
				case 'v':
					advance ();
					debugln ("Lexer found V");
					return (Token) {.type = TV};
				case 'm':
					advance ();
					debugln ("Lexer found MAX");
					return (Token) {.type = TM};

				default: error ("unexpected character '%c'", *lexer->c);
				}
		}

	if (*lexer->c == EOF || *lexer->c == '\0')
		{
			return (Token) {.type = TEOF};
		}

	return operator (lexer);
}
