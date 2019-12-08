#define _CRT_SECURE_NO_WARNINGS
#define MAX_UINT 0x100000000

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <malloc.h>
#include "..//inc/BigNumber.h"
#include "..//inc/BigFloat.h"
//#include "..//common.h"

#define MAX_LEN 1024
long long int_modulo = 10;//(long long)((unsigned int)-1) + 1;


BigNumber bits_shifting(int j,BigNumber num)
{
    BigNumber BN;

    for (Itype i = 0; i < MAX_LEN; i++) BN._bits[i] = 0;

    for(Itype i = 0; i < num.SIZE; i++)
    {
        BN._bits[i + j] = num._bits[i];
    }

    return BN;
}

BigNumber bits_cutting(int j,BigNumber num)
{
    BigNumber BN;

    for (Itype i = 0; i < MAX_LEN; i++) BN._bits[i] = 0;

    for(Itype i = 0; i < MAX_LEN - j - 1; i++)
    {
        BN._bits[i + j] = num._bits[i];
    }

    return BN;
}

int memory_leak(BigNumber diff_exp,BigFloat* BF)//check memory leak        было BigFloat& BF
{
    if(diff_exp.SIZE > 1)
    {
           printf("Takes a lot of memory to convert the whole number!");
           return 1;
    }
    else if(diff_exp.SIZE == 1 && diff_exp._bits[0] + BF->mantissa.SIZE >= MAX_LEN)
    {
        if(diff_exp._bits[0] >= MAX_LEN)
        {
           printf("Takes a lot of memory to convert the whole number!");
           return 1;
        }
        else
        {
            //частичная утечка обрезка значений
            BF->mantissa = bits_cutting(diff_exp._bits[0],BF->mantissa);//Ignore the part that goes beyond the size of the array
            return 2;
        }
    }

    return 0;
}

void BigFloatAdjustment(BigFloat* x, BigFloat* y)
{//conversion to the same exponent
    BigNumber diff_exp;
    Itype greater_exp = BigNumberCompare(x->exponent,y->exponent);//ignoring the sign

    if(x->exponent._sign == 1 && y->exponent._sign == 1)
    {
        if(greater_exp == 1)
        {
            diff_exp = Sub(x->exponent,y->exponent);
            y->exponent = x->exponent;

            if(memory_leak(diff_exp,y) == 1)
            {
                for(Itype i = 0; i < MAX_LEN; i++) y->mantissa._bits[i] = 0;
            }
            else if(memory_leak(diff_exp,y) == 0)
            {
                y->mantissa = bits_shifting(diff_exp._bits[0],y->mantissa);
            }
        }
        else
        {
            diff_exp = Sub(y->exponent,x->exponent);
            x->exponent = y->exponent;

            if(memory_leak(diff_exp,x)== 1)
            {
                for(Itype i = 0; i < MAX_LEN; i++) x->mantissa._bits[i] = 0;
            }
            else if(memory_leak(diff_exp,x) == 0)
            {
                x->mantissa = bits_shifting(diff_exp._bits[0],x->mantissa);
            }
        }
    }
    else if(x->exponent._sign == -1 && y->exponent._sign == -1)
    {
        if(greater_exp == 1)
        {
            diff_exp = Sub(y->exponent,x->exponent);
            x->exponent = y->exponent;

            if(memory_leak(diff_exp,x) == 1)
            {
                for(Itype i = 0; i < MAX_LEN; i++) x->mantissa._bits[i] = 0;
            }
            else if(memory_leak(diff_exp,x) == 0)
            {
                x->mantissa = bits_shifting(diff_exp._bits[0],x->mantissa);
            }
        }
        else
        {
            diff_exp = Sub(x->exponent,y->exponent);
            y->exponent = x->exponent;

            if(memory_leak(diff_exp,y) == 1)
            {
                for(Itype i = 0; i < MAX_LEN; i++) y->mantissa._bits[i] = 0;
            }
            else if(memory_leak(diff_exp,y) == 0)
            {
                y->mantissa = bits_shifting(diff_exp._bits[0],y->mantissa);
            }
        }
    }
    else if(x->exponent._sign == 1 && y->exponent._sign == -1)
    {
        diff_exp = Sub(x->exponent,y->exponent);
        y->exponent = x->exponent;

        if(memory_leak(diff_exp,y) == 1)
        {
            for(Itype i = 0; i < MAX_LEN; i++) y->mantissa._bits[i] = 0;
        }
        else if(memory_leak(diff_exp,y) == 0)
        {
            y->mantissa = bits_shifting(diff_exp._bits[0],y->mantissa);
        }
    }
    else
    {
        diff_exp = Sub(y->exponent,x->exponent);
        x->exponent = y->exponent;

        if(memory_leak(diff_exp,x) == 1)
        {
            for(Itype i = 0; i < MAX_LEN; i++) x->mantissa._bits[i] = 0;
        }
        else if(memory_leak(diff_exp,x) == 0)
        {
            x->mantissa = bits_shifting(diff_exp._bits[0],x->mantissa);
        }
    }
}

BigFloat add(BigFloat x, BigFloat y)
{
    BigFloat z;

    if(x.exponent._sign != y.exponent._sign || BigNumberCompare(x.exponent,y.exponent) != 2)
    {
        BigFloatAdjustment(&x,&y);
    }

    z.exponent = x.exponent;
    z.mantissa = Add(x.mantissa,y.mantissa);

	return z;
}

BigFloat sub(BigFloat x, BigFloat y)
{
    BigFloat z;

	if(x.exponent._sign != y.exponent._sign || BigNumberCompare(x.exponent,y.exponent) != 2)
    {
        BigFloatAdjustment(&x,&y);
    }

    z.exponent = x.exponent;
    z.mantissa = Sub(x.mantissa,y.mantissa);

	return z;
}


BigFloat mul(BigFloat x, BigFloat y)
{
    BigFloat z;

	z.exponent = Add(x.exponent,y.exponent);
	z.mantissa = Mul(x.mantissa,y.mantissa);

	return z;
}

BigFloat _div(BigFloat x, BigFloat y)
{
    BigFloat q;

	q.exponent = Sub(x.exponent,y.exponent);
	q.mantissa = Div(x.mantissa,y.mantissa);

	return q;
}

BigFloat mul_Num(BigFloat x,Itype l)
{
    mul_num(&x.mantissa,l);

    return x;
}

BigFloat del_Num(BigFloat x,Itype l)
{
    del_num(&x.mantissa,l);

    return x;
}

BigFloat BF_abs(BigFloat x)
{
    if(x.mantissa._sign < 0)
    {
        x.mantissa._sign = 1;
    }

    return x;
}

Itype BigFloatCompare(BigFloat x, BigFloat y)
{
    BigNumber diff_exp;
    int diff_SIZE;

    if (BigNumberCompare(x.mantissa,y.mantissa) == 1 && BigNumberCompare(x.exponent,y.exponent) == 1)
    {
        if(x.mantissa._sign > y.mantissa._sign)
        {
            return 1;
        }
        else if(x.mantissa._sign == y.mantissa._sign && x.mantissa._sign == 1)
        {
            if(x.exponent._sign >= y.exponent._sign && x.exponent._sign == 1)
            {
                return 1;
            }
            else// x.exponent._sign < y.exponent._sign y.exponent._sign = 1 && x.exponent._sign == y.exponent._sign x.exponent._sign == -1
            {
                diff_exp = Sub(y.exponent,x.exponent);
                diff_SIZE = y.mantissa.SIZE - x.mantissa.SIZE;

                if(diff_exp.SIZE == 1 && diff_SIZE - (diff_exp._bits[0])*diff_exp._sign == 0)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            return 0;
        }
    }
	else if(BigNumberCompare(x.mantissa,y.mantissa) == 0 && BigNumberCompare(x.exponent,y.exponent) == 0)
    {
        if(y.mantissa._sign > x.mantissa._sign)
        {
            return 0;
        }
        else if(y.mantissa._sign == x.mantissa._sign && y.mantissa._sign == 1)
        {
            if(y.exponent._sign >= x.exponent._sign && y.exponent._sign == 1)
            {
                return 0;
            }
            else
            {
                diff_exp = Sub(x.exponent,y.exponent);
                diff_SIZE = x.mantissa.SIZE - y.mantissa.SIZE;

                if(diff_exp.SIZE == 1 && diff_SIZE - (diff_exp._bits[0])*diff_exp._sign == 0)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
        }
        else
        {
            return 1;
        }
    }
    else if(BigNumberCompare(x.mantissa,y.mantissa) == 1 && BigNumberCompare(x.exponent,y.exponent) == 0)
    {
        if(x.mantissa._sign > y.mantissa._sign)
        {
            return 1;
        }
        else if(x.mantissa._sign == y.mantissa._sign && x.mantissa._sign == 1)
        {
            if(x.exponent._sign > y.exponent._sign)
            {
                return 1;
            }
            else if(x.exponent._sign == y.exponent._sign && x.exponent._sign == -1)
            {
                return 1;
            }
            else
            {
                diff_exp = Sub(y.exponent,x.exponent);
                diff_SIZE = y.mantissa.SIZE - x.mantissa.SIZE;

                if(diff_exp.SIZE == 1 && diff_SIZE - (diff_exp._bits[0])*diff_exp._sign < 0)
                {
                    return 0;
                }
                else if(diff_exp.SIZE == 1 && diff_SIZE - (diff_exp._bits[0])*diff_exp._sign > 0)
                {
                    return 1;
                }
                else if(diff_exp.SIZE == 1 && diff_SIZE - (diff_exp._bits[0])*diff_exp._sign == 0)
                {
                    return 2;
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            return 0;
        }
    }
    else if(BigNumberCompare(x.mantissa,y.mantissa) == 0 && BigNumberCompare(x.exponent,y.exponent) == 1)
    {
        if(y.mantissa._sign > x.mantissa._sign)
        {
            return 0;
        }
        else if(y.mantissa._sign == x.mantissa._sign && y.mantissa._sign == 1)
        {
            if(y.exponent._sign > x.exponent._sign)
            {
                return 0;
            }
            else if(y.exponent._sign == x.exponent._sign && y.exponent._sign == -1)
            {
                return 0;
            }
            else
            {
                diff_exp = Sub(x.exponent,y.exponent);
                diff_SIZE = x.mantissa.SIZE - y.mantissa.SIZE;

                if(diff_exp.SIZE == 1 && diff_SIZE - (diff_exp._bits[0])*diff_exp._sign < 0)
                {
                    return 1;
                }
                else if(diff_exp.SIZE == 1 && diff_SIZE - (diff_exp._bits[0])*diff_exp._sign > 0)
                {
                    return 0;
                }
                else if(diff_exp.SIZE == 1 && diff_SIZE - (diff_exp._bits[0])*diff_exp._sign == 0)
                {
                    return 2;
                }
                else
                {
                    return 1;
                }
            }
        }
        else
        {
            return 1;
        }
    }
    else if(BigNumberCompare(x.mantissa,y.mantissa) == 2 && BigNumberCompare(x.exponent,y.exponent) == 1)
    {
        if(x.mantissa._sign > y.mantissa._sign)
        {
            return 1;
        }
        else if(x.mantissa._sign == y.mantissa._sign && x.mantissa._sign == 1)
        {
            if(x.exponent._sign >= y.exponent._sign && x.exponent._sign == 1)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else if(BigNumberCompare(x.mantissa,y.mantissa) == 2 && BigNumberCompare(x.exponent,y.exponent) == 0)
    {
        if(y.mantissa._sign > x.mantissa._sign)
        {
            return 0;
        }
        else if(y.mantissa._sign == x.mantissa._sign && y.mantissa._sign == 1)
        {
            if(y.exponent._sign >= x.exponent._sign && y.exponent._sign == 1)
            {
                return 0;
            }
            else
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }

    }
    else if(BigNumberCompare(x.mantissa,y.mantissa) == 0 && BigNumberCompare(x.exponent,y.exponent) == 2)
    {
        if(y.mantissa._sign > x.mantissa._sign)
        {
            return 0;
        }
        else if(y.mantissa._sign == x.mantissa._sign && y.mantissa._sign == 1)
        {
            if(y.exponent._sign >= x.exponent._sign && y.exponent._sign == 1)
            {
                return 0;
            }
            else if(y.exponent._sign == x.exponent._sign && y.exponent._sign == -1)
            {
                return 0;
            }
            else
            {
                diff_exp = Sub(x.exponent,y.exponent);
                diff_SIZE = x.mantissa.SIZE - y.mantissa.SIZE;

                if(diff_exp.SIZE == 1 && diff_SIZE - (diff_exp._bits[0])*diff_exp._sign > 0)
                {
                    return 0;
                }
                else
                {
                    return 1;
                }
            }
        }
        else
        {
            return 1;
        }
    }
    else if(BigNumberCompare(x.mantissa,y.mantissa) == 1 && BigNumberCompare(x.exponent,y.exponent) == 2)
    {
        if(x.mantissa._sign > y.mantissa._sign)
        {
            return 1;
        }
        else if(x.mantissa._sign == y.mantissa._sign && x.mantissa._sign == 1)
        {
            if(x.exponent._sign >= y.exponent._sign && x.exponent._sign == 1)
            {
                return 1;
            }
            else if(x.exponent._sign == y.exponent._sign && x.exponent._sign == -1)
            {
                return 1;
            }
            else
            {
                diff_exp = Sub(y.exponent,x.exponent);
                diff_SIZE = y.mantissa.SIZE - x.mantissa.SIZE;

                if(diff_exp.SIZE == 1 && diff_SIZE - (diff_exp._bits[0])*diff_exp._sign > 0)
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }
        else
        {
            return 0;
        }
    }
    else// x.mantissa == y.mantissa && x.exponent == y.exponent
    {
        if(x.mantissa._sign > y.mantissa._sign)
        {
            return 1;
        }
        else if(x.mantissa._sign == y.mantissa._sign && x.mantissa._sign == 1)
        {
            if(x.exponent._sign > y.exponent._sign)
            {
                return 1;
            }
            else if(x.exponent._sign == y.exponent._sign)
            {
                return 2;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
}

BigFloat Sqrt(BigFloat value)
{
    BigFloat prev = del_Num(value,2);//value / 2
    BigFloat curr = del_Num(add(prev,_div(value,prev)),2);//(prev + value / prev) / 2;
    BigFloat E;

    for (Itype i = 0; i < MAX_LEN; i++) E.mantissa._bits[0];E.exponent._bits[0];
    E.mantissa._bits[512] = 1;// 0.000000.....100000...  precision
    E.mantissa._sign = 1;
    E.exponent._sign = 1;

    while(BigFloatCompare(BF_abs(sub(curr,prev)),E) == 1)//fabs(curr - prev) > E
    {
        prev = curr;
        curr = del_Num(add(prev,_div(value,prev)),2);//(prev + value / prev) / 2;
    }

    /*
    const float E = 0.00000000001;
    int value = 2;
    float prev = value / 2;
    float curr = (prev + value / prev) / 2;

    while(fabs(curr - prev) > E)
    {
        prev = curr;
        curr = (prev + value / prev) / 2;
        printf("%f",curr);
    }

    printf("sqrt %i = %f",value,curr);
    */

    return curr;
}

BigFloat linear(BigFloat a, BigFloat b)
{
    BigFloat x;

    if(a.mantissa.SIZE == 0 && a.mantissa._bits[0] == 0)
    {
        printf("No roots!");
        return x;
    }

    b.mantissa._sign = -1;
    x = _div(b,a);// -b/a

    return x;
}

Roots sqreq(BigFloat a, BigFloat b, BigFloat c)
{
    BigFloat x1,x2,D;
    Roots roots;

    D = sub(mul(b,b),mul_Num(mul(a,c),4));// b*b - 4*a*c

    if(D.mantissa._sign < 0)
    {
        printf("Empty set!\n");
        return roots;
    }

    b.mantissa._sign = -1;

    x1 = _div(add(b,Sqrt(D)),mul_Num(a,2));// (-b + sqrt(D))/ 2*a
    x2 = _div(sub(b,Sqrt(D)),mul_Num(a,2));// (-b - sqrt(D))/ 2*a

    roots.x1 = x1;
    roots.x2 = x2;

    return roots;
}

void add_int(unsigned int * arr, int *len, int incr)
{
	long long leftover = incr;

	for (int i = 0; i < *len; ++i)
	{
		long long r = (long long)arr[i] + leftover;
		arr[i] = r % int_modulo;
		leftover = r / int_modulo;
	}
	if (leftover != 0)
	{
		arr[*len++] = leftover;
	}
}

void mul_10(unsigned int * arr, int *len)
{
	long long leftover = 0;

	for (int i = 0; i < *len; ++i)
	{
		long long r = (long long)arr[i] * (long long)10 + leftover;
		arr[i] = r % int_modulo;
		leftover = r / int_modulo;
	}
	if (leftover != 0)
	{
		arr[(*len)++] = leftover;
	}
}

BigFloat input_BF()
{
    BigFloat a;

    printf("Enter mantissa:\n");
    _input(&a.mantissa);
    printf("Enter exponent:\n");
    _input(&a.exponent);

    return a;
}

void output_BF(BigFloat a)
{
    printf("Mantissa: ");
    _output(a.mantissa);
    printf("Exponent: ");
    _output(a.exponent);
}

BigFloat input(char* str,int base)
{
    Itype q;
    Itype pos = 0;
    Itype k = 0;
	BigFloat res;
	Itype len = strlen(str);
	Chtype before_point[1024] = "";
	Chtype Str[1024] = "";

	if(base == 16)
    {
        q = 8;
    }
    else if(base == 2)
    {
        q = 32;
    }
    else if(base == 10)
    {
        q = 10;
    }
    else if(base == 8)
    {
        q = 11;
    }
    else
    {
         q = 8;
    }

	if (str[0] == '-')
	{
		res.mantissa._sign = -1;
		pos++;
	}
	else
	{
		res.mantissa._sign = 1;
	}

    for(int i = pos; strlen(str); i++)
    {
        if(str[i] == '.')
        {
            strcpy(Str, str + i + 1);
            break;
        }

        before_point[i] = str[i];
    }

    res.exponent.SIZE = 1;
    res.exponent._bits[0] = strlen(before_point);

    if (strlen(before_point) % q == 0) res.mantissa.SIZE = strlen(before_point) / q - 1;
	else res.mantissa.SIZE = strlen(before_point) / q;

    if (strlen(Str) % q == 0) res.mantissa.SIZE += strlen(Str) / q - 1;
	else res.mantissa.SIZE += strlen(Str) / q;

	for (Itype i = strlen(before_point);i > 0;i -= q)
	{
		res.mantissa._bits[k] = Convert(i > q ? before_point + i - q : before_point,base);
		if (i > q)before_point[i - q] = '\0';
		k++;
	}

	for (Itype i = strlen(Str);i > 0;i -= q)
	{
		res.mantissa._bits[k] = Convert(i > q ? Str + i - q : Str,base);
		if (i > q)Str[i - q] = '\0';
		k++;
	}

    return res;
}

char* to_string(BigFloat *a)
{
	char* num = malloc(sizeof(char) * 11 * MAX_LEN);
	int pos = 0;
	if (a->mantissa._sign == -1)
	{
		num[pos++] = '-';
	}
	for (int i = 0; i < a->exponent.SIZE; ++i)
	{
		unsigned int c = a->exponent._bits[i];
		do
		{
			num[pos++] = c % 10 + '0';
			c /= 10;
		} while (c);
	}

	for (int i = num[0] == '-' ? 1 : 0; pos - i > i; ++i)
	{
		char c = num[i];
		num[i] = num[pos - 1 - i];
		num[pos - 1 - i] = c;
	}
	int point_pos = pos;
	num[pos++] = '.';
	for (int i = a->mantissa.SIZE - 1; i >=0 ; --i)
	{
		unsigned int c = a->mantissa._bits[i];
		int beg_pos = pos;

		do{
			num[pos++] = c % 10 + '0';
			c /= 10;
		}while (c);
		for (int j = 0; beg_pos + j < pos - 1 - j;++j)
		{
			int c = num[beg_pos + j];
			num[beg_pos + j] = num[pos - 1 - j];
			num[pos - 1 - j] = c;
		}
	}
	for (int i = 0; point_pos + 1 + i < pos - i - 1; ++i)
	{
		char c = num[point_pos + 1 + i];
		num[point_pos + 1 + i] = num[pos - 1 - i];
		num[pos - 1 - i] = c;
	}
	num[pos] = '\0';

	return num;
}

void convert_from_dec(int num,int base,int i,int _bits[])//decimal to any base
{
    //num is decimal number
    if(num >= 1)
    {
        _bits[i] = num % base;
        i++;
        convert_from_dec(num / base,base,i,_bits);
    }
}

int convert_to_dec(char num[],int base)//Any base to decimal
{
    //num[i] - '0' - преобразование символа в цифру
    //int value of char A is 17,B is 18 etc thats why - 7 to get value in 16 base A(10),B(11),C(12)
    unsigned int decimal = 0;
    int len = strlen(num)-1;

    for(int i = len; i >= 0; i--)
    {
        //decimal += (num[i] - '0' > 10 ? (num[i] - '0' > 22 ? num[i] - '0' - 39 : num[i] - '0' - 7) : num[i] - '0') * pow(base,len - i);
        //decimal += ((int)tolower(num[i]) > 57 ? (int)tolower(num[i]) - 87 : (int)num[i] - 48) * pow(base,len - i);
        if (num[i] >= '0' && num[i] <= '9') decimal += ((int)num[i] - 48)*pow(base,len - i);
		else if (tolower(num[i]) >= 'a' && tolower(num[i]) <= 'f') decimal += ((int)tolower(num[i]) - 87)*pow(base,len - i);
		else printf("Convert error %c %s\n", tolower(num[i]), num);
    }

    return decimal;
}

Chtype* getExponent(BigFloat a)
{
    char* num = malloc(sizeof(char) * 11 * MAX_LEN);
	int pos = 0;

	if (a.exponent._sign == -1)
	{
		num[pos++] = '-';
	}

	for (int i = 0; i < a.exponent.SIZE; ++i)
	{
		unsigned int c = a.exponent._bits[i];
		do
		{
			num[pos++] = c % 10 + '0';
			c /= 10;
		} while (c);
	}

	for (int i = num[0] == '-' ? 1 : 0; pos - i > i; ++i)
	{
		char c = num[i];
		num[i] = num[pos - 1 - i];
		num[pos - 1 - i] = c;
	}

	int point_pos = pos;
	num[pos] = '\0';

	return num;
}

Chtype* getMantissa(BigFloat a)
{
    char* num = malloc(sizeof(char) * 11 * MAX_LEN);
	int pos = 0;

	if (a.mantissa._sign == -1)
	{
		num[pos++] = '-';
	}

	for (int i = 0; i < a.mantissa.SIZE; ++i)
	{
		unsigned int c = a.mantissa._bits[i];
		do
		{
			num[pos++] = c % 10 + '0';
			c /= 10;
		} while (c);
	}

	for (int i = num[0] == '-' ? 1 : 0; pos - i > i; ++i)
	{
		char c = num[i];
		num[i] = num[pos - 1 - i];
		num[pos - 1 - i] = c;
	}

	int point_pos = pos;
	num[pos] = '\0';

	return num;

	/*
    Itype j = 0;
    Chtype *mantissa = (char*)malloc(1024*sizeof(char));
    Chtype buffer[10];

    if(a.mantissa._sign == -1)
    {
        mantissa[0] = '-';
        j++;
    }

    for (Itype i = j; i < a.mantissa.SIZE; i++)
    {
        sprintf(buffer,"%d",a.mantissa._bits[i]);

        for(Itype k = 0; k < strlen(buffer); k++)
        {
            mantissa[i + k] = buffer[k];
            i++;
        }
    }

    return mantissa;
    */
}

BigFloat setExponent(char* str)
{
    BigFloat BF;
    BigNumber BN;
    Itype k = 0;

    if (strlen(str) % 8 == 0) BN.SIZE = strlen(str) / 8 - 1;
	else BN.SIZE = strlen(str) / 8;
	if (str[0] == '-') {
		BN._sign = -1;
		strcpy(str, str + 1);
	}
	else BN._sign = 1;

	for (Itype i = strlen(str);i > 0;i -= 8)
	{
		BN._bits[k] = Convert(i > 8 ? str + i - 8 : str, BN.base);
		if (i > 8)str[i - 8] = '\0';
		k++;
	}

    BF.exponent = BN;

    return BF;
}

BigFloat setMantissa(char* str)
{
    BigFloat BF;
    BigNumber BN;
    Itype k = 0;

    if (strlen(str) % 8 == 0) BN.SIZE = strlen(str) / 8 - 1;
	else BN.SIZE = strlen(str) / 8;
	if (str[0] == '-') {
		BN._sign = -1;
		strcpy(str, str + 1);
	}
	else BN._sign = 1;

	for (Itype i = strlen(str);i > 0;i -= 8)
	{
		BN._bits[k] = Convert(i > 8 ? str + i - 8 : str, BN.base);
		if (i > 8)str[i - 8] = '\0';
		k++;
	}

    BF.mantissa = BN;

    return BF;
}

