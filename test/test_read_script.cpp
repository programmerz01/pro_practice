#include "../include/Interpreter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

int main() {
    Interpreter *pi = new Interpreter();
    std::string filename = "../data/demo_script.txt";
    std::ostringstream oss;

    pi->init(filename);
    pi->show_funcs(oss);
    std::string res = oss.str();

    assert(res == "All functions:\nfunc 123:\n\tlet age 1\n\tlet name \"mc987+987\"\n\nfunc main:\n\tlet age 1\n\tlet name \"65488\"\n\tcall 123\n\n");
    return 0;
} 