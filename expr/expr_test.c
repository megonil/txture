#include "expr_test.h"

#include "../utils.h"
#include "parser.h"

#include <math.h>

#define parser_test_failed(formula, expected, got)                        \
	println_exit (1, "test %d(%s) failed! Expected %g, but got %g", num,  \
				  formula, expected, got)

int num = 1;

#define X_TESTED 10
#define Y_TESTED 20
#define M_TESTED 255
#define V_TESTED 0

Parser test_parser;

static void
test (const char* formula, double expected)
{
	double got = expr (&test_parser, X_TESTED, Y_TESTED, V_TESTED,
					   M_TESTED, formula);
	if (fabs (got - expected) > 1e-3)
		{
			println ("");
			print_ast (&test_parser);
			parser_test_failed (formula, expected, got);
		}
	else
		{
			num++;
		}

	println ("");
	parser_reset (&test_parser);
}

static void
test_operators ()
{
	// arithmetic
	test ("2 + 150", 152);
	test ("2 * 5", 10);
	test ("3!", 6.0);
	test ("1 << 2", 1 << 2);
	test ("13 >> 1", 13 >> 1);
	test ("20 * 5", 100.0);
	test ("10 / 5", 2.0);
	test ("2 * (10 + 50)", 120.0);
	test ("2 * 10 + 50", 70);
	test ("5 & 2", 5 & 2);
	test ("5 ^ 2", 5 ^ 2);
	test ("5.1 * 2.5", 5.1 * 2.5);
	test ("-5.1 * 2.5", -5.1 * 2.5);
	test ("~5", ~5);
	test ("2 ** 5", 32);
	test ("5 / 2", 2.5);

	// logical
	test ("5 > 2", 1);
	test ("5 > 5", 0);
	test ("1 < 2", 1);
	test ("2 < 2", 0);
	test ("5 >= 2", 1);
	test ("1 <= 2", 1);
	test ("1 == 1", 1);
	test ("1 != 1", 0);
	test ("5 >= 2 && 1 == 1", 1);
	test ("5 <= 2 && 1 == 1", 0);
	test ("5 <= 2 || 1 == 1", 1);
}

static void
test_variables ()
{
	test ("x", X_TESTED);
	test ("y", Y_TESTED);
	test ("v", V_TESTED);
	test ("m", M_TESTED);
}

static void
test_double ()
{
	test ("5.5 & 4.1", 6 & 4);
	test ("5.5 | 4.1", 6 | 4);
	test ("5.5 * 2", 11);
}

void
expr_test ()
{
	test_operators ();
	test_variables ();
	test_double ();

	println_exit (0, "All test passed!");
}

#undef start_test
