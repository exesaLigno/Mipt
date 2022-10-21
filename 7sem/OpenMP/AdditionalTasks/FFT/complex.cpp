#pragma once
#include <cmath>

class Complex
{
  private:
    double real = 0;
    double imag = 0;
  
  public:
    Complex() { }

    Complex(double real_part, double imag_part)
    {
        real = fabs(real_part) < 1e-10 ? 0 : real_part;
        imag = fabs(imag_part) < 1e-10 ? 0 : imag_part;
    }

    Complex(double real_part)
    {
        real = fabs(real_part) < 1e-10 ? 0 : real_part;
    }

    Complex operator+(Complex other)
    {
        return Complex(real + other.real, imag + other.imag);
    }

    void operator+=(Complex other)
    {
        real += other.real;
        imag += other.imag;
    }

    Complex operator-(Complex other)
    {
        return Complex(real - other.real, imag - other.imag);
    }

    void operator-=(Complex other)
    {
        real -= other.real;
        imag -= other.imag;
    }

    Complex operator*(Complex other)
    {
        return Complex(real * other.real - imag * other.imag, real * other.imag + imag * other.real);
    }

    void operator*=(Complex other)
    {
        real = real * other.real - imag * other.imag;
        imag = real * other.imag + imag * other.real;
    }

    Complex operator/(Complex other)
    {
        double divider = other.real * other.real + other.imag * other.imag;
        return Complex((real * other.real + imag * other.imag) / divider,
                       (imag * other.real - real * other.imag) / divider);
    }

    void operator/=(Complex other)
    {
        double divider = other.real * other.real + other.imag * other.imag;
        real = (real * other.real + imag * other.imag) / divider;
        imag = (imag * other.real - real * other.imag) / divider;
    }

    bool operator==(Complex other)
    {
        return real == other.real and imag == other.imag;
    }

    bool operator!=(Complex other)
    {
        return real != other.real or imag != other.imag;
    }

    double abs()
    {
        return sqrt(real * real + imag * imag);
    }

    double re()
    {
        return real;
    }

    double im()
    {
        return imag;
    }

    Complex operator^(Complex other)
    {
        Complex e_power_mult_1 = other;
        Complex e_power_mult_2 = Complex(log(sqrt(real * real + imag * imag)),
            atan(imag / real));
        Complex e_power = e_power_mult_1 * e_power_mult_2;
        return Complex(exp(e_power.real) * cos(e_power.imag),
                       exp(e_power.real) * sin(e_power.imag));
    }

    Complex prec(long precision)
    {
        double re_rounded = round(real * pow(10, precision)) / pow(10, precision);
        double im_rounded = round(imag * pow(10, precision)) / pow(10, precision);
        return Complex(re_rounded, im_rounded);
    }

    void print(const char separator[] = " ")
    {
        printf("%.3lg", real);
        if (imag > 0)
            printf("+%.3lgi", imag);
        else if (imag < 0)
            printf("-%.3lgi", -imag);
        printf(separator);
    }

    Complex mirrorIm()
    {
        return Complex(real, -imag);
    }

    Complex mirrorRe()
    {
        return Complex(-real, imag);
    }
};

#ifdef USE_COMPLEX
    typedef Complex cpl;
#endif
