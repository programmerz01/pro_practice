#include "../include/Expression.h"
#include "../include/Environment.h"
#include <regex>
#include <sstream>
#include <iostream>
#include <string>
#include <variant>

// 解析解析参数并获取其值
static std::variant<double, std::string> get_value(std::string arg, Environment &e) noexcept
{
    double value_real;
    std::string value_str;
    if (arg[0] == '$')
    {
        if (e.get_real(arg.substr(1, arg.size() - 1), value_real) )
        {
            return value_real;
        }
        if (e.get_string(value_str, arg))
        {
            return value_str;
        }
        throw std::invalid_argument("argunment not found");
    }
    else // 进行字面量转换
    {
        if(arg[0] == '\'')
        {
            value_str = arg.substr(1, arg.size() - 2);
            return value_str;
        }
        value_real = std::stod(arg);
        return value_real;
    }
}

// 比较两个参数
static bool campare_variants(const std::variant<double, std::string> value1,const std::variant<double, std::string> value2, Environment &e)
{
    // 如果两个std::variant中的类型不同，它们不相等。
    if (value1.index() != value2.index())
    {
        return false; 
    }
    else
    {
        if (std::holds_alternative<double>(value1))
        {
            double v1 = std::get<double>(value1);
            double v2 = std::get<double>(value2);
            double epsilon;
            e.get_real("__equal_epsilon", epsilon);
            return std::abs(v1 - v2) < epsilon; 
        }
        else
        {
            std::string str1 = std::get<std::string>(value1);
            std::string str2 = std::get<std::string>(value2);
            return str1 == str2; // 如果两个std::string相等，它们相等。
        }
    }
}

static std::string getSubstringInParentheses(const std::string &input)
{
    std::string result;
    size_t startPos = input.find('(');
    if (startPos != std::string::npos)
    {
        size_t endPos = input.find(')', startPos);
        if (endPos != std::string::npos)
        {
            startPos++; // 移动到括号内部的起始位置
            result = input.substr(startPos, endPos - startPos);
        }
    }
    return result;
}

// 几种默认的构造函数
Expression::Expression(ExpType type)
{
    this->type = type;
}

Expression::Expression(ExpType type, std::string arg1)
{
    this->type = type;
    this->arg1 = arg1;
    this->argc = 1;
}

Expression::Expression(ExpType type, std::string arg1, std::string arg2)
{
    this->type = type;
    this->arg1 = arg1;
    this->arg2 = arg2;
    this->argc = 2;
}

Expression::Expression(ExpType type, std::string arg1, Expression *arg3)
{
    this->type = type;
    this->arg1 = arg1;
    this->arg3 = arg3;
    this->argc = 3;
}

// 从字符串中解析出表达式
bool Expression::parse(std::string s, Expression *&exp)
{
    std::istringstream iss(s);
    std::string word;

    iss >> word;
    ExpType type = string_to_type(word);

    // 处理运算符
    if (type == ExpType::ADD || type == ExpType::SUB ||
        type == ExpType::MUL || type == ExpType::DIV ||
        type == ExpType::LET)
    {
        exp = new Expression(type);
        exp->argc = 2;
        iss >> word;
        exp->arg1 = word;
        iss >> word;
        exp->arg2 = word;
        return true;
    }
    // 处理get、reply，其参数为若干字符串及变量拼接
    else if (type == ExpType::GET || type == ExpType::RESPONSE)
    {
        exp = new Expression(type);
        exp->argc = 1;
        iss >> word;
        exp->arg1 = word;
        return true;
    }
    // 处理call
    else if (type == ExpType::CALL)
    {
        exp = new Expression(type);
        exp->argc = 1;
        iss >> word;
        exp->arg1 = word;
        return true;
    }
    // 处理equal, 参数为两个变量以及一个表达式
    else if (type == ExpType::IF_EQUAL)
    {
        exp = new Expression(type);
        exp->argc = 3;
        iss >> word;
        exp->arg1 = word;
        iss >> word;
        exp->arg2 = word;

        // 获取括号内的表达式
        std::string sub = getSubstringInParentheses(s);
        Expression *sub_exp;
        if (!Expression::parse(sub, sub_exp))
        {
            return false;
        }
        exp->arg3 = sub_exp;

        return true;
    }
}

std::string Expression::type_to_string(ExpType type)
{
    switch (type)
    {
    case ExpType::ADD:
        return "add";
    case ExpType::SUB:
        return "sub";
    case ExpType::MUL:
        return "mul";
    case ExpType::DIV:
        return "div";
    case ExpType::IF_EQUAL:
        return "equal";
    case ExpType::GET:
        return "get";
    case ExpType::LET:
        return "let";
    case ExpType::RESPONSE:
        return "reply";
    case ExpType::CALL:
        return "call";
    default:
        return "unknown";
    }
}

Expression::ExpType Expression::string_to_type(std::string type_str)
{
    if (type_str == "add")
        return ExpType::ADD;
    else if (type_str == "sub")
        return ExpType::SUB;
    else if (type_str == "mul")
        return ExpType::MUL;
    else if (type_str == "div")
        return ExpType::DIV;
    else if (type_str == "if_equal")
        return ExpType::IF_EQUAL;
    else if (type_str == "get")
        return ExpType::GET;
    else if (type_str == "let")
        return ExpType::LET;
    else if (type_str == "reply")
        return ExpType::RESPONSE;
    else if (type_str == "call")
        return ExpType::CALL;
    else
        return ExpType::ADD;
}

std::string Expression::toString()
{
    std::ostringstream oss;
    oss << type_to_string(this->type) << " ";
    if (this->argc == 1)
        oss << this->arg1;
    else if (this->argc == 2)
        oss << this->arg1 << " " << this->arg2;
    else if (this->argc == 3)
        oss << this->arg1 << " " << this->arg2 << " " << this->arg3;
    return oss.str();
}

/* 语句执行部分 */
// 对于运算，将结果存储在第一个操作数中
void Expression::execute(Environment &e)
{
    switch (this->type)
    {
    case ExpType::ADD:
    case ExpType::SUB:
    case ExpType::MUL:
    case ExpType::DIV:
        execute_operator(*this, e);
        break;
    case ExpType::IF_EQUAL:
        execute_if_equal(*this, e);
        break;
    case ExpType::GET:
        execute_get(*this, e);
        break;
    case ExpType::LET:
        execute_let(*this, e);
        break;
    case ExpType::RESPONSE:
        execute_reply(*this, e);
        break;
    case ExpType::CALL:
        execute_call(*this, e);
        break;
    default:
        std::cerr << "unknown expression type" << std::endl;
        break;
    }
}

// 对变量的运算语句
void Expression::execute_operator(Expression p, Environment &e)
{
    std::string arg_name = p.arg1.substr(1, p.arg1.size() - 1);
    double value_real, result, value_real2;
    std::variant<double, std::string> value1, value2;
    try
    {
        value1 = get_value(p.arg1, e);
        value2 = get_value(p.arg2, e);
        if(!std::holds_alternative<double>(value1) || !std::holds_alternative<double>(value2))
        {
            throw std::invalid_argument("Operator don't support string");
        }
        value_real = std::get<double>(value1);
        value_real2 = std::get<double>(value2);

        // 成功找出两个参数值
        if (p.type == ExpType::ADD)
            result = value_real + value_real2;
        else if (p.type == ExpType::SUB)
            result = value_real - value_real2;
        else if (p.type == ExpType::MUL)
            result = value_real * value_real2;
        else if (p.type == ExpType::DIV)
        {
            if (value_real2 == 0)
                throw std::invalid_argument("Divide by zero");
            result = value_real / value_real2;
        }
        e.set_real(arg_name, result);
    }
    catch (std::out_of_range const &e)
    {
        // 如果转换的结果超出了double的范围，处理错误
        std::cerr << "Error integer overflow: std::out_of_range thrown" << '\n';
        return;
    }
    catch (std::invalid_argument const &e)
    {
        // 如果除数为0，处理错误
        std::cerr << "Error:" << e.what() << '\n';
        return;
    }
}


// 对于if_equal语句,能够比较字符串和数字
void Expression::execute_if_equal(Expression p, Environment &e)
{
    std::variant<double, std::string> value1, value2;
    try
    {
        // 参数比较时，可能为字符串比较，也可能为数字比较
        // 参数可能为变量引用，也可能为字面量比较
        value1 = get_value(p.arg1, e);
        value2 = get_value(p.arg2, e);

        // 比较两个参数
        if(campare_variants(value1, value2, e)){
            p.arg3->execute(e);
        }
    }
    catch (std::invalid_argument const &e)
    {
        // 如果除数为0，处理错误
        std::cerr << "Error:" << e.what() << '\n';
        return;
    }
    catch (std::bad_variant_access const &e)
    {
        // 比较错误
        std::cerr << "Error:" << e.what() << '\n';
        return;
    }
}

// 对于get语句，获取用户输入，存储在变量中，变量为字符串
void Expression::execute_get(Expression p, Environment &e)
{
    std::string get;
    std::cin>>get;
    if(!e.set_string(p.arg1, get)){
        std::cerr << "Error: variable " << p.arg1 << " not found" << std::endl;
    }
}


// 处理reply语句，将+分隔的变量及字符串拼接输出
void Expression::execute_reply(Expression p, Environment &e)
{
    std::ostringstream oss;
    std::istringstream iss(p.arg1);
    std::string sub_arg;
    try
    {
        while(std::getline(iss, sub_arg, '+')){
            std::variant<double, std::string> value;
            value = get_value(sub_arg, e);
            if(std::holds_alternative<double>(value)){
                oss << std::get<double>(value);
            }
            else{
                oss << std::get<std::string>(value);
            }
        }
    }
    catch (std::invalid_argument const &e)
    {
        // 比较错误
        std::cerr << "Error:" << e.what() << '\n';
        return;
    }
    std::cout<<oss.str()<<std::endl;
}

// 处理let语句，将变量存储在环境中
void Expression::execute_let(Expression p, Environment &e)
{
    std::string str_value;
    double real_value;
    std::variant<double, std::string> value;

    try{
        value = get_value(p.arg2, e);

        // 设置字符串
        if(e.get_string(str_value, p.arg1)){
            e.set_string(p.arg1, std::get<std::string>(value));
        }
        // 设置实数
        else if(e.get_real(p.arg1, real_value)){
            e.set_real(p.arg1, std::get<double>(value));
        }
        // 新建变量
        else{
            if(std::holds_alternative<double>(value)){
                e.add_valuable(p.arg1, std::get<double>(value));
            }
            else{
                e.add_valuable(p.arg1, std::get<std::string>(value));
            }
        }
    }
    catch (std::invalid_argument const &e)
    {
        // 参数错误
        std::cerr << "Error:" << e.what() << '\n';
        return;
    }
    catch (std::bad_variant_access const &e)
    {
        // 类型错误
        std::cerr << "Error:" << e.what() << '\n';
        return;
    }
    catch(std::out_of_range const &e){
        // 超出范围
        std::cerr << "Error:" << e.what() << '\n';
        return;
    }
}

// 处理call语句，调用函数
void Expression::execute_call(Expression p, Environment &e)
{
    Func *func;
    if(!e.get_one_func(p.arg1, func)){
        std::cerr << "Error: function " << p.arg1 << " not found" << std::endl;
        return;
    }
    std::vector<Expression*> exps = func->get_expressions();
    for(auto it = exps.begin(); it != exps.end(); it++){
        e.push(*it);
    }
}