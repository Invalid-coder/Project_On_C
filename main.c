#include <stdio.h>
#include <stdlib.h>
#include "inc/BigFloat.h"

void test1()
{
    BigFloat a,b,x;

    a = input("1a10.00000000000000000032342343242242343204",16);//Convert from 16 base to 10 base
    b = input("22b0.0330000000000000000324243222323077",16);//Convert from 16 base to 10 base
    x = add(b,a);

    //output_BF(x);
    printf("%s\n",to_string(&x));
}

void test2()
{
    BigFloat a,b,x;

    a = input("1a10.00000000000000000032342343242242343204",16);//Convert from 16 base to 10 base
    b = input("22b0.0330000000000000000324243222323077",16);//Convert from 16 base to 10 base
    x = sub(a,b);

    //output_BF(x);
    printf("%s\n",to_string(&x));
}

void test3()
{
    BigFloat a,b,x;

    a = input("1a10.00000000000000000032342343242242343204",16);//Convert from 16 base to 10 base
    b = input("22b0.0330000000000000000324243222323077",16);//Convert from 16 base to 10 base
    x = mul(a,b);

    //output_BF(x);
    printf("%s\n",to_string(&x));
}

void test4()
{
    BigFloat a,b,x;

    a = input("8.0",16);//Convert from 16 base to 10 base
    b = input("4.0",16);//Convert from 16 base to 10 base
    x = _div(a,b);

    output_BF(x);
}

void test5()
{
    BigFloat a,b,x;
    Roots roots;

    a = input("4.0",16);//Convert from 16 base to 10 base
    b = input("8.0",16);//Convert from 16 base to 10 base

    x = linear(a,b);

    output_BF(x);
}

void test6()
{
    BigFloat a,b,c;
    Roots roots;

    a = input("1231.00c004",16);//Convert from 16 base to 10 base
    b = input("20.0332d3",16);//Convert from 16 base to 10 base
    c = input("30.004454450f6",16);//Convert from 16 base to 10 base

    roots = sqreq(a,b,c);

    //output_BF(roots.x1);
    //output_BF(roots.x2);
    printf("%s\n",to_string(&roots.x1));
    printf("%s\n",to_string(&roots.x2));
}

int main()
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();

    return 0;
}

