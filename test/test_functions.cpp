#include "../include/Expression.h"
#include "../include/Environment.h"
#include "../include/Interpreter.h"
#include <cassert>
#include <sstream>

void test_execute_call()
{
    // Create an Expression object
    Expression* exp = new Expression(Expression::ExpType::CALL, "func1");
    Expression **exps = new Expression *[5];
    exps[0] = new Expression(Expression::ExpType::RESPONSE, "123");
    exps[1] = new Expression(Expression::ExpType::IF_EQUAL, "123", "213", exp);
    exps[2] = new Expression(Expression::ExpType::ADD, "$x", "$z");
    exps[3] = new Expression(Expression::ExpType::LET, "$x", "$z");
    exps[4] = new Expression(Expression::ExpType::GET_STRING, "z");

    // Create an Environment object
    Environment e;
    e.init();

    // Create a function and add it to the environment
    Func func1("func1");
    for (int i = 0; i < 5; i++)
    {
        func1.add_expression(exps[i]);
    }
    e.add_func(&func1);

    // Call the execute_call function
    exp->execute(e);

    // Check the result
    Expression *res;
    for (int i = 0; i < 5; i++)
    {
        e.pop(res);
        assert(res->toString() == exps[4 - i]->toString());
    }
    assert(e.pop(res) == false);
}

void test_execute_call_invalid_argument(std::ostringstream &err_output)
{
    // Create an Expression object
    Expression exp(Expression::ExpType::CALL, "func2");

    // Create an Environment object
    Environment e;
    e.init();
    err_output.str("");
    err_output.clear();

    // Call the execute_call function
    exp.execute(e);

    // Check the result
    std::string res = err_output.str();
    assert(res == exp.toString() + " Error:calling function not found func2\n");

    // 清空oss的内容
    err_output.str("");
    err_output.clear();
}

int main()
{
    /* 重定向以便进行测试 */
    std::ostringstream output, err_output;
    std::streambuf *oldCoutbuf = std::cout.rdbuf(output.rdbuf());
    std::streambuf *oldCerrbuf = std::cerr.rdbuf(err_output.rdbuf());

    test_execute_call();
    test_execute_call_invalid_argument(err_output);

    /* 恢复重定向 */
    std::cout.rdbuf(oldCoutbuf);
    std::cerr.rdbuf(oldCerrbuf);

    return 0;
}