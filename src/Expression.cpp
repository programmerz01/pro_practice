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
    else if(type == ExpType::EQUAL){
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