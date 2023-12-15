#include "../include/Interpreter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

void test_file_read(std::string filename, std::string expected_res)
{
    Interpreter *pi = new Interpreter();
    std::ostringstream oss;

    try
    {
        pi->init(filename);
        pi->show_funcs(oss);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        assert(false);
    }
    
    std::string res = oss.str();
    std::cout << res << std::endl;
    assert(res ==expected_res);
}

static std::string readFileToString(const std::string &filename)
{
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    std::string answer1 = readFileToString("../data/expect_out_demo1.txt");
    std::string answer2 = readFileToString("../data/expect_out_demo2.txt");

    // test_file_read("../data/demo_script.txt", answer1);
    test_file_read("../data/demo_script2.txt",answer2);

    return 0;
} 