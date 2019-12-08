#pragma once

#define MAX_UINT 0x100000000

#include "common.h"

typedef struct BigNumber {
	S_Ttype SIZE;
	Itype base;
	Itype _sign;
	UItype _bits[1024];
} BigNumber;

extern void _input(BigNumber *x);
extern void _output(BigNumber x);
extern BigNumber Add(BigNumber x, BigNumber y);
extern BigNumber Sub(BigNumber x, BigNumber y);
extern BigNumber Mul(BigNumber x, BigNumber y);
extern BigNumber add_num(BigNumber* x, Itype l);
extern BigNumber mul_num(BigNumber* x, Itype l);
extern BigNumber Div(BigNumber x, BigNumber y);
extern BigNumber Parse();
/*extern BigNumber mod(BigNumber x, BigNumber y);
extern BigNumber unadd(BigNumber x, BigNumber y);
extern BigNumber unsub(BigNumber x, BigNumber y);*/


