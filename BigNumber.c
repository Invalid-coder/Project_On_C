#define _CRT_SECURE_NO_WARNINGS
#define MAX_UINT 0x100000000

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <malloc.h>
#include "..//inc/BigNumber.h"
#include "..//inc/common.h"

#define MAX_LEN 1024

UItype Convert(char* number, Itype base);

void _input(BigNumber *BN) {
	//args
	Chtype str[MAX_LEN];
	Itype systema_chislennya;
	Itype k = 0;

	printf("Systema_chislennya: ");
	scanf("%d", &systema_chislennya);
	BN->base = systema_chislennya;

	for (Itype i = 0; i < MAX_LEN; i++) BN->_bits[i] = 0;
	printf("Number: ");
	getchar();
	//gets_s(str, 1024); // only for visual studio
	gets(str);
	if (strlen(str) % 8 == 0) BN->SIZE = strlen(str) / 8 - 1;
	else BN->SIZE = strlen(str) / 8;
	if (str[0] == '-') {
		BN->_sign = -1;
		strcpy(str, str + 1);
	}
	else BN->_sign = 1;

	for (Itype i = strlen(str);i > 0;i -= 8)
	{
		BN->_bits[k] = Convert(i > 8 ? str + i - 8 : str, BN->base);//convert_to_dec(i > 8 ? str + i - 8 : str, BN->base);
		if (i > 8)str[i - 8] = '\0';
		k++;
	}
}

//BigNumber add_num(const BigNumber* x, Itype l);
BigNumber mul_num(BigNumber* x, Itype l);
void change_size(BigNumber* x);

BigNumber Parse() {
	Itype n;
	BigNumber x;
	for (Itype i = 0; i < MAX_LEN; i++) x._bits[i] = 0;
	x.SIZE = 0;
	x._sign = 1;
	Chtype str[MAX_LEN];
	printf("Number: ");
	//gets_s(str, 1024);
	gets(str);
	for (Itype i = 0;i < strlen(str);i++) {
		change_size(&x);
		n = (int)str[i] - (int)'0';
		mul_num(&x, 10);
		add_num(&x, n);
	}
	return x;
}

void _output(BigNumber x){
	if (x._sign == -1) printf("-");
	for (Itype i = x.SIZE;i >= 0;i--)
	{
		if (x._bits == 0) x.SIZE--;
		else break;
	}
	printf("%x", x._bits[x.SIZE]);
	for (Itype i = x.SIZE-1;i >= 0;i--) printf("%08x", x._bits[i]);
	printf("\n");
}

void change_size(BigNumber* x) {
	Itype i = x->SIZE+1;
	while (i<=1024)
	{
		if (x->_bits[i] != 0) {
			x->SIZE++;
			i++;
		}
		else break;
	}
}

UItype Convert(char* number, Itype base) {
	UItype num = 0;
	Itype len = strlen(number)-1;
	//printf("len %d %s\n", len, number);
	for (Itype i = len; i >= 0; i--)
	{
		if (number[i] >= '0' && number[i] <= '9') num += ((Itype)number[i] - 48)*pow(base, len - i);
		else if (tolower(number[i]) >= 'a' && tolower(number[i]) <= 'f') num += ((Itype)tolower(number[i]) - 87)*pow(base, len - i);
		else printf("Convert error %c %s\n", tolower(number[i]), number);
	}
	return num;
}

//error C2371  redefinition; different basic types
void add_BigNumber(BigNumber x, BigNumber y, BigNumber *z);
void add_BigNumber2(BigNumber x, BigNumber y, BigNumber *z);
void sub_BigNumber(BigNumber x, BigNumber y, BigNumber *z);
void sub_BigNumber2(BigNumber x, BigNumber y, BigNumber *z);


BigNumber Add(BigNumber x,BigNumber y){
	BigNumber z;
	z.base = x.base;
	LLtype num = 0;
	for (Itype i = 0; i < MAX_LEN; i++) z._bits[i] = 0;
	z.SIZE = BigNumberMaxSize(x, y);
	if (x._sign == y._sign) add_BigNumber(x, y, &z);
	else sub_BigNumber(x, y, &z);
	return z;
}

BigNumber add_num(BigNumber* x, Itype l) {
	LLtype num = 0;
	for (Itype i = 0;i <= x->SIZE;i++) {
		num = x->_bits[i] + l;
		if (num > MAX_UINT) {
			x->_bits[i] = num - MAX_UINT;
			l = 1;
		}
		else { x->_bits[i] = num; l = 0; }
	}
	if (l != 0) {
		int i = x->SIZE+1;
		while (l!=0)
		{
			num = x->_bits[i] + l;
			if (num > MAX_UINT) {
				x->_bits[i] = num - MAX_UINT;
				l = 1;
			}
			else { x->_bits[i] = num; l = 0; }
			i++;
			x->SIZE++;
		}
	}
}

BigNumber mul_num(BigNumber* x, Itype l){
	Itype m = x->SIZE + 1;
	ULLtype k, t, b;
	Itype i;
	k = 0;
	for (i = 0; i < m; i++) {
		t = (ULLtype)x->_bits[i] * (ULLtype)l + k;
		x->_bits[i] = (UItype)t;
		if (t > MAX_UINT) k = t >> 32;
		else k = 0;
	}
	x->_bits[m] = k;
}

void del_num(BigNumber* x, Itype l)
{
	ULLtype t;
	Itype i;

	for (i = 0; i < x->SIZE; i++) {
		t = (ULLtype)x->_bits[i] / (ULLtype)l;
		x->_bits[i] = (UItype)t;
	}
}

void add_BigNumber(BigNumber x, BigNumber y, BigNumber *z) {
	z->_sign = x._sign;
	ULLtype num = 0;
	Itype i = 0;
	while (i <= x.SIZE && i <= y.SIZE)
	{
		num = (ULLtype)x._bits[i] + (ULLtype)y._bits[i];
		if (z->_bits[i] + num > MAX_UINT) {
			z->_bits[i] += (UItype)(num - MAX_UINT);
			z->_bits[i + 1] += 1;
			i++;
		}
		else {
			z->_bits[i] += (UItype)num;
			i++;
		}
	}
	if (x.SIZE > y.SIZE) {
		add_BigNumber2(x, y, z);
	}
	else if (x.SIZE < y.SIZE) {
		add_BigNumber2(y, x, z);
	}
}

void add_BigNumber2(BigNumber x, BigNumber y, BigNumber *z) {
	ULLtype num;
	for (Itype i = y.SIZE + 1;i <= x.SIZE;i++) {
		num = z->_bits[i] + x._bits[i];
		if (num > MAX_UINT) {
			z->_bits[i] += num - MAX_UINT;
			z->_bits[i + 1] = 1;
		}
		else z->_bits[i] += num;
	}
}

BigNumber Sub(BigNumber x, BigNumber y) {
	BigNumber z;
	z.base = x.base;
	LLtype num = 0;
	for (Itype i = 0; i < MAX_LEN; i++) z._bits[i] = 0;
	z.SIZE = BigNumberMaxSize(x, y);
	if (x._sign == y._sign) sub_BigNumber(x, y, &z);
	else add_BigNumber(x, y, &z);
	return z;
}

void sub_BigNumber(BigNumber x, BigNumber y, BigNumber *z) {
	z->_sign = x._sign * y._sign;
	Stype CompareResult = BigNumberCompare(x, y);
	if (CompareResult == 2) {
		z->SIZE = 0;
		z->_bits[0] = 0;
		z->_sign = 1;
	}
	else if (CompareResult == 1) {
		z->_sign = x._sign;
		sub_BigNumber2(x, y, z);
	}
	else if (CompareResult == 0) {
		z->_sign = y._sign * -1;
		sub_BigNumber2(y, x, z);
	}
}

void sub_BigNumber2(BigNumber x, BigNumber y, BigNumber *z) {
	LLtype num = 0;
	Itype i = 0;
	while (i <= x.SIZE && i <= y.SIZE)
	{
		if (x._bits[i] < y._bits[i]) {
			z->_bits[i] += MAX_UINT + x._bits[i] - y._bits[i];
			z->_bits[i + 1] -= 1;
			i++;
		}
		else {
			z->_bits[i] += x._bits[i] - y._bits[i];
			i++;
		}
	}
	if (x.SIZE > y.SIZE) {
		for (Itype i = y.SIZE + 1;i <= x.SIZE;i++) {
			if (z->_bits[i] == MAX_UINT - 1 && x._bits[i] == 0) z->_bits[i + 1] -= 1;
			else z->_bits[i] += x._bits[i];
		}
	}
	else if (x.SIZE < y.SIZE) {
		for (Itype i = x.SIZE + 1;i <= y.SIZE;i++) {
			if (z->_bits[i] == MAX_UINT - 1 && y._bits[i] == 0) z->_bits[i + 1] -= 1;
			else z->_bits[i] += y._bits[i];
		}
	}
}

Itype BigNumberMaxSize(BigNumber x, BigNumber y) {
	if (x.SIZE > y.SIZE) return x.SIZE;
	else return y.SIZE;
}

Itype BigNumberCompare(BigNumber x, BigNumber y) {
	if (x.SIZE > y.SIZE) return 1;
	else if (x.SIZE < y.SIZE) return 0;
	else
	{
		Itype i = x.SIZE;
		while (i>=0)
		{
			if (x._bits[i] > y._bits[i]) return 1;
			else if (x._bits[i] < y._bits[i]) return 0;
			i--;
		}
		return 2;
	}
}

void mulmns(BigNumber x, BigNumber y, BigNumber* z);

BigNumber Mul(BigNumber x, BigNumber y) {
	BigNumber z;
	z.base = x.base;
	z._sign = x._sign * y._sign;
	for (Itype i = 0; i < MAX_LEN; i++) z._bits[i] = 0;
	z.SIZE = x.SIZE + y.SIZE;
	mulmns(x, y, &z);
	return z;
}

void mulmns(BigNumber x, BigNumber y, BigNumber* z) {
	Itype m = x.SIZE+1;
	Itype n = y.SIZE+1;
	ULLtype k, t, b;
	Itype i, j;
	for (j = 0; j < n; j++) {
		k = 0;
		for (i = 0; i < m; i++) {
			t = (ULLtype)x._bits[i] * (ULLtype)y._bits[j] + (ULLtype)z->_bits[i + j] + k;
			z->_bits[i + j] = (UItype)t;
			if (t > MAX_UINT) k = t >> 32;
			else k = 0;
		}
		z->_bits[j + m] = k;
	}
}

int divmnu(BigNumber* q, BigNumber* r, const BigNumber x, const BigNumber y);
int nlz(unsigned x);

BigNumber Div(BigNumber x, BigNumber y) {
	BigNumber q;
	q.base = x.base;
	q._sign = x._sign * y._sign;
	for (Itype i = 0; i < MAX_LEN; i++) q._bits[i] = 0;
	q.SIZE = BigNumberMaxSize(x, y);
	BigNumber r;
	r.base = x.base;
	r._sign = x._sign * y._sign;
	for (Itype i = 0; i < MAX_LEN; i++) r._bits[i] = 0;
	r.SIZE = BigNumberMaxSize(x, y);
	divmnu(&q, &r, x, y);
	return q;
}

int nlz(unsigned x) {
	int n;

	if (x == 0) return(32);
	n = 0;
	if (x <= 0x0000FFFF) { n = n + 16; x = x << 16; }
	if (x <= 0x00FFFFFF) { n = n + 8; x = x << 8; }
	if (x <= 0x0FFFFFFF) { n = n + 4; x = x << 4; }
	if (x <= 0x3FFFFFFF) { n = n + 2; x = x << 2; }
	if (x <= 0x7FFFFFFF) { n = n + 1; }
	return n;
}

int divmnu(BigNumber* q, BigNumber* r, const BigNumber x, const BigNumber y) {

	Itype m = x.SIZE + 1;
	Itype n = y.SIZE + 1;
	const ULLtype b = 4294967296LL; // Number base (2**32).
	UItype *un, *vn;                         // Normalized form of u, v.
	ULLtype qhat;                   // Estimated quotient digit.
	ULLtype rhat;                   // A remainder.
	ULLtype p;                      // Product of two digits.
	LLtype t, k;
	Itype s, i, j;

	if (m < n || n <= 0 || y._bits[n - 1] == 0)
		return 1;                         // Return if invalid param.

	if (n == 1) {                        // Take care of
		k = 0;                            // the case of a
		for (j = m - 1; j >= 0; j--) {    // single-digit
			q->_bits[j] = (k*b + x._bits[j]) / y._bits[0];      // divisor here.
			k = (k*b + x._bits[j]) - q->_bits[j] * y._bits[0];
		}
		if (r != NULL) r->_bits[0] = k;
		return 0;
	}

	/* Normalize by shifting v left just enough so that its high-order
	bit is on, and shift u left the same amount. We may have to append a
	high-order digit on the dividend; we do that unconditionally. */

	s = nlz(y._bits[n - 1]);             // 0 <= s <= 31.
	vn = (UItype *)alloca(4 * n);
	for (i = n - 1; i > 0; i--)
		vn[i] = (y._bits[i] << s) | ((ULLtype)y._bits[i - 1] >> (32 - s));
	vn[0] = y._bits[0] << s;

	un = (UItype *)alloca(4 * (m + 1));
	un[m] = (ULLtype)x._bits[m - 1] >> (32 - s);
	for (i = m - 1; i > 0; i--)
		un[i] = (x._bits[i] << s) | ((ULLtype)x._bits[i - 1] >> (32 - s));
	un[0] = x._bits[0] << s;

	for (j = m - n; j >= 0; j--) {       // Main loop.
										 // Compute estimate qhat of q[j].
		qhat = (un[j + n] * b + un[j + n - 1]) / vn[n - 1];
		rhat = (un[j + n] * b + un[j + n - 1]) - qhat*vn[n - 1];
	again:
		if (qhat >= b || qhat*vn[n - 2] > b*rhat + un[j + n - 2])
		{
			qhat = qhat - 1;
			rhat = rhat + vn[n - 1];
			if (rhat < b) goto again;
		}

		// Multiply and subtract.
		k = 0;
		for (i = 0; i < n; i++) {
			p = qhat*vn[i];
			t = un[i + j] - k - (p & 0xFFFFFFFFLL);
			un[i + j] = t;
			k = (p >> 32) - (t >> 32);
		}
		t = un[j + n] - k;
		un[j + n] = t;

		q->_bits[j] = qhat;              // Store quotient digit.
		if (t < 0) {              // If we subtracted too
			q->_bits[j] = q->_bits[j] - 1;       // much, add back.
			k = 0;
			for (i = 0; i < n; i++) {
				t = (ULLtype)un[i + j] + vn[i] + k;
				un[i + j] = t;
				k = t >> 32;
			}
			un[j + n] = un[j + n] + k;
		}
	} // End j.
	  // If the caller wants the remainder, unnormalize
	  // it and pass it back.
	if (r != NULL) {
		for (i = 0; i < n - 1; i++)
			r->_bits[i] = (un[i] >> s) | ((unsigned long long)un[i + 1] << (32 - s));
		r->_bits[n - 1] = un[n - 1] >> s;
	}
	return 0;
}
