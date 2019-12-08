#pragma once

#include "BigNumber.h"

typedef struct BigFloat
{
    BigNumber exponent;
    BigNumber mantissa;
} BigFloat;

typedef struct Roots
{
    BigFloat x1;
    BigFloat x2;
} Roots;

BigFloat add(BigFloat x, BigFloat y);
BigFloat sub(BigFloat x, BigFloat y);
BigFloat mul(BigFloat x, BigFloat y);
BigFloat mul_Num(BigFloat x,Itype l);
BigFloat del_Num(BigFloat x,Itype l);
BigFloat _div(BigFloat x, BigFloat y);
BigFloat Sqrt(BigFloat x);

BigFloat input_BF();
BigFloat input(char* num,int base);
char* to_string(BigFloat *a);
void output_BF(BigFloat x);

BigFloat linear(BigFloat a, BigFloat b);
Roots sqreq(BigFloat a, BigFloat b, BigFloat c);

Chtype* getExponent(BigFloat a);
Chtype* getMantissa(BigFloat a);
BigFloat setExponent(char* a);
BigFloat setMantissa(char* a);


