#pragma once

#include <map>
#include <iostream>
#include <set>
#include <stack>
#include "Expression.h" 
#include "Func.h"


/* 执行语句的环境，包含堆栈，全局变量，所有函数[，与客户端的连接，用于通信[,多线程相关]] */
class Environment
{
private:
    /* 全局变量存储区 */
    std::map<std::string, double> global_real;
    std::map<std::string, std::string> global_string;
    /* 堆栈 */
    std::stack<Expression*> stack;
    /* 函数 */
    std::map<std::string, Func*> global_func;
public:
    Environment();
    /* 对环境中全局变量的所有操作 */
    // 初始化操作，定义一些常用的内置全局变量
    void init();

    // 添加全局变量
    bool add_func(Func *func);
    bool add_valuable(std::string name, double value);
    bool add_valuable(std::string name, std::string value);

    // 将全局变量设定为指定值
    bool set_valuable(std::string name, double value);
    bool set_valuable(std::string name, std::string value);
    bool set_default_precision(int precision);

    // 获取全局变量
    bool get_valuable(std::string name, double &value);
    bool get_valuable(std::string name, std::string &value);
    bool get_one_func(std::string name, Func* &exp);
    bool get_funcs(std::map<std::string, Func*> &funcs);
    int get_default_precision();

    bool push(Expression *exp);
    bool pop(Expression *&exp);
};
