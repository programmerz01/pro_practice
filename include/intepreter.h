#ifndef INTERPRETER_H
#define INTERPRETER_H

/*
方法类，用于实现解释器脚本解析、执行
*/
class Interpreter {
};

/* 语句的基类，解释器执行的基本单位，有原语，函数两种类型 */
class Expression
{
public:
    /*虚函数，为语句（函数，原语）执行的模板*/
    virtual void execute() = 0; 

};

/* 执行语句的环境，包含堆栈，全局变量，所有函数[，与客户端的连接，用于通信[,多线程相关]] */
class environment
{
};

#endif