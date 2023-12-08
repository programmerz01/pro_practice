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
    std::map<std::string, int> global_int;
    std::map<std::string, double> global_real;
    std::set<std::string> global_string;
    /* 堆栈 */
    std::stack<Expression*> stack;
    /* 函数 */
    std::map<std::string, Func*> global_func;
public:
    Environment();
    bool add_func(Func *func);
    bool add_valuable(std::string name, int value);
    bool add_valuable(std::string name, double value);
    bool add_valuable(std::string value);

    bool get_valuable_int(std::string name, int &value);
    bool get_valuable_real(std::string name, double &value);
    bool get_valuable_string(std::string name, std::string &value);
    bool get_one_func(std::string name, Func* &exp);
    bool get_funcs(std::map<std::string, Func*> &funcs);
};
