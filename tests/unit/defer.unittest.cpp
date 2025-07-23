#include <gtest/gtest.h>

#include <refed/defer.hpp>

TEST(DeferTestSuite, BasicTest)
{
    int x = 42;

    refed::Defer d1{[&](){ EXPECT_EQ(x, 92); }};
    EXPECT_EQ(x, 42);

    refed::Defer d2{[&](){ x *= 2; }};
    EXPECT_EQ(x, 42);

    refed::Defer d3{[&](){ EXPECT_EQ(x, 46); }};
    EXPECT_EQ(x, 42);

    refed::Defer d4{[&](){ x += 2; }};
    EXPECT_EQ(x, 42);

    refed::Defer d5{[&](){ EXPECT_EQ(x, 44); }};
    EXPECT_EQ(x, 42);
    
    x += 2;
}

TEST(DeferSuite, ThrowTest)
{
    int x = 42;

    EXPECT_THROW({
        refed::Defer d1{[&](){ EXPECT_EQ(x, 84); }};
        refed::Defer d2{[&](){ x *= 2; }};
        refed::Defer d3{[&](){ EXPECT_EQ(x, 42); }};
    
        throw std::runtime_error{"ThrowTest exception"};
    }, std::runtime_error);

    EXPECT_EQ(x, 84);
}
