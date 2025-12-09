#include "gtest/gtest.h"
#include "../sd/list/list.hpp"
#include <sstream>
#include <string>
#include <chrono>

std::string capturePrint(const List& lst) {
    std::stringstream ss;
    std::streambuf* old = std::cout.rdbuf(ss.rdbuf());
    lst.print();
    std::cout.rdbuf(old);
    return ss.str();
}

TEST(ListTest, EmptyList) {
    List lst;
    EXPECT_TRUE(lst.is_empty());
    EXPECT_EQ(lst.get_size(), 0);
    EXPECT_EQ(lst.get_at(0), nullptr);
    EXPECT_EQ(capturePrint(lst), "\n");
}

TEST(ListTest, PushBackAndFront) {
    List lst;
    lst.push_back("a");
    EXPECT_FALSE(lst.is_empty());
    EXPECT_EQ(lst.get_size(), 1);
    EXPECT_EQ(lst.get_at(0)->data, "a");

    lst.push_front("b");
    EXPECT_EQ(lst.get_size(), 2);
    EXPECT_EQ(lst.get_at(0)->data, "b");
    EXPECT_EQ(lst.get_at(1)->data, "a");
}

TEST(ListTest, InsertAfterBefore) {
    List lst;
    lst.push_back("x");
    lst.push_back("y");
    lst.insert_after("x", "z");
    EXPECT_EQ(lst.get_size(), 3);
    EXPECT_EQ(lst.get_at(1)->data, "z");

    lst.insert_before("y", "w");
    EXPECT_EQ(lst.get_size(), 4);
    EXPECT_EQ(lst.get_at(2)->data, "w");
}

TEST(ListTest, DeleteOperations) {
    List lst;
    lst.push_back("one");
    lst.push_back("two");
    lst.push_back("three");

    lst.del("two");
    EXPECT_EQ(lst.get_size(), 2);
    EXPECT_EQ(lst.get_at(1)->data, "three");

    lst.del_head();
    EXPECT_EQ(lst.get_size(), 1);
    EXPECT_EQ(lst.get_at(0)->data, "three");

    lst.del_tail();
    EXPECT_TRUE(lst.is_empty());
}

TEST(ListTest, DelAfterBefore) {
    List lst;
    lst.push_back("a");
    lst.push_back("b");
    lst.push_back("c");
    lst.push_back("d");

    lst.del_after("b"); // удаляет "c"
    EXPECT_EQ(lst.get_size(), 3);
    EXPECT_EQ(lst.get_at(2)->data, "d");

    lst.del_before("d"); // удаляет "b"
    EXPECT_EQ(lst.get_size(), 2);
    EXPECT_EQ(lst.get_at(0)->data, "a");
    EXPECT_EQ(lst.get_at(1)->data, "d");
}

TEST(ListTest, FindFunction) {
    List lst;
    lst.push_back("foo");
    lst.push_back("bar");
    EXPECT_NE(lst.find("foo"), nullptr);
    EXPECT_EQ(lst.find("baz"), nullptr);
}

TEST(ListTest, PrintCheck) {
    List lst;
    lst.push_back("1");
    lst.push_back("2");
    lst.push_back("3");

    EXPECT_EQ(capturePrint(lst), "1 2 3 \n");
}

TEST(ListTest, ComplexOperations) {
    List lst;
    lst.push_back("x");
    lst.push_back("y");
    lst.push_back("z");

    lst.insert_after("x", "a"); // x a y z
    lst.insert_before("y", "b"); // x a b y z
    lst.del_before("y"); // удаляет b -> x a y z
    lst.del_after("a"); // удаляет y -> x a z

    EXPECT_EQ(lst.get_size(), 3);
    EXPECT_EQ(lst.get_at(0)->data, "x");
    EXPECT_EQ(lst.get_at(1)->data, "a");
    EXPECT_EQ(lst.get_at(2)->data, "z");
}

TEST(ListTest, EdgeCases) {
    List lst;

    // Удаление из пустого списка
    lst.del("x");
    lst.del_head();
    lst.del_tail();
    lst.del_after("x");
    lst.del_before("x");

    EXPECT_TRUE(lst.is_empty());

    // Вставка после/перед отсутствующего элемента
    lst.insert_after("nope", "val");
    lst.insert_before("nope", "val2");
    EXPECT_TRUE(lst.is_empty());

    // Проверка одной ноды
    lst.push_back("solo");
    EXPECT_EQ(lst.get_size(), 1);
    lst.del_after("solo");   // ничего не удаляет
    lst.del_before("solo");  // ничего не удаляет
    lst.del("solo");
    EXPECT_TRUE(lst.is_empty());
}

// ===== SERIALIZATION TESTS =====
TEST(ListTest, SerializationBasic) {
    List lst;
    lst.push_back("alpha");
    lst.push_back("beta");
    lst.push_back("gamma");
    
    std::stringstream ss;
    lst.serialize(ss);
    
    List lst2;
    lst2.deserialize(ss);
    
    EXPECT_EQ(lst2.get_size(), 3);
    EXPECT_EQ(lst2.get_at(0)->data, "alpha");
    EXPECT_EQ(lst2.get_at(1)->data, "beta");
    EXPECT_EQ(lst2.get_at(2)->data, "gamma");
}

TEST(ListTest, SerializationEmpty) {
    List lst;
    std::stringstream ss;
    lst.serialize(ss);
    
    List lst2;
    lst2.deserialize(ss);
    EXPECT_TRUE(lst2.is_empty());
}

TEST(ListTest, SerializationLargeList) {
    List lst;
    for (int i = 0; i < 100; ++i) {
        lst.push_back("elem_" + std::to_string(i));
    }
    
    std::stringstream ss;
    lst.serialize(ss);
    
    List lst2;
    lst2.deserialize(ss);
    
    EXPECT_EQ(lst2.get_size(), 100);
    EXPECT_EQ(lst2.get_at(0)->data, "elem_0");
    EXPECT_EQ(lst2.get_at(50)->data, "elem_50");
    EXPECT_EQ(lst2.get_at(99)->data, "elem_99");
}

TEST(ListTest, SerializationSpecialChars) {
    List lst;
    lst.push_back("hello world");
    lst.push_back("123!@#");
    lst.push_back("");
    
    std::stringstream ss;
    lst.serialize(ss);
    
    List lst2;
    lst2.deserialize(ss);
    
    EXPECT_EQ(lst2.get_size(), 3);
    EXPECT_EQ(lst2.get_at(0)->data, "hello world");
    EXPECT_EQ(lst2.get_at(1)->data, "123!@#");
    EXPECT_EQ(lst2.get_at(2)->data, "");
}

// ===== BENCHMARKS =====
TEST(ListBench, BENCHMARK_List_PushBack) {
    auto start = std::chrono::high_resolution_clock::now();
    List l;
    for (int i = 0; i < 10000; ++i) l.push_back("elem_" + std::to_string(i));
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "\npush_back x10000: " << ms << " ms\n";
}

TEST(ListBench, BENCHMARK_List_Find) {
    List l;
    for (int i = 0; i < 1000; ++i) l.push_back("elem_" + std::to_string(i));
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 50000; ++i) l.find("elem_500");
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "\nfind x50000: " << ms << " ms\n";
}