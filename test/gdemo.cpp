// // FILEPATH: /home/authorz/pro_practice/test/InterpreterTest.cpp

// #include "../include/Interpreter.h"
// #include "../include/Func.h"
// #include <gtest/gtest.h>
// #include <sstream>

// class InterpreterTest : public ::testing::Test {
// protected:
//     Interpreter* interpreter;

//     void SetUp() override {
//         interpreter = new Interpreter();
//     }

//     void TearDown() override {
//         delete interpreter;
//     }
// };

// TEST_F(InterpreterTest, TestGetOneFunc) {
//     std::istringstream iss("line1\nline2\nendf\n");
//     Func* func = interpreter->get_one_func(iss, "test_func");

//     ASSERT_NE(func, nullptr);
//     ASSERT_EQ(func->get_expressions().size(), 2);
//     ASSERT_EQ(func->get_expressions()[0]->get_line(), "line1");
//     ASSERT_EQ(func->get_expressions()[1]->get_line(), "line2");
// }

// TEST_F(InterpreterTest, TestGetOneFuncWithEmptyLines) {
//     std::istringstream iss("\n\nline1\n\nline2\n\nendf\n\n");
//     Func* func = interpreter->get_one_func(iss, "test_func");

//     ASSERT_NE(func, nullptr);
//     ASSERT_EQ(func->get_expressions().size(), 2);
//     ASSERT_EQ(func->get_expressions()[0]->get_line(), "line1");
//     ASSERT_EQ(func->get_expressions()[1]->get_line(), "line2");
// }

// TEST_F(InterpreterTest, TestGetOneFuncWithNoEndf) {
//     std::istringstream iss("line1\nline2\n");
//     Func* func = interpreter->get_one_func(iss, "test_func");

//     ASSERT_NE(func, nullptr);
//     ASSERT_EQ(func->get_expressions().size(), 2);
//     ASSERT_EQ(func->get_expressions()[0]->get_line(), "line1");
//     ASSERT_EQ(func->get_expressions()[1]->get_line(), "line2");
// }