// FILEPATH: /home/authorz/pro_practice/test/InterpreterTest.cpp
#include <gtest/gtest.h>
#include <fstream>
#include <string>

class FileOpenTest : public ::testing::Test {
protected:
    std::string valid_filename = "/home/authorz/pro_practice/src/Intepreter.cpp";
    std::string invalid_filename = "/invalid/path/to/file.cpp";
};

TEST_F(FileOpenTest, OpensValidFile) {
    std::ifstream file(valid_filename);
    EXPECT_TRUE(file.is_open());
}

TEST_F(FileOpenTest, FailsToOpenInvalidFile) {
    std::ifstream file(invalid_filename);
    EXPECT_FALSE(file.is_open());
}