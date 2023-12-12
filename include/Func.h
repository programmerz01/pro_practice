#pragma once
#include "Expression.h"

/* 函数类 */
class Func
{
private:
    std::string name;
    /* 函数中含有若干原语，并支持函数嵌套调用 */
    std::vector<Expression *> exps;

public:
    Func();
    Func(std::string name);

    void add_expression(Expression *e);
    std::vector<Expression *> get_expressions();
    void set_name(std::string name);
    std::string get_name();

    std::string toString();
};
