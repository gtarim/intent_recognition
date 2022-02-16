#include <gtest/gtest.h>
#include "../recognition.h"

struct TestFixture : public ::testing::Test
{
    Recognition recognition;
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F( TestFixture, IsFileExist )
{
    bool ret = recognition.isFileExist( "./test.cpp" );
    EXPECT_EQ( ret, true );

    ret = recognition.isFileExist( "asd.txt" );
    EXPECT_NE( ret, true );
}

TEST_F( TestFixture, FindCorrection )
{
    bool ret = recognition.find( "This is a test message", "message" );
    EXPECT_EQ( ret, true );
    ret = recognition.find( "This is a test message", "This" );
    EXPECT_EQ( ret, true );
    ret = recognition.find( "This is a test message", "a test" );
    EXPECT_EQ( ret, true );
    ret = recognition.find( "This is a test message", "This is a test message" );
    EXPECT_EQ( ret, true );

    ret = recognition.find( "This is a test message", "this" ); // important : first letter lowercase
    EXPECT_NE( ret, true );
    ret = recognition.find( "This is a test message", "TEST" ); // important : first letter uppercase
    EXPECT_NE( ret, true );
}

TEST_F( TestFixture, SplitLinesToVector )
{
    std::vector<std::string> vec = recognition.prepareInputFileToLines( "../inputs/operations.txt" );
    EXPECT_EQ( vec.size(), 2 ); // important : if file line count increased change the test variable
}

TEST_F( TestFixture, InputOutputTest )
{
    std::map<std::string,std::string> inputs
    {
        { "What is the weather like today?", "Get Weather"},
        { "What is the weather like in Paris today?", "Get Weather City"},
        { "Tell me about fact", "Get Fact" },
        { "Can you open the door?", "Set Door" },
        { "May you change the AC to 22 degree ?", "" },
        { "Could you open window?", "" },
        { "Tell me about weather in Paris today?", "Get Weather City" },
        { "What is the fact?", "Get Fact" }
    };
    
    bool ret = recognition.init( "../inputs/", "operations.txt", "questions.txt", "subjects.txt", "objects.txt" );

    ASSERT_EQ( ret, true );
    
    for( auto& input : inputs )
    {
        auto response = recognition.recognize( input.first );
        EXPECT_STREQ( response.c_str(), input.second.c_str() );
    }
}
