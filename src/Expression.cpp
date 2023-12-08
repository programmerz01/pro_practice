#include "../include/Expression.h"
#include <regex>
#include <sstream>
#include <iostream>
#include <string>

std::string getSubstringInParentheses(const std::string &input)
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

Expression::Expression(ExpType type)
{
    this->type = type;
}

// 从字符串中解析出表达式
bool Expression::parse(std::string s, Expression *&exp)
{
    std::istringstream iss(s);
    std::string word;

    iss >> word;
    ExpType type = string_to_type(word);

    // 处理运算符
    if(type == ExpType::ADD || type == ExpType::SUB||
        type == ExpType::MUL || type == ExpType::DIV||
        type == ExpType::LET){
        exp = new Expression(type);
        exp->argc = 2;
        iss >> word;
        exp->arg1 = word;
        iss >> word;
        exp->arg2 = word;
        return true;
    }
    // 处理get、reply，其参数为若干字符串及变量拼接
    else if(type == ExpType::GET || type == ExpType::RESPONSE){
        exp = new Expression(type);
        exp->argc = 1;
        iss >> word;
        exp->arg1 = word;
        return true;
    }
    // 处理call
    else if(type == ExpType::CALL){
        exp = new Expression(type);
        exp->argc = 1;
        iss >> word;
        exp->arg1 = word;
        return true;
    }
    // 处理equal, 参数为两个变量以及一个表达式
    else if(type == ExpType::IF_EQUAL){
        exp = new Expression(type);
        exp->argc = 3;
        iss >> word;
        exp->arg1 = word;
        iss >> word;
        exp->arg2 = word;

        // 获取括号内的表达式
        std::string sub = getSubstringInParentheses(s);
        Expression* sub_exp;
        if(!Expression::parse(sub, sub_exp)){
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
    if(type_str == "add")
        return ExpType::ADD;
    else if(type_str == "sub")
        return ExpType::SUB;
    else if(type_str == "mul")
        return ExpType::MUL;
    else if(type_str == "div")
        return ExpType::DIV;
    else if(type_str == "equal")
        return ExpType::IF_EQUAL;
    else if(type_str == "get")
        return ExpType::GET;
    else if(type_str == "let")
        return ExpType::LET;
    else if(type_str == "reply")
        return ExpType::RESPONSE;
    else if(type_str == "call")
        return ExpType::CALL;
    else
        return ExpType::ADD;
}

std::string Expression::toString(){
    std::ostringstream oss;
    oss << type_to_string(this->type) << " ";
    if(this->argc == 1)
        oss << this->arg1;
    else if(this->argc == 2)
        oss << this->arg1 << " " << this->arg2;
    else if(this->argc == 3)
        oss << this->arg1 << " " << this->arg2 << " " << this->arg3;
    return oss.str();

}
