#include <iostream>
#include <unordered_map>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <regex>
#include <mpfr.h>
// mpfr_add(mpfr_t rop, mpfr_t op1, mpfr_t op2, mpfr_rnd_t rnd)：
// rop：目标 MPFR 变量。
// op1：第一个操作数。
// op2：第二个操作数。
// rnd：舍入模式。
class Complex
{
public:
    mpfr_t real;
    mpfr_t imag;

    Complex(const char *r = "0", const char *i = "0")
    {
        mpfr_init_set_str(real, r, 10, MPFR_RNDN);
        mpfr_init_set_str(imag, i, 10, MPFR_RNDN);
    }
    Complex operator+(const Complex &other) const
    {
        Complex result;
        mpfr_add(result.real, real, other.real, MPFR_RNDN);
        mpfr_add(result.imag, imag, other.imag, MPFR_RNDN);
        return result;
    }

    Complex operator-(const Complex &other) const
    {
        Complex result;
        mpfr_sub(result.real, real, other.real, MPFR_RNDN);
        mpfr_sub(result.imag, imag, other.imag, MPFR_RNDN);
        return result;
    }

    Complex operator*(const Complex &other) const
    {
        Complex result;
        mpfr_mul(result.real, real, other.real, MPFR_RNDN);
        mpfr_mul(result.imag, imag, other.imag, MPFR_RNDN);
        return result;
    }

    Complex operator/(const Complex &other) const
    {
        Complex result;
        mpfr_div(result.real, real, other.real, MPFR_RNDN);
        mpfr_div(result.imag, imag, other.imag, MPFR_RNDN);
        return result;
    }

    static void parseAndStore(const std::string &line, std::unordered_map<std::string, Complex> &vars)
    {
        std::regex re("([a-zA-Z]+)=(-?\\d+(\\.\\d+)?)([+-]-?\\d+(\\.\\d+)?)?i?");
        std::regex re_op("([a-zA-Z]+)([+-/*])([a-zA-Z]+)");
        std::smatch match;

        if (std::regex_match(line, match, re))
        {
            if (match[4].matched)
            {
                vars[match[1]] = Complex(match[2].str().c_str(), match[4].str().c_str());
                mpfr_printf("%s = %.10Rf + %.10Rfi\n", match[1].str().c_str(), vars[match[1]].real, vars[match[1]].imag);//使用 MPFR 库的 mpfr_printf 函数的调用，用于格式化输出。：
            }
            else if (line.back() == 'i')
            {
                vars[match[1]] = Complex("0", match[2].str().c_str());
                mpfr_printf("%s = %.10Rfi\n", match[1].str().c_str(), vars[match[1]].imag);
            }
            else
            {
                vars[match[1]] = Complex(match[2].str().c_str(), "0");
                mpfr_printf("%s = %.10Rf\n", match[1].str().c_str(), vars[match[1]].real);
            }
        }
        else if (std::regex_match(line, match, re_op))
        {
            if (vars.count(match[1]) && vars.count(match[3]))
            {
                Complex result;
                switch (match[2].str()[0])
                {
                case '+':
                    result = vars[match[1]] + vars[match[3]];
                    break;
                case '-':
                    result = vars[match[1]] - vars[match[3]];
                    break;
                case '*':
                    result = vars[match[1]] * vars[match[3]];
                    break;
                case '/':
                    result = vars[match[1]] / vars[match[3]];
                    break;
                }
                mpfr_printf("%.10Rf + %.10Rfi\n", result.real, result.imag);
            }
            else
            {
                std::cout << "NameError: name is not defined" << std::endl;
            }
        }
    }
};

int main()
{
    std::unordered_map<std::string, Complex> vars;
    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line == "exit")
        {
            break;
        }
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        Complex::parseAndStore(line, vars);
        std::cout << ">>>";
    }
    return 0;
}
