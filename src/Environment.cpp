#include "../include/Environment.h"


Environment::Environment()
{
    // 初始化全局变量
    global_real = std::map<std::string, double>();
    global_string = std::map<std::string, std::string>();
    // 初始化函数
    global_func = std::map<std::string, Func*>();
}

void Environment::init()
{
    global_real["_func_return"] = 0;// 函数返回值
    global_real["_exp_return"] = 0;// 语句返回值
    global_string["_get"] = "";// 用于获取用户输入
    global_real["__equal_epsilon"] = 1e-6;// 用于判断两个double是否相等
    
    // main函数为入口函数
    Func *func;
    std::vector<Expression*> exps = func -> get_expressions();
    if (this->get_one_func("main", func))
    {
        for(auto it = exps.begin(); it != exps.end(); it++){
            this->push(*it);
        }
    }

}

bool Environment::add_func(Func *func)
{
    std::string name = func->get_name();
    if(global_func.find(name) != global_func.end()){
        std::cerr << "error: function " << name << " has been defined" << std::endl;
        return false;
    }
    global_func[name] = func;
    return true;
}

bool Environment::add_valuable(std::string name, double value)
{
    if(global_real.find(name) != global_real.end()){
        return false;
    }
    global_real[name] = value;
    return true;
}

bool Environment::add_valuable(std::string name, std::string value)
{
    if(global_string.find(name) != global_string.end()){
        return false;
    }
    global_string[name] = value;
    return true;
}

bool Environment::set_real(std::string name, double value)
{
    if(global_real.find(name) == global_real.end()){
        return false;
    }
    global_real[name] = value;
    return true;
}

bool Environment::set_string(std::string name, std::string value)
{
    if(global_string.find(name) == global_string.end()){
        return false;
    }
    global_string[name] = value;
    return true;
}

bool Environment::get_real(std::string name, double &value)
{
    if(global_real.find(name) == global_real.end()){
        return false;
    }
    value = global_real[name];
    return true;
}

bool Environment::get_string(std::string name, std::string &value)
{
    if(global_string.find(name) == global_string.end()){
        return false;
    }
    value = global_string[name];
    return true;
}

bool Environment::get_one_func(std::string name, Func *&func)
{
    if(global_func.find(name) == global_func.end()){
        std::cerr << "error: function " << name << " not found" << std::endl;
        return false;
    }
    func = global_func[name];
    return true;
}

bool Environment::get_funcs(std::map<std::string, Func*> &funcs)
{
    funcs = global_func;
    return true;
}

bool Environment::push(Expression *exp)
{
    stack.push(exp);
    return true;
}

bool Environment::pop(Expression *&exp)
{
    if(stack.empty()){
        return false;
    }
    exp = stack.top();
    stack.pop();
    return true;
}