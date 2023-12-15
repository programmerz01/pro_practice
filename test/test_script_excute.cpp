#include "../include/Expression.h"
#include "../include/Environment.h"
#include "../include/Interpreter.h"
#include <cassert>
#include <sstream>


int main()
{
    Interpreter *pi = new Interpreter();
    std::string fp = "../data/script_test_1.txt";
    if(fp.empty())
    {
        std::cerr << "Error: file path is empty" << '\n';
        return 0;
    }

    pi->init(fp);
    pi->execute();

    return 0;
}