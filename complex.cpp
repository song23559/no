#include <iostream>
#include <unordered_map>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <regex>
class Complex//对复数类的定义 包含封装的操作函数
{
public:
    double real;
    double imag;
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    //对运算符的重载
    Complex operator+(const Complex &other) const
    {
        return Complex(real + other.real, imag + other.imag);
    }
    Complex operator-(const Complex &other) const
    {
        return Complex(real - other.real, imag - other.imag);
    }
    Complex operator*(const Complex &other) const
    {
        return Complex(real * other.real - imag * other.imag, real * other.imag + imag * other.real);
    }
    Complex operator/(const Complex &other) const
    {
        double denominator = other.real * other.real + other.imag * other.imag;
        return Complex((real * other.real + imag * other.imag) / denominator,
                       (imag * other.real - real * other.imag) / denominator);
    }

    //定义求模和求共轭函数的运算
    Complex conjugate() const
    {
        return Complex(real, -imag);
    }
    double mod() const
    {
        return sqrt(real * real + imag * imag);
    }

    //实现复数运算的操作函数
    static void parseAndStore(const std::string &line, std::unordered_map<std::string, Complex> &vars)
    {
        //解析输入的字符串 line，并根据定义的一系列正则表达式进行操作，将解析结果存储在一个传入的无序映射 vars 中
        std::regex re("([a-zA-Z]+)=(-?\\d+(\\.\\d+)?)([+-]-?\\d+(\\.\\d+)?)?i?");                    //a=3+5i
        std::regex re_var("([a-zA-Z]+)");                                                            //a
        std::regex re_num("(-?\\d+(\\.\\d+)?)([+-]-?\\d+(\\.\\d+)?)?i?");                            //5+5i
        std::regex re_var_assign("([a-zA-Z]+)=([a-zA-Z]+)");                                         //a=b
        std::regex re_op("([a-zA-Z]+)([+-/*])([a-zA-Z]+)");                                          //a[+-*/]b
        std::regex re_mod("mod\\(([^)]+)\\)");                                                       //mod[]
        std::regex re_con("con\\(([^)]+)\\)");                                                       //con[]
        std::regex re_mod_assign("([a-zA-Z]+)=mod\\(([^)]+)\\)");                                    //a=mod[]
        std::regex re_con_assign("([a-zA-Z]+)=con\\(([^)]+)\\)");                                    //a=con[]
        std::regex re_expr("((([a-zA-Z]+)|(-?\\d+(\\.\\d+)?))(\\+\\d+i)?)(\\+((([a-zA-Z]+)|(-?\\d+(\\.\\d+)?))(\\+\\d+i)?))+");//连续加法
        std::regex re_expr2("((([a-zA-Z]+)|(-?\\d+(\\.\\d+)?))(\\+\\d+i)?)(\\-((([a-zA-Z]+)|(-?\\d+(\\.\\d+)?))(\\+\\d+i)?))+");//连续减法
        std::regex re_expr_mul("((([a-zA-Z]+)|(-?\\d+(\\.\\d+)?))(\\+\\d+i)?)(\\*((([a-zA-Z]+)|(-?\\d+(\\.\\d+)?))(\\+\\d+i)?))+");//连续乘法
        std::regex re_expr_div("((([a-zA-Z]+)|(-?\\d+(\\.\\d+)?))(\\+\\d+i)?)(\\/((([a-zA-Z]+)|(-?\\d+(\\.\\d+)?))(\\+\\d+i)?))+");//连续除法

        //存储正则表达式匹配的结果
        std::smatch match;


        if (std::regex_match(line, match, re))
        {
            if (match[4].matched)//有实数和虚数部分
            {
                //std::unordered_map 就是一种映射容器，将变量名（match[]）映射到对应的复数值
                vars[match[1]] = Complex(std::stod(match[2]), std::stod(match[4]));//stod将字符串转换成double
                std::cout << match[1] << " = " << vars[match[1]].real << " + " << vars[match[1]].imag << "i" << std::endl;
            }
            
            else if (line.back() == 'i')//只有虚数部分
            {
                vars[match[1]] = Complex(0, std::stod(match[2]));
                std::cout << match[1] << " = " << vars[match[1]].imag << "i" << std::endl;
            }
            else//只有实数部分
            {
                vars[match[1]] = Complex(std::stod(match[2]), 0);
                std::cout << match[1] << " = " << vars[match[1]].real << std::endl;
            }
        }
        else if (std::regex_match(line, match, re_var_assign))
        {
            if (vars.count(match[2]))
            {
                vars[match[1]] = vars[match[2]];//将第二个变量的值赋值给第一个
                std::cout << match[1] << " = " << vars[match[1]].real << " + " << vars[match[1]].imag << "i" << std::endl;
            }
            else
            {
                std::cout << "NameError: name '" << match[2] << "' is not defined" << std::endl;//如果是未定义变量则报错
            }
        }
        else if (std::regex_match(line, match, re_var))
        {
            if (vars.count(match[1]))//如果变量已经有定义，则打印对应的复数
            {
                std::cout << match[1] << " = " << vars[match[1]].real << " + " << vars[match[1]].imag << "i" << std::endl;
            }
            else//未定义则报错
            {
                std::cout << "NameError: name '" << match[1] << "' is not defined" << std::endl;
            }
        }
        else if (std::regex_match(line, match, re_num))
        {
            if (match[3].matched)//包含实部和虚部
            {
                Complex c(std::stod(match[1]), std::stod(match[3]));
                std::cout << c.real << " + " << c.imag << "i" << std::endl;
            }
            else if (line.back() == 'i')//只有虚部
            {
                Complex c(0, std::stod(match[1]));
                std::cout << c.imag << "i" << std::endl;
            }
            else//只有实部
            {
                Complex c(std::stod(match[1]), 0);
                std::cout << c.real << std::endl;
            }
        }
        else if (std::regex_match(line, match, re_op))
        {
            //如果运算符左右两侧变量都已经定义，则运算
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
                std::cout << result.real << " + " << result.imag << "i" << std::endl;
            }
            //有未定义的变量，则提示
            else
            {
                std::cout << "NameError: name is not defined" << std::endl;
            }
        }
        else if (std::regex_match(line, match, re_mod))
        {
            //提取变量名，存在则运算
            std::string arg = match[1];
            if (vars.count(arg))
            {
                double mod = vars[arg].mod();
                std::cout << mod << std::endl;
            }
            //匹配是否是复数
            else if (std::regex_match(arg, match, re_num))
            {
                //检查正则表达式匹配结果中的第三个子匹配项 match[3] 是否成功匹配。如果成功匹配，
                // 就使用 std::stod(match[3]) 将其转换为 double 类型；否则，返回 0。
                Complex c(std::stod(match[1]), match[3].matched ? std::stod(match[3]) : 0);
                double mod = c.mod();
                std::cout << mod << std::endl;
            }
            //提示未定义信息
            else
            {
                std::cout << "NameError: name '" << arg << "' is not defined" << std::endl;
            }
        }
        else if (std::regex_match(line, match, re_con))
        {
            //提取变量名，匹配是否定义
            std::string arg = match[1];
            if (vars.count(arg))
            {
                Complex con = vars[arg].conjugate();
                std::cout << con.real << " + " << con.imag << "i" << std::endl;
            }
            else if (std::regex_match(arg, match, re_num))
            {
                //检查正则表达式匹配结果中的第三个子匹配项 match[3] 是否成功匹配。如果成功匹配，
                // 就使用 std::stod(match[3]) 将其转换为 double 类型；否则，返回 0。
                Complex c(std::stod(match[1]), match[3].matched ? std::stod(match[3]) : 0);
                Complex con = c.conjugate();
                std::cout << con.real << " + " << con.imag << "i" << std::endl;
            }
            //提示未定义
            else
            {
                std::cout << "NameError: name '" << arg << "' is not defined" << std::endl;
            }
        }
        else if (std::regex_match(line, match, re_mod_assign))
        {
            //匹配是否存在变量
            std::string arg = match[2];
            if (vars.count(arg))
            {
                double mod = vars[arg].mod();
                vars[match[1]] = Complex(mod, 0);
                std::cout << match[1] << " = " << vars[match[1]].real << std::endl;
            }
            //匹配是否是一个复数
            else if (std::regex_match(arg, match, re_num))
            {
                Complex c(std::stod(match[1]), match[3].matched ? std::stod(match[3]) : 0);
                double mod = c.mod();
                vars[match[1]] = Complex(mod, 0);
                std::cout << match[1] << " = " << vars[match[1]].real << std::endl;
            }
            //都不是则提示未定义
            else
            {
                std::cout << "NameError: name '" << arg << "' is not defined" << std::endl;
            }
        }
        else if (std::regex_match(line, match, re_con_assign))
        {
            //匹配是否存在变量
            std::string arg = match[2];
            if (vars.count(arg))
            {
                Complex con = vars[arg].conjugate();
                vars[match[1]] = con;
                std::cout << match[1] << " = " << vars[match[1]].real << " + " << vars[match[1]].imag << "i" << std::endl;
            }
            //匹配是否是一个复数
            else if (std::regex_match(arg, match, re_num))
            {
                Complex c(std::stod(match[1]), match[3].matched ? std::stod(match[3]) : 0);
                Complex con = c.conjugate();
                vars[match[1]] = con;
                std::cout << match[1] << " = " << vars[match[1]].real << " + " << vars[match[1]].imag << "i" << std::endl;
            }
            //都不是则提示未定义
            else
            {
                std::cout << "NameError: name '" << arg << "' is not defined" << std::endl;
            }
        }
        else if (std::regex_match(line, match, re_expr))
        {
            //一些存储的变量
            std::string expr = match[0];
            std::smatch match_num;
            std::regex re_num("(-?\\d+(\\.\\d+)?)([+-]-?\\d+(\\.\\d+)?)?i?");
            std::smatch match_var;
            std::regex re_var("([a-zA-Z]+)");
            Complex result;
            size_t pos = 0;

            //循环条件是正则表达式中还能找到"+"运算符
            while ((pos = expr.find('+')) != std::string::npos)
            {
                //pos是加号位置，提取两个加号之间的变量或复数
                std::string token = expr.substr(0, pos);
                //如果匹配一个复数形式，则进行加法运算
                if (std::regex_match(token, match_num, re_num))
                {
                    if (match_num[3].matched)
                    {
                        if (token.back() == 'i')
                        {
                            result = result + Complex(0, std::stod(match_num[1]));
                        }
                        else
                        {
                            result = result + Complex(std::stod(match_num[1]), std::stod(match_num[3]));
                        }
                    }
                    else
                    {
                        result = result + Complex(std::stod(match_num[1]), 0);
                    }
                }
                //如果匹配一个变量形式，则进行加法运算
                else if (std::regex_match(token, match_var, re_var))
                {
                    if (vars.count(match_var[1]))
                    {
                        result = result + vars[match_var[1]];
                    }
                    //否则提示有变量未定义
                    else
                    {
                        std::cout << "NameError: name '" << match_var[1] << "' is not defined" << std::endl;
                    }
                }
                //将已经处理的两个加号删除，处理下一个加法
                expr.erase(0, pos + 1);
            }
            //处理表达式最后一个数
            //匹配一个复数
            if (std::regex_match(expr, match_num, re_num))
            {
                if (match_num[3].matched)
                {
                    if (expr.back() == 'i')
                    {
                        result = result + Complex(0, std::stod(match_num[1]));
                    }
                    else
                    {
                        result = result + Complex(std::stod(match_num[1]), std::stod(match_num[3]));
                    }
                }
                else
                {
                    result = result + Complex(std::stod(match_num[1]), 0);
                }
            }
            //匹配一个变量
            else if (std::regex_match(expr, match_var, re_var))
            {
                if (vars.count(match_var[1]))
                {
                    result = result + vars[match_var[1]];
                }
                else
                {
                    std::cout << "NameError: name '" << match_var[1] << "' is not defined" << std::endl;
                }
            }
            std::cout << result.real << " + " << result.imag << "i" << std::endl;
        }
        else if (std::regex_match(line, match, re_expr2))
        {
            //一些用来存储的变量
            std::string expr = match[0];
            std::smatch match_num;
            std::regex re_num("(-?\\d+(\\.\\d+)?)([+-]-?\\d+(\\.\\d+)?)?i?");
            std::smatch match_var;
            std::regex re_var("([a-zA-Z]+)");
            Complex result;
            size_t pos = 0;

            // 处理第一个操作数
            std::string token = expr.substr(0, expr.find_first_of("+-"));
            //匹配一个数字
            if (std::regex_match(token, match_num, re_num))
            {
                if (match_num[3].matched)
                {
                    if (token.back() == 'i')
                    {
                        result = result + Complex(0, std::stod(match_num[1]));
                    }
                    else
                    {
                        result = result + Complex(std::stod(match_num[1]), std::stod(match_num[3]));
                    }
                }
                else
                {
                    result = result + Complex(std::stod(match_num[1]), 0);
                }
            }
            //匹配一个变量
            else if (std::regex_match(token, match_var, re_var))
            {
                if (vars.count(match_var[1]))
                {
                    result = result + vars[match_var[1]];
                }
                else
                {
                    std::cout << "NameError: name '" << match_var[1] << "' is not defined" << std::endl;

                }
            }
            //从 expr 中删除已处理的部分，即当前操作数及其后的 - 符号。
            expr.erase(0, token.size() + 1);
            // 处理后面的减法运算
            while ((pos = expr.find('-')) != std::string::npos)
            {
                token = expr.substr(0, pos);
                if (std::regex_match(token, match_num, re_num))
                {
                    if (match_num[3].matched)
                    {
                        if (token.back() == 'i')
                        {
                            result = result - Complex(0, std::stod(match_num[1]));
                        }
                        else
                        {
                            result = result - Complex(std::stod(match_num[1]), std::stod(match_num[3]));
                        }
                    }
                    else
                    {
                        result = result - Complex(std::stod(match_num[1]), 0);
                    }
                }
                else if (std::regex_match(token, match_var, re_var))
                {
                    if (vars.count(match_var[1]))
                    {
                        result = result - vars[match_var[1]];
                    }
                    else
                    {
                        std::cout << "NameError: name '" << match_var[1] << "' is not defined" << std::endl;
                    }
                }
                expr.erase(0, pos + 1);
            }
            // 处理最后一个操作数
            if (std::regex_match(expr, match_num, re_num))
            {
                if (match_num[3].matched)
                {
                    if (expr.back() == 'i')
                    {
                        result = result - Complex(0, std::stod(match_num[1]));
                    }
                    else
                    {
                        result = result - Complex(std::stod(match_num[1]), std::stod(match_num[3]));
                    }
                }
                else
                {
                    result = result - Complex(std::stod(match_num[1]), 0);
                }
            }
            else if (std::regex_match(expr, match_var, re_var))
            {
                if (vars.count(match_var[1]))
                {
                    result = result - vars[match_var[1]];
                }
                else
                {
                    std::cout << "NameError: name '" << match_var[1] << "' is not defined" << std::endl;

                }
            }
            std::cout << result.real << " + " << result.imag << "i" << std::endl;
        }
        else if (std::regex_match(line, match, re_expr_mul))
        {
            std::string expr = match[0];
            std::smatch match_num;
            std::regex re_num("(-?\\d+(\\.\\d+)?)([+-]-?\\d+(\\.\\d+)?)?i?");
            std::smatch match_var;
            std::regex re_var("([a-zA-Z]+)");
            // 初始化为1，因为乘法的单位元是1所以结果先初始化为1
            Complex result(1, 0); 
            size_t pos = 0;
            //查找 * 位置
            while ((pos = expr.find('*')) != std::string::npos)
            {
                std::string token = expr.substr(0, pos);
                if (std::regex_match(token, match_num, re_num))
                {
                    if (match_num[3].matched)
                    {
                        if (token.back() == 'i')
                        {
                            result = result * Complex(0, std::stod(match_num[1]));
                        }
                        else
                        {
                            result = result * Complex(std::stod(match_num[1]), std::stod(match_num[3]));
                        }
                    }
                    else
                    {
                        result = result * Complex(std::stod(match_num[1]), 0);
                    }
                }
                else if (std::regex_match(token, match_var, re_var))
                {
                    if (vars.count(match_var[1]))
                    {
                        result = result * vars[match_var[1]];
                    }
                    else
                    {
                        std::cout << "NameError: name '" << match_var[1] << "' is not defined" << std::endl;

                    }
                }
                expr.erase(0, pos + 1);
            }
            // 处理最后一个操作数
            if (std::regex_match(expr, match_num, re_num))
            {
                if (match_num[3].matched)
                {
                    if (expr.back() == 'i')
                    {
                        result = result * Complex(0, std::stod(match_num[1]));
                    }
                    else
                    {
                        result = result * Complex(std::stod(match_num[1]), std::stod(match_num[3]));
                    }
                }
                else
                {
                    result = result * Complex(std::stod(match_num[1]), 0);
                }
            }
            else if (std::regex_match(expr, match_var, re_var))
            {
                if (vars.count(match_var[1]))
                {
                    result = result * vars[match_var[1]];
                }
                else
                {
                    std::cout << "NameError: name '" << match_var[1] << "' is not defined" << std::endl;

                }
            }
            std::cout << result.real << " + " << result.imag << "i" << std::endl;
        }
        else if (std::regex_match(line, match, re_expr_div))
        {
            //一些存储变量
            std::string expr = match[0];
            std::smatch match_num;
            std::regex re_num("(-?\\d+(\\.\\d+)?)([+-]-?\\d+(\\.\\d+)?)?i?");
            std::smatch match_var;
            std::regex re_var("([a-zA-Z]+)");
            Complex result;
            size_t pos = 0;

            // 处理第一个操作数
            std::string token = expr.substr(0, expr.find_first_of("/"));
            if (std::regex_match(token, match_num, re_num))
            {
                if (match_num[3].matched)
                {
                    if (token.back() == 'i')
                    {
                        result = result + Complex(0, std::stod(match_num[1]));
                    }
                    else
                    {
                        result = result + Complex(std::stod(match_num[1]), std::stod(match_num[3]));
                    }
                }
                else
                {
                    result = result + Complex(std::stod(match_num[1]), 0);
                }
            }
            else if (std::regex_match(token, match_var, re_var))
            {
                if (vars.count(match_var[1]))
                {
                    result = result + vars[match_var[1]];
                }
                else
                {
                    std::cout << "NameError: name '" << match_var[1] << "' is not defined" << std::endl;

                }
            }
            expr.erase(0, token.size() + 1);

            // 处理后面的除法运算
            while ((pos = expr.find('/')) != std::string::npos)
            {
                token = expr.substr(0, pos);
                if (std::regex_match(token, match_num, re_num))
                {
                    if (match_num[3].matched)
                    {
                        if (token.back() == 'i')
                        {
                            result = result / Complex(0, std::stod(match_num[1]));
                        }
                        else
                        {
                            result = result / Complex(std::stod(match_num[1]), std::stod(match_num[3]));
                        }
                    }
                    else
                    {
                        result = result / Complex(std::stod(match_num[1]), 0);
                    }
                }
                else if (std::regex_match(token, match_var, re_var))
                {
                    if (vars.count(match_var[1]))
                    {
                        result = result / vars[match_var[1]];
                    }
                    else
                    {
                        std::cout << "NameError: name '" << match_var[1] << "' is not defined" << std::endl;

                    }
                }
                expr.erase(0, pos + 1);
            }

            // 处理最后一个操作数
            if (std::regex_match(expr, match_num, re_num))
            {
                if (match_num[3].matched)
                {
                    if (expr.back() == 'i')
                    {
                        result = result / Complex(0, std::stod(match_num[1]));
                    }
                    else
                    {
                        result = result / Complex(std::stod(match_num[1]), std::stod(match_num[3]));
                    }
                }
                else
                {
                    result = result / Complex(std::stod(match_num[1]), 0);
                }
            }
            else if (std::regex_match(expr, match_var, re_var))
            {
                if (vars.count(match_var[1]))
                {
                    result = result / vars[match_var[1]];
                }
                else
                {
                    std::cout << "NameError: name '" << match_var[1] << "' is not defined" << std::endl;
 
                }
            }
            std::cout << result.real << " + " << result.imag << "i" << std::endl;
        }
        //如果都不匹配则提示输入有误
        else
        {
            std::cout << "SyntaxError: invalid syntax." << std::endl;
        }
    }
};

int main()
{
    //创建一个无序映射（unordered_map），用于存储变量名和对应的复数值
    std::unordered_map<std::string, Complex> vars;
    std::string line;
    //循环读入命令行的一整行内容
    while (std::getline(std::cin, line))
    {
        
        if (line == "exit")
        {
            break;
        }
        //去除命令行内容中的空格符号
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        //执行运算操作
        Complex::parseAndStore(line, vars);
        std::cout << ">>>";
    }
    return 0;
}