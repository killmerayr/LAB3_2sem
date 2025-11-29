#include "gtest/gtest.h"
#include "../sd/db_list/db_list.hpp"

class DBListTest : public ::testing::Test {
protected:
    void SetUp() override {
        list = new DoublyList();
    }

    void TearDown() override {
        delete list;
    }

    DoublyList* list;
};

// Тесты конструктора и деструктора
TEST_F(DBListTest, ConstructorAndDestructor) {
    EXPECT_TRUE(list->is_empty());
    EXPECT_EQ(list->get_size(), 0);
}

// Тесты метода is_empty
TEST_F(DBListTest, IsEmpty) {
    EXPECT_TRUE(list->is_empty());
    list->push_back("test");
    EXPECT_FALSE(list->is_empty());
    list->del_head();
    EXPECT_TRUE(list->is_empty());
}

// Тесты метода push_back
TEST_F(DBListTest, PushBack) {
    list->push_back("first");
    EXPECT_FALSE(list->is_empty());
    EXPECT_EQ(list->get_size(), 1);
    EXPECT_EQ(list->get_at(0)->data, "first");

    list->push_back("second");
    EXPECT_EQ(list->get_size(), 2);
    EXPECT_EQ(list->get_at(1)->data, "second");
}

// Тесты метода push_front
TEST_F(DBListTest, PushFront) {
    list->push_front("first");
    EXPECT_EQ(list->get_at(0)->data, "first");

    list->push_front("second");
    EXPECT_EQ(list->get_at(0)->data, "second");
    EXPECT_EQ(list->get_at(1)->data, "first");
}

// Тесты метода find
TEST_F(DBListTest, Find) {
    EXPECT_EQ(list->find("nonexistent"), nullptr);

    list->push_back("a");
    list->push_back("b");
    list->push_back("c");

    EXPECT_NE(list->find("a"), nullptr);
    EXPECT_NE(list->find("b"), nullptr);
    EXPECT_NE(list->find("c"), nullptr);
    EXPECT_EQ(list->find("d"), nullptr);
}

// Тесты метода get_size
TEST_F(DBListTest, GetSize) {
    EXPECT_EQ(list->get_size(), 0);
    
    list->push_back("a");
    EXPECT_EQ(list->get_size(), 1);
    
    list->push_back("b");
    EXPECT_EQ(list->get_size(), 2);
    
    list->del_head();
    EXPECT_EQ(list->get_size(), 1);
}

// Тесты метода insert_after
TEST_F(DBListTest, InsertAfter) {
    list->push_back("a");
    list->push_back("c");
    
    // Вставка в середину
    list->insert_after("a", "b");
    EXPECT_EQ(list->get_at(1)->data, "b");
    EXPECT_EQ(list->get_size(), 3);
    
    // Вставка после последнего элемента
    list->insert_after("c", "d");
    EXPECT_EQ(list->get_at(3)->data, "d");
    
    // Вставка после несуществующего элемента
    testing::internal::CaptureStdout();
    list->insert_after("nonexistent", "x");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());
}

// Тесты метода insert_before
TEST_F(DBListTest, InsertBefore) {
    list->push_back("b");
    list->push_back("c");
    
    // Вставка перед первым элементом
    list->insert_before("b", "a");
    EXPECT_EQ(list->get_at(0)->data, "a");
    EXPECT_EQ(list->get_size(), 3);
    
    // Вставка в середину
    list->insert_before("c", "b2");
    EXPECT_EQ(list->get_at(2)->data, "b2");
    
    // Вставка перед несуществующим элементом
    testing::internal::CaptureStdout();
    list->insert_before("nonexistent", "x");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());
}

// Тесты метода del
TEST_F(DBListTest, Delete) {
    list->push_back("a");
    list->push_back("b");
    list->push_back("c");
    
    // Удаление из середины
    list->del("b");
    EXPECT_EQ(list->get_size(), 2);
    EXPECT_EQ(list->get_at(0)->data, "a");
    EXPECT_EQ(list->get_at(1)->data, "c");
    
    // Удаление первого элемента
    list->del("a");
    EXPECT_EQ(list->get_size(), 1);
    EXPECT_EQ(list->get_at(0)->data, "c");
    
    // Удаление последнего элемента
    list->del("c");
    EXPECT_TRUE(list->is_empty());
    
    // Удаление несуществующего элемента
    list->push_back("x");
    testing::internal::CaptureStdout();
    list->del("nonexistent");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(output.empty());
}

// Тесты метода del_head
TEST_F(DBListTest, DeleteHead) {
    // Удаление из пустого списка
    list->del_head();
    EXPECT_TRUE(list->is_empty());
    
    // Удаление из списка с одним элементом
    list->push_back("only");
    list->del_head();
    EXPECT_TRUE(list->is_empty());
    
    // Удаление из списка с несколькими элементами
    list->push_back("first");
    list->push_back("second");
    list->push_back("third");
    
    list->del_head();
    EXPECT_EQ(list->get_size(), 2);
    EXPECT_EQ(list->get_at(0)->data, "second");
}

// Тесты метода del_tail
TEST_F(DBListTest, DeleteTail) {
    // Удаление из пустого списка
    list->del_tail();
    EXPECT_TRUE(list->is_empty());
    
    // Удаление из списка с одним элементом
    list->push_back("only");
    list->del_tail();
    EXPECT_TRUE(list->is_empty());
    
    // Удаление из списка с несколькими элементами
    list->push_back("first");
    list->push_back("second");
    list->push_back("third");
    
    list->del_tail();
    EXPECT_EQ(list->get_size(), 2);
    EXPECT_EQ(list->get_at(1)->data, "second");
}

// Тесты метода del_after
TEST_F(DBListTest, DeleteAfter) {
    list->push_back("a");
    list->push_back("b");
    list->push_back("c");
    
    // Удаление после первого элемента
    list->del_after("a");
    EXPECT_EQ(list->get_size(), 2);
    EXPECT_EQ(list->get_at(0)->data, "a");
    EXPECT_EQ(list->get_at(1)->data, "c");
    
    // Удаление после последнего элемента (ничего не должно произойти)
    list->del_after("c");
    EXPECT_EQ(list->get_size(), 2);
    
    // Удаление после несуществующего элемента
    list->del_after("nonexistent");
    EXPECT_EQ(list->get_size(), 2);
}

// Тесты метода del_before
TEST_F(DBListTest, DeleteBefore) {
    list->push_back("a");
    list->push_back("b");
    list->push_back("c");
    
    // Удаление перед последним элементом
    list->del_before("c");
    EXPECT_EQ(list->get_size(), 2);
    EXPECT_EQ(list->get_at(0)->data, "a");
    EXPECT_EQ(list->get_at(1)->data, "c");
    
    // Удаление перед первым элементом (ничего не должно произойти)
    list->del_before("a");
    EXPECT_EQ(list->get_size(), 2);
    
    // Удаление перед несуществующего элемента
    list->del_before("nonexistent");
    EXPECT_EQ(list->get_size(), 2);
}

// Тесты метода get_at
TEST_F(DBListTest, GetAt) {
    EXPECT_EQ(list->get_at(0), nullptr);
    EXPECT_EQ(list->get_at(-1), nullptr);
    EXPECT_EQ(list->get_at(100), nullptr);
    
    list->push_back("a");
    list->push_back("b");
    list->push_back("c");
    
    EXPECT_EQ(list->get_at(0)->data, "a");
    EXPECT_EQ(list->get_at(1)->data, "b");
    EXPECT_EQ(list->get_at(2)->data, "c");
    EXPECT_EQ(list->get_at(3), nullptr);
}

// Тесты методов печати (проверяем, что не падают)
TEST_F(DBListTest, PrintMethods) {
    // Печать пустого списка
    testing::internal::CaptureStdout();
    list->print_forward();
    list->print_backward();
    testing::internal::GetCapturedStdout(); // Просто проверяем что не падает
    
    // Печать непустого списка
    list->push_back("test1");
    list->push_back("test2");
    
    testing::internal::CaptureStdout();
    list->print_forward();
    list->print_backward();
    testing::internal::GetCapturedStdout(); // Просто проверяем что не падает
}

// Комплексные тесты
TEST_F(DBListTest, ComplexOperations) {
    // Создание сложной структуры
    list->push_back("start");
    list->push_front("before_start");
    list->push_back("end");
    list->insert_after("start", "middle");
    list->insert_before("end", "before_end");
    
    EXPECT_EQ(list->get_size(), 5);
    
    // Проверка порядка
    EXPECT_EQ(list->get_at(0)->data, "before_start");
    EXPECT_EQ(list->get_at(1)->data, "start");
    EXPECT_EQ(list->get_at(2)->data, "middle");
    EXPECT_EQ(list->get_at(3)->data, "before_end");
    EXPECT_EQ(list->get_at(4)->data, "end");
    
    // Удаление в разном порядке
    list->del("middle");
    list->del_head();
    list->del_tail();
    
    EXPECT_EQ(list->get_size(), 2);
    EXPECT_EQ(list->get_at(0)->data, "start");
    EXPECT_EQ(list->get_at(1)->data, "before_end");
}

// Тесты граничных условий
TEST_F(DBListTest, EdgeCases) {
    // Работа с одним элементом
    list->push_back("single");
    list->del("single");
    EXPECT_TRUE(list->is_empty());
    
    // Многократное добавление/удаление
    for (int i = 0; i < 10; ++i) {
        list->push_back(std::to_string(i));
    }
    EXPECT_EQ(list->get_size(), 10);
    
    for (int i = 0; i < 10; ++i) {
        list->del_head();
    }
    EXPECT_TRUE(list->is_empty());
}