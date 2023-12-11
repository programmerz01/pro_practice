#include <iostream>
#include <fstream>
#include "../include/Interpreter.h"

int main() {
    Interpreter *pi = new Interpreter();
    std::string filename = "../data/demo_script.txt";
    
    pi->init(filename);
    pi->show_funcs();
    return 0;
} 