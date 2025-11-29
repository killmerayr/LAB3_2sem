#include "gtest/gtest.h"
#include "../sd/queue/queue.hpp"
#include <sstream>
#include <string>

// Вспомогательная функция для перехвата вывода print()
std::string capturePrint(const Queue& q) {
    std::stringstream ss;
    std::streambuf* old = std::cout.rdbuf(ss.rdbuf());
    q.print();
    std::cout.rdbuf(old);
    return ss.str();
}

TEST(QueueTest, EmptyInitially) {
    Queue q;
    EXPECT_TRUE(q.is_empty());
    EXPECT_EQ(q.front(), "");
    std::string out = capturePrint(q);
    EXPECT_EQ(out, "Очередь пуста.\n");
}

TEST(QueueTest, PushAndFront) {
    Queue q;
    q.push("first");
    EXPECT_FALSE(q.is_empty());
    EXPECT_EQ(q.front(), "first");

    q.push("second");
    EXPECT_EQ(q.front(), "first"); // первый элемент не меняется
}

TEST(QueueTest, PopAndFront) {
    Queue q;
    q.push("one");
    q.push("two");
    q.push("three");

    EXPECT_EQ(q.front(), "one");
    q.pop();
    EXPECT_EQ(q.front(), "two");
    q.pop();
    EXPECT_EQ(q.front(), "three");
    q.pop();
    EXPECT_TRUE(q.is_empty());
    EXPECT_EQ(q.front(), "");
}

TEST(QueueTest, PrintOrder) {
    Queue q;
    q.push("a");
    q.push("b");
    q.push("c");

    std::string out = capturePrint(q);
    EXPECT_EQ(out, "a b c \n");
}

TEST(QueueTest, PopEmpty) {
    Queue q;
    std::string out_before = capturePrint(q);
    q.pop(); // должен вывести сообщение о пустой очереди
    std::string out_after = capturePrint(q);
    EXPECT_EQ(out_before, "Очередь пуста.\n");
    EXPECT_EQ(out_after, "Очередь пуста.\n");
}

TEST(QueueTest, StressTest) {
    Queue q;
    for (int i = 0; i < 100; ++i) {
        q.push(std::to_string(i));
    }
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(q.front(), std::to_string(i));
        q.pop();
    }
    EXPECT_TRUE(q.is_empty());
}