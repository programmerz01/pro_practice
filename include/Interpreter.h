#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Environment.h"

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
    /* 展示已有的函数 */
    void show_funcs();
private:
    /* 接受文件流及函数名，并在输入流中读取函数定义，最后返回函数 */
    Func* get_one_func(std::istream &iss, std::string func_name);
    /* 处理一行表达式，将解析出的表达式记录在exp中，成功识别返回true否则为false */
    bool handle_one_line(std::string s, Expression* &exp);
};
#endif