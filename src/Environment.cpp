#include "../include/Environment.h"


Environment::Environment()
{
    // 初始化全局变量
    global_int = std::map<std::string, int>();
    global_real = std::map<std::string, double>();
    global_string = std::set<std::string>();
    // 初始化函数
    global_func = std::map<std::string, Func*>();
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