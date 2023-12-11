#include <fstream>
#include <cassert>
#include "../include/Interpreter.h"
#include "../include/Expression.h"

void test_execute_call() {
    // Create an Interpreter object
    Interpreter *pi = new Interpreter();
    
    // Load a script file
    std::string filename = "../data/demo_script.txt";
    pi->init(filename);

    std::cout << "show functions:" << std::endl;
    pi->show_funcs();
    
    // Call the execute_call function
    pi->execute();
    
    // Clean up
    delete pi;
}

void test_execute_operator() {
    // Create an Expression object
    Expression exp(Expression::ExpType::ADD, "$x", "$y");
    
    // Create an Environment object
    Environment e;
    e.set_real("x", 5.0);
    e.set_real("y", 3.0);
    
    // Call the execute_operator function
    exp.execute(e);
    
    // Check the result
    double result; 
    e.get_real("x", result);
    assert(result == 8.0);
}


void test_execute_operator_divide_by_zero() {
    // Create an Expression object
    Expression exp(Expression::ExpType::DIV, "$x", "$y");
    
    // Create an Environment object
    Environment e;
    e.set_real("x", 5.0);
    e.set_real("y", 0.0);
    
    // Call the execute_operator function
    try {
        exp.execute_operator(exp, e);
        // The function should throw an exception, so the following line should not be reached
        assert(false);
    } catch (std::invalid_argument const& e) {
        // Check if the exception message is correct
        assert(std::string(e.what()) == "Divide by zero");
    }
}

void test_execute_operator_invalid_argument() {
    // Create an Expression object
    Expression exp(Expression::ExpType::ADD, "$x", "$y");
    
    // Create an Environment object
    Environment e;
    e.set_string("x", "hello");
    e.set_real("y", 3.0);
    
    // Call the execute_operator function
    try {
        exp.execute_operator(exp, e);
        // The function should throw an exception, so the following line should not be reached
        assert(false);
    } catch (std::invalid_argument const& e) {
        // Check if the exception message is correct
        assert(std::string(e.what()) == "Operator don't support string");
    }
}

int main() {
    // Run the basic test
    test_execute_operator();
    
    // Run the operator and exception test
    test_execute_operator();
    test_execute_operator_divide_by_zero();
    test_execute_operator_invalid_argument();
    
    // Run the entire test
    // test_execute_call();

    return 0;
}