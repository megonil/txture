#ifndef txture_expr_operators_list
#define txture_expr_operators_list

#define BINOP_LIST(T)                                                     \
	T (BiAdd, TPlus)                                                      \
	T (BiSub, TMinus)                                                     \
	T (BiMul, TStar)                                                      \
	T (BiDiv, TSlash)                                                     \
	T (BiMod, TPercent)                                                   \
	T (BiAnd, TAmpAmp)                                                    \
	T (BiOr, TPipePipe)                                                   \
	T (BiBAnd, TAmp)                                                      \
	T (BiBOr, TPipe)                                                      \
	T (BiXor, TCaret)                                                     \
	T (BiPow, TStarStar)                                                  \
	T (BiRShift, TRShift)                                                 \
	T (BiLShift, TLShift)                                                 \
	T (BiGt, TGt)                                                         \
	T (BiGe, TGe)                                                         \
	T (BiLt, TLt)                                                         \
	T (BiLe, TLe)                                                         \
	T (BiEq, TEq)                                                         \
	T (BiNEq, TNEq)

#define UNOP_LIST(T)                                                      \
	T (UnNot, TTilda)                                                     \
	T (UnNegate, TMinus)                                                  \
	T (UnFact, TBang)

#endif // !txture_expr_operators_list
