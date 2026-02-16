#ifndef txtrure_expr_token_list
#define txtrure_expr_token_list

#define TOKEN_LIST(T)                                                     \
	T (TPlus, "+", Term)                                                  \
	T (TMinus, "-", Term)                                                 \
	T (TStar, "*", Factor)                                                \
	T (TSlash, "/", Factor)                                               \
	T (TPercent, "%", Factor)                                             \
	T (TCaret, ", ^", BXor)                                               \
	T (TAmp, "&", BAnd)                                                   \
	T (TPipe, "|", BOr)                                                   \
	T (TPipePipe, ", ||", Or)                                             \
	T (TAmpAmp, "&&", And)                                                \
	T (TBang, "!", Postfix)                                               \
	T (TStarStar, "**", Factor)                                           \
	T (TLparen, "(", None)                                                \
	T (TRparen, ")", None)                                                \
	T (TTilda, "~", Unary)                                                \
	T (TValue, "<value>", Literal)                                        \
	T (TX, "<x>", Literal)                                                \
	T (TY, "<y>", Literal)                                                \
	T (TV, "<v>", Literal)                                                \
	T (TM, "<m>", Literal)                                                \
	T (TEq, "==", Eq)                                                     \
	T (TNEq, "!=", Eq)                                                    \
	T (TGt, ">", Comp)                                                    \
	T (TGe, ">=", Comp)                                                   \
	T (TLt, "<", Comp)                                                    \
	T (TLe, "<=", Comp)                                                   \
	T (TRShift, ", >>", BShift)                                           \
	T (TLShift, ", <<", BShift)                                           \
	T (TEOF, "EOF", None)

#endif // !txtrure_expr_token_list
