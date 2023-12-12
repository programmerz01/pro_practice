#include "../include/Func.h"
#include <sstream>

Func::Func(){}
Func::Func(std::string name){
    this->name = name;
}

std::string Func::get_name()
{
    return this->name;
}

void Func::set_name(std::string name)
{
    this->name = name;
}

void Func::add_expression(Expression *e)
{
    this->exps.push_back(e);
}

std::vector<Expression*> Func::get_expressions()
{
    return this->exps;
}

std::string Func::toString()
{
    std::ostringstream oss;

    oss << "func " << this->name << ":" << std::endl;
    for(auto it = this->exps.begin(); it != this->exps.end(); it++){
        oss << "\t" << (*it)->toString() << std::endl;
    }
    return oss.str();
}