#include "../include/Expression.h"
#include "../include/Environment.h"
#include "../include/Interpreter.h"
#include <cassert>
#include <sstream>

// 改进：
// 将Interpreter类中的init函数改为构造函数，将execute函数改为析构函数
// 命令行参数：脚本文件路径、用户数据、多文件读取函数
// 优化if equal，添加 if not equal
int main()
{
    Interpreter *pi = new Interpreter();
    std::string fp = "../data/script1.txt";

    if (fp.empty())
    {
        std::cerr << "Error: file path is empty" << '\n';
        return 0;
    }

    pi->init(fp);
    pi->execute();

    return 0;
}