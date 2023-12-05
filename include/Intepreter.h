#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <map>
#include <iostream>
#include <set>
#include <stack>
#include "Expression.h" 



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
    std::map<std::string, Expression> global_func;
public:
    Environment() = default;
    bool add_func(Func *func);
    bool add_valuable(std::string name, int value);
    bool add_valuable(std::string name, double value);
    bool add_valuable(std::string value);
};

/*
方法类，实现最终的解释器脚本解析、执行
*/
class Interpreter {
private:
    Environment* e;
public:
    Interpreter();
    ~Interpreter();
    /* 读取脚本 */
    bool get_script(std::string filename);
    /* 执行脚本 */
    void execute();
private:
    /* 接受文件流及函数名，并在输入流中读取函数定义，最后返回函数 */
    Func* get_one_func(std::istream &iss, std::string func_name);
    /* 处理一行表达式，将解析出的表达式记录在exp中，成功识别返回true否则为false */
    bool handle_one_line(std::string s, Expression* &exp);
};
#endif