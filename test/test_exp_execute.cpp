#include <fstream>
#include <sstream>
#include <cassert>
#include "../include/Environment.h"
#include "../include/Interpreter.h"
#include "../include/Expression.h"

void test_execute_operator()
{
    // Create an Expression object
    Expression exp(Expression::ExpType::ADD, "$x", "$y");
    Expression exp2(Expression::ExpType::ADD, "$x", "$z");
    Expression exp3(Expression::ExpType::SUB, "$x", "$z");
    Expression exp4(Expression::ExpType::MUL, "$y", "$z");
    Expression exp5(Expression::ExpType::ADD, "$x", "123");

    double result, result2, result3, result4, result5;

    // Create an Environment object
    Environment e;
    e.init();
    e.add_valuable("x", 5.0);
    e.add_valuable("y", 3.0);
    e.add_valuable("z", 9.5);

    // Call the execute_operator function
    try
    {
        exp.execute(e);
        e.get_valuable("x", result);
        exp2.execute(e);
        e.get_valuable("x", result2);
        exp3.execute(e);
        e.get_valuable("x", result3);
        exp4.execute(e);
        e.get_valuable("y", result4);
        exp5.execute(e);
        e.get_valuable("x", result5);
    }
    catch (...)
    {
        assert(false);
    }

    assert(std::abs(result - 8.0) < 1e-6);
    assert(std::abs(result2 - 17.5) < 1e-6);
    assert(std::abs(result3 - 8.0) < 1e-6);
    assert(std::abs(result4 - 28.5) < 1e-6);
    assert(std::abs(result5 - 131.0) < 1e-6);
}

void test_execute_operator_divide()
{
    // Create an Expression object
    Expression exp(Expression::ExpType::DIV, "$x", "$y");
    Expression exp2(Expression::ExpType::DIV, "$x", "$z");
    double result, result2;
    std::string err_str;

    // Create an Environment object
    Environment e;
    e.init();

    e.add_valuable("x", 5.0);
    e.add_valuable("y", 0.0);
    e.add_valuable("z", 2.0);

    // Call the execute_operator function
    try
    {
        exp.execute(e);
        e.get_valuable("x", result);
    }
    catch (std::invalid_argument const &e)
    {
        err_str = e.what();
        assert(err_str == "Error: " + exp.toString() + " divided by zero");
    }
    try
    {
        exp2.execute(e);
        e.get_valuable("x", result2);
    }
    catch (...)
    {
        assert(false);
    }

    // Check the result
    assert(std::abs(result2 - 2.5) < 1e-6);
}

void test_execute_operator_invalid_argument()
{
    // Create an Expression object
    Expression exp(Expression::ExpType::ADD, "$x", "$y");
    Expression exp2(Expression::ExpType::ADD, "$x", "aszx");
    std::string err_str;

    // Create an Environment object
    Environment e;
    e.init();

    e.add_valuable("x", "hello");
    e.add_valuable("y", 3.0);

    // Call the execute_operator function
    try
    {
        exp.execute(e);
    }
    catch (const std::exception &e)
    {
        err_str = e.what();
        assert(err_str == "Error: " + exp.toString() + " operator don't support string");
    }

    try
    {
        exp2.execute(e);
    }
    catch (const std::exception &e)
    {
        err_str = e.what();
        assert(err_str == "Error: " + exp2.toString() + " stod invalid argument : " + exp2.get_arg2());
        // assert(err_str == "add $x aszx Error:stod\n");
    }

}

void test_execute_reply_single_value(std::ostringstream &output)
{
    // Create an Expression object
    Expression exp(Expression::ExpType::RESPONSE, "$x");
    Expression exp2(Expression::ExpType::RESPONSE, "\"hello\"");
    std::string res;

    // Create an Environment object
    Environment e;
    e.init();

    e.add_valuable("x", 5.0);

    exp.execute(e);
    res = output.str();
    assert(res == "5\n");
    output.str("");
    output.clear();

    // 测试精度
    e.set_default_precision(2);
    exp.execute(e);
    res = output.str();
    assert(res == "5.00\n");
    output.str("");
    output.clear();

    exp2.execute(e);
    res = output.str();
    assert(res == "hello\n");

    // 清除缓冲区
    output.str("");
    output.clear();
}

void test_execute_reply_multiple_values(std::ostringstream &output)
{
    // Create an Expression object
    Expression exp(Expression::ExpType::RESPONSE, "$x+\" and \"+$y");
    std::string res;

    // Create an Environment object
    Environment e;
    e.init();
    output.str("");
    output.clear();

    // Add some valuable
    e.add_valuable("x", 5.0);
    e.add_valuable("y", 3.0);

    // Call the execute_reply function
    exp.execute(e);
    res = output.str();
    assert(res == "5 and 3\n");

    output.str("");
    output.clear();
}

void test_execute_reply_invalid_argument(std::ostringstream &err_output)
{
    // Create an Expression object
    Expression exp(Expression::ExpType::RESPONSE, "$x+$123");

    // Create an Environment object
    Environment e;
    e.init();
    err_output.str("");
    err_output.clear();

    e.add_valuable("x", "hello");
    e.add_valuable("y", 3.0);

    // Call the execute_reply function
    exp.execute_reply(exp, e);

    // Check the result
    assert(err_output.str() == "reply $x+$123 Error:argunment not found : $123\n");

    err_output.str("");
    err_output.clear();
}

void test_execute_let_value()
{
    // Create an Expression object
    Expression exp(Expression::ExpType::LET, "x", "\"hello\"");
    Expression exp2(Expression::ExpType::LET, "y", "1.23");
    Expression exp3(Expression::ExpType::LET, "z", "$y");
    Expression exp4(Expression::ExpType::LET, "w", "$x");
    std::string str_res, str_res2;
    double double_res, double_res2;

    // Create an Environment object
    Environment e;
    e.init();

    // Call the execute_let function
    exp.execute(e);
    exp2.execute(e);
    exp3.execute(e);
    exp4.execute(e);

    // Check the result
    e.get_valuable("x", str_res);
    e.get_valuable("w", str_res2);
    e.get_valuable("y", double_res);
    e.get_valuable("z", double_res2);

    assert(str_res == "hello");
    assert(str_res2 == "hello");
    assert(std::abs(double_res - 1.23) < 1e-6);
    assert(std::abs(double_res2 - 1.23) < 1e-6);
}

void test_execute_let_invalid_argument(std::ostringstream &err_output)
{
    // Create an Expression object
    Expression exp(Expression::ExpType::LET, "x", "$y");

    // Create an Environment object
    Environment e;
    e.init();
    err_output.str("");
    err_output.clear();

    // Call the execute_let function
    exp.execute(e);

    // Check the result
    assert(err_output.str() == exp.toString() + " Error:argunment not found : $y\n");

    // 清空oss的内容
    err_output.str("");
    err_output.clear();
}

void test_execute_if_equal()
{
    // Create an Expression object
    Expression exp(Expression::ExpType::IF_EQUAL, "$x", "$y", new Expression(Expression::ExpType::LET, "z", "1"));

    // Create an Environment object
    Environment e;
    e.init();

    // Set the values of $x and $y to be equal
    e.add_valuable("x", 5.0);
    e.add_valuable("y", 5.0);

    // Call the execute_if_equal function
    exp.execute(e);

    // Check the result
    double result;
    e.get_valuable("z", result);
    assert(std::abs(result - 1.0) < 1e-6);
}

void test_execute_if_not_equal()
{
    // Create an Expression object
    Expression exp(Expression::ExpType::IF_EQUAL, "$x", "$y", new Expression(Expression::ExpType::LET, "$z", "1"));

    // Create an Environment object
    Environment e;
    e.init();

    // Set the values of $x and $y to be not equal
    e.add_valuable("x", 5.0);
    e.add_valuable("y", 6.0);

    // Call the execute_if_equal function
    exp.execute(e);

    // Check the result
    double result;
    bool is_exist = e.get_valuable("z", result);
    assert(!is_exist);
}

void test_execute_if_equal_invalid_argument(std::ostringstream &err_output)
{
    // Create an Expression object
    Expression arg3(Expression::ExpType::LET, "z", "1");
    Expression exp(Expression::ExpType::IF_EQUAL, "$x", "$y", &arg3);

    // Create an Environment object
    Environment e;
    e.init();
    err_output.str("");
    err_output.clear();

    // Set the value of $x
    e.add_valuable("x", 5.0);

    // Call the execute_if_equal function
    exp.execute(e);

    // Check the result
    std::string err_str = err_output.str();
    assert(err_str == exp.toString() + " Error:argunment not found : $y\n");

    // 清空oss的内容
    err_output.str("");
    err_output.clear();
}

void test_execute_get()
{
    // Create an Expression object
    Expression exp(Expression::ExpType::GET_STRING, "x");
    Expression exp2(Expression::ExpType::GET_STRING, "y");

    // Create an Environment object
    Environment e;
    e.init();

    // Redirect cin
    std::istringstream input("5.0\nhello\n");
    std::cin.rdbuf(input.rdbuf());

    // Call the execute_get function
    exp.execute(e);
    exp2.execute(e);

    // Check the result
    std::string result, result2;
    e.get_valuable("x", result);
    e.get_valuable("y", result2);
    assert(result == "5.0");
    assert(result2 == "hello");
}

int main()
{
    /* 重定向以便进行测试 */
    std::ostringstream output, err_output;
    std::streambuf *oldCoutbuf = std::cout.rdbuf(output.rdbuf());
    std::streambuf *oldCerrbuf = std::cerr.rdbuf(err_output.rdbuf());

    /* test for operator */
    test_execute_operator();                  // Run the basic test
    test_execute_operator_divide(); // exception test
    test_execute_operator_invalid_argument();

    /* test for reply*/
    test_execute_reply_single_value(output); // basic test
    test_execute_reply_multiple_values(output);
    test_execute_reply_invalid_argument(err_output); // exception test

    /* test for let */
    test_execute_let_value();                      // basic test
    test_execute_let_invalid_argument(err_output); // exception test

    /* test for if equal */
    test_execute_if_equal(); // basic test
    test_execute_if_not_equal();
    test_execute_if_equal_invalid_argument(err_output); // exception test

    /* test for get */
    test_execute_get(); // basic test no exception

    // 恢复cout
    std::cout.rdbuf(oldCoutbuf);
    std::cerr.rdbuf(oldCerrbuf);
    return 0;
}