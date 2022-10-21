#define USE_COMPLEX
#include "complex.cpp"
#include <cstdio>

void print(cpl x)
{
    printf("%lg%c%lgi\n", x.re(), x.im() >= 0 ? '+' : '-', x.im() >= 0 ? x.im() : -x.im());
}

int main()
{
    auto a = cpl(2, 1);
    auto b = cpl(3, -5);
    printf("a = ");
    print(a);
    printf("b = ");
    print(b);

    auto sum = a + b;
    printf("sum = ");
    print(sum);

    auto sub = a - b;
    printf("sub = ");
    print(sub);

    auto mul = a * b;
    printf("mul = ");
    print(mul);

    auto div = a / b;
    printf("div = ");
    print(div);

    auto power = a^b;
    printf("power = ");
    print(power);

    auto abs = a.abs();
    printf("abs = %lg\n", abs);

    return 0;
}
