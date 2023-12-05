#pragma once
#include <iostream>
#include <variant>
#include <vector> 

class Environment;

/* 函数类 */
class Func 
{
private:
    /* 函数中含有若干原语，并支持函数嵌套调用 */
    std::vector<Expression*> exps;
public:
    Func();
    void add_expression(Expression* e);
    std::vector<Expression*> get_expressions();
};


/* 表达式，脚本语言基本的执行语句 */
/* 
type: add/sub/mul/div/equal, get, let, reply, equal v1 v2 (expressoin) , call func_name 
*/
class Expression { 
public:
    enum ExpType {ADD, SUB, MUL, DIV, EQUAL, GET, LET, RESPONSE, CALL};
private:
    /* 表达式类型 */
    ExpType type;
    /* 表达式参数个数 */
    int argc; 
    /* 表达式可能的参数 */
    std::variant<std::string, int, double, Expression*> arg1, arg2, arg3;

public:
    Expression(Expression::ExpType type);


    /* 原语的执行 */
    void execute(Environment &e);
    
    /* expression对应的执行过程 */
    static void execute_add(Expression p, Environment &e);
    static void execute_sub(Expression p, Environment &e);
    static void execute_mul(Expression p, Environment &e);
    static void execute_div(Expression p, Environment &e);
    static void execute_get(Expression p, Environment &e);
    static void execute_let(Expression p, Environment &e);
    static void execute_reply(Expression p, Environment &e);
    static void execute_if(Expression p, Environment &e);
    static void execute_call(Expression p, Environment &e);

    /* 从字符串中解析出表达式, 解析成功返回true，否则为false */
    static bool parse(std::string s, Expression* &exp);

    static std::string type_to_string(ExpType type);
    static ExpType string_to_type(std::string type_str);
};