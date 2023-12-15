#include <sstream>
#include <fstream>
#include "../include/Interpreter.h"
#include "../include/Func.h"


Interpreter::Interpreter()
{
    e = new Environment();
}

Interpreter::~Interpreter()
{
    delete e;
}

bool Interpreter::init(std::string filename)
{
    if(get_script(filename))
    {
        e->init();
        return true;
    }
    return false;
}

// 开始执行脚本，从main函数开始
void Interpreter::execute()
{
    Expression *exp;

    try
    {
        while(e->pop(exp)){
            exp->execute(*e);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return;
    }

    std::cout << "execute finished" << std::endl;
}

// 从文件中读取脚本
bool Interpreter::get_script(std::string filename)
{
    std::ifstream file(filename);
    Func *func;
    if (!file.is_open())
    {
        std::cerr << "can't open file:" << filename << std::endl;
        exit(1);
    }

    // 不断寻找函数定义，寻找成功则获取该函数 不能出现函数定义以外的语句
    std::string line;
    while(getline(file, line)){
        if(line.empty() || line[0] == '#') // 空行或注释
            continue;

        // 该行不为函数定义,重复读取直至找到下一个函数定义
        if(line.substr(0, 4) != "func"){
            std::cerr << "error:no function belonged" << line << std::endl;
        }
        else{
            // 获取函数定义并记录
            std::string name = line.substr(5, line.size()-5);
            func = get_one_func(file, name);
            e->add_func(func);
        }

    }
    return true;
}

// 获取一个函数的定义
Func* Interpreter::get_one_func(std::istream &iss, std::string name_line)
{
    Func *func = new Func();
    Expression *exp;
    // 获取每一行的表达式
    std::string line;
    while(getline(iss, line)){
        if(line == "endf")// 函数定义结束
            break;
        if(line.empty()) // 空行
            continue;

        if(handle_one_line(line, exp)){ 
            func->add_expression(exp);
        }
    }
    func->set_name(name_line);
    return func;
}

// 获取一个函数的一行语句定义
bool Interpreter::handle_one_line(std::string s, Expression* &exp)
{
    // 处理行首空格或制表符
    size_t pos = s.find_first_not_of(" \t");
    if (pos != std::string::npos)
        s = s.substr(pos); // 截取非空开始的子串
    else  // 字符串均为空格或制表符，解析失败
        return false;
    // 注释
    if(s[0] == '#')
        return false;
    
    std::istringstream iss(s);
    std::string first_word; 
    iss >> first_word;

    // 将处理过的字符串传入解析函数
    return Expression::parse(s, exp);
}

void Interpreter::show_funcs(std::ostream &oss)
{
    oss << "All functions:" << std::endl;
    std::map<std::string, Func*> funcs;
    e->get_funcs(funcs);

    for(auto it = funcs.begin(); it != funcs.end(); it++){
        oss << (it->second)->toString() << std::endl;
    }
}
