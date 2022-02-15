#include <gtest/gtest.h>
#include "../recognition.h"

struct TestFixture : public ::testing::Test
{
    Recognition recognition;
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F( TestFixture, ExampleInputOutputs )
{
    std::map<std::string,std::string> inputs
    {
        { "What is the weather like today?", "Get Weather"},
        { "What is the weather like in Paris today?", "Get Weather City"},
        { "Tell me about fact", "Get Fact" },
        { "Can you open the door?", "Set Door" },
        { "May you change the AC to 22 degree ?", "" },
        { "Could you open window?", "" }
    };
    
    bool ret = recognition.init( "../inputs/", "operations.txt", "questions.txt", "subjects.txt", "objects.txt" );

    ASSERT_EQ( ret, true );
    
    for( auto& input : inputs )
    {
        auto response = recognition.recognize( input.first );
        EXPECT_STREQ( response.c_str(), input.second.c_str() );
    }
}