#include "gtest/gtest.h"
#include "../sd/stack/stack.hpp"
#include <sstream>
#include <string>

// Вспомогательная функция для перехвата вывода print()
std::string capturePrint(const Stack& s) {
    std::stringstream ss;
    std::streambuf* old = std::cout.rdbuf(ss.rdbuf());
    s.print();
    std::cout.rdbuf(old);
    return ss.str();
}

TEST(StackTest, EmptyInitially) {
    Stack s;
    EXPECT_TRUE(s.is_empty());
    EXPECT_EQ(s.top(), "");
    std::string out = capturePrint(s);
    EXPECT_EQ(out, "\n");
}

TEST(StackTest, PushAndTop) {
    Stack s;
    s.push("first");
    EXPECT_FALSE(s.is_empty());
    EXPECT_EQ(s.top(), "first");

    s.push("second");
    EXPECT_EQ(s.top(), "second");
}

TEST(StackTest, PopAndTop) {
    Stack s;
    s.push("one");
    s.push("two");
    s.push("three");

    EXPECT_EQ(s.top(), "three");
    s.pop();
    EXPECT_EQ(s.top(), "two");
    s.pop();
    EXPECT_EQ(s.top(), "one");
    s.pop();
    EXPECT_TRUE(s.is_empty());
    EXPECT_EQ(s.top(), "");
}

TEST(StackTest, PrintOrder) {
    Stack s;
    s.push("a");
    s.push("b");
    s.push("c");

    std::string out = capturePrint(s);
    EXPECT_EQ(out, "c b a \n");
}

TEST(StackTest, PopEmpty) {
    Stack s;
    std::string out_before = capturePrint(s);
    s.pop();  // должен вывести сообщение о пустом стеке
    std::string out_after = capturePrint(s);
    EXPECT_EQ(out_before, "\n");
    EXPECT_EQ(out_after, "\n");  // стек все еще пуст
}

TEST(StackTest, StressTest) {
    Stack s;
    for (int i = 0; i < 100; ++i) {
        s.push(std::to_string(i));
    }
    for (int i = 99; i >= 0; --i) {
        EXPECT_EQ(s.top(), std::to_string(i));
        s.pop();
    }
    EXPECT_TRUE(s.is_empty());
}