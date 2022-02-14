#include <gtest/gtest.h>
#include "../recognizer.h"

struct TestFixture : public ::testing::Test
{
    Recognizer recognizer;
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

    for( auto& input : inputs )
    {
        auto response = recognizer.responser( input.first );
        EXPECT_STREQ( response.c_str(), input.second.c_str() );
    }
}