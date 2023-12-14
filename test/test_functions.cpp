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
        assert(res->toString() == exps[i]->toString());
    }
    assert(e.pop(res) == false);

    // free
    delete exp;
    for (int i = 0; i < 5; i++)
    {
        delete exps[i];
    }
}

void test_execute_call_invalid_argument()
{
    // Create an Expression object
    Expression exp(Expression::ExpType::CALL, "func2");

    // Create an Environment object
    Environment e;
    e.init();

    try
    {
        exp.execute(e);
    }
    catch(const std::exception& e)
    {
    // Check the result
        std::string res = e.what();
        assert(res == "Error: " + exp.toString() + " calling function not found " + exp.get_arg1());
    }
    
}

void test_execute_func_operator()
{
    // Create an Expression object
    Expression* exp = new Expression(Expression::ExpType::CALL, "func1");
    Expression **exps = new Expression *[5];
    exps[4] = new Expression(Expression::ExpType::ADD, "$x", "$x");
    exps[0] = new Expression(Expression::ExpType::LET, "x", "123");
    exps[1] = new Expression(Expression::ExpType::LET, "y", "123");
    exps[2] = new Expression(Expression::ExpType::IF_EQUAL, "$x", "$y", exps[4]);
    exps[3] = new Expression(Expression::ExpType::LET, "z", "$x");

    // Create an Environment object
    Environment e;
    e.init();

    // Create a function and add it to the environment
    Func func1("func1");
    for (int i = 0; i < 4; i++)
    {
        func1.add_expression(exps[i]);
    }
    e.add_func(&func1);

    // Call the execute_call function
    exp->execute(e);

    // Check the result
    Expression *res;
    while(e.pop(res))
    {
        res->execute(e);
    }
    double z_value;
    e.get_valuable("z", z_value);
    assert(std::abs(z_value - 246) < 1e-6);

    // assert(e.pop(res) == false);
    // free
    delete exp;
    for (int i = 0; i < 5; i++)
    {
        delete exps[i];
    }
}

void test_execute_interaction_func()
{
    // 重定向输入
    std::istringstream input("123\n123\n");
    std::streambuf *oldCinbuf = std::cin.rdbuf(input.rdbuf());

    // Create an Expression object
    Expression* exp = new Expression(Expression::ExpType::CALL, "func1");
    Expression **exps = new Expression *[6];
    exps[4] = new Expression(Expression::ExpType::LET, "z", "456");
    exps[5] = new Expression(Expression::ExpType::LET, "m", "123");

    exps[0] = new Expression(Expression::ExpType::GET_STRING, "x");
    exps[1] = new Expression(Expression::ExpType::GET_STRING, "y");
    exps[2] = new Expression(Expression::ExpType::IF_EQUAL, "$x", "$y", exps[4]);
    exps[3] = new Expression(Expression::ExpType::IF_EQUAL, "$x", "789", exps[5]);

    // Create an Environment object
    Environment e;
    e.init();

    // Create a function and add it to the environment
    Func func1("func1");
    for (int i = 0; i < 4; i++)
    {
        func1.add_expression(exps[i]);
    }
    e.add_func(&func1);

    // Call the execute_call function
    exp->execute(e);

    // Check the result
    Expression *res;
    while(e.pop(res))
    {
        res->execute(e);
    }
    double z_value, m_value;
    e.get_valuable("z", z_value);
    e.get_valuable("m", m_value);
    assert(std::abs(z_value - 456) < 1e-6);
    assert(e.get_valuable("m", m_value) == false);
}

int main()
{
    /* 重定向以便进行测试 */
    std::ostringstream output, err_output;
    std::streambuf *oldCoutbuf = std::cout.rdbuf(output.rdbuf());
    std::streambuf *oldCerrbuf = std::cerr.rdbuf(err_output.rdbuf());

    test_execute_call();
    test_execute_func_operator();
    test_execute_interaction_func();
    test_execute_call_invalid_argument();

    /* 恢复重定向 */
    std::cout.rdbuf(oldCoutbuf);
    std::cerr.rdbuf(oldCerrbuf);

    return 0;
}