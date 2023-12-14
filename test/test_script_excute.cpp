#include "../include/Expression.h"
#include "../include/Environment.h"
#include "../include/Interpreter.h"
#include <cassert>
#include <sstream>


int main()
{

    // test_execute_func_operator();
    // test_execute_interaction_func();
    {
    }
    try{

    }
    catch (std::out_of_range const &e)
    {
        // 如果转换的结果超出了double的范围，处理错误
        std::cerr << p.toString() << " Error : std::out_of_range thrown" << '\n';
        return;
    }
    catch (std::invalid_argument const &e)
    {
        // 如果除数为0，处理错误
        std::cerr << p.toString() << " Error:" << e.what() << '\n';
        return;
    }
    }

    return 0;
}