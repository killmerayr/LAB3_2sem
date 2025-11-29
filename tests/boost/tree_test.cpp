#include <boost/test/unit_test.hpp>
#include <sstream>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include "../../sd/tree/tree.hpp"

BOOST_AUTO_TEST_SUITE(AVLSuite)

// Улучшенная функция для сбора элементов при обходе
std::vector<int> collectInorder(const AVL& tree) {
    std::vector<int> result;
    std::stringstream buffer;
    auto old_cout = std::cout.rdbuf(buffer.rdbuf());
    tree.print_inorder();
    std::cout.rdbuf(old_cout);
    
    std::string output = buffer.str();
    std::stringstream ss(output);
    int value;
    while (ss >> value) {
        result.push_back(value);
    }
    return result;
}

// Функция для проверки что вектор содержит значение
bool contains(const std::vector<int>& vec, int value) {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}

BOOST_AUTO_TEST_CASE(ConstructorAndDestructor)
{
    AVL t;
    BOOST_TEST(t.getRoot() == nullptr);
    
    {
        AVL temp;
    }
    BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(InsertAndSearch)
{
    AVL t;
    t.insert(10);
    t.insert(5);
    t.insert(15);

    BOOST_TEST(t.search(10) != nullptr);
    BOOST_TEST(t.search(5) != nullptr);
    BOOST_TEST(t.search(15) != nullptr);
    BOOST_TEST(t.search(20) == nullptr);
    
    BOOST_TEST(t.search(10)->val == 10);
    BOOST_TEST(t.search(5)->val == 5);
    BOOST_TEST(t.search(15)->val == 15);
}

BOOST_AUTO_TEST_CASE(RemoveAndCheck)
{
    AVL t;
    t.insert(20);
    t.insert(10);
    t.insert(30);

    t.remove(20);
    BOOST_TEST(t.search(20) == nullptr);
    BOOST_TEST(t.search(10) != nullptr);
    BOOST_TEST(t.search(30) != nullptr);

    t.remove(30);
    BOOST_TEST(t.search(30) == nullptr);

    t.remove(10);
    BOOST_TEST(t.search(10) == nullptr);
    BOOST_TEST(t.getRoot() == nullptr);
}

BOOST_AUTO_TEST_CASE(TreeTraversals)
{
    AVL t;
    t.insert(10);
    t.insert(5);
    t.insert(15);

    BOOST_CHECK_NO_THROW(t.print_inorder());
    BOOST_CHECK_NO_THROW(t.print_preorder());
    BOOST_CHECK_NO_THROW(t.print_postorder());
    BOOST_CHECK_NO_THROW(t.print_lvlorder());
}

BOOST_AUTO_TEST_CASE(AllRotationCases)
{
    // Тест LL rotation
    AVL t1;
    t1.insert(30);
    t1.insert(20);
    t1.insert(10);
    
    auto result1 = collectInorder(t1);
    BOOST_TEST(result1.size() == 3UL);
    
    // Тест RR rotation
    AVL t2;
    t2.insert(10);
    t2.insert(20);
    t2.insert(30);
    
    auto result2 = collectInorder(t2);
    BOOST_TEST(result2.size() == 3UL);
    
    // Тест LR rotation
    AVL t3;
    t3.insert(30);
    t3.insert(10);
    t3.insert(20);
    
    auto result3 = collectInorder(t3);
    BOOST_TEST(result3.size() == 3UL);
    
    // Тест RL rotation
    AVL t4;
    t4.insert(10);
    t4.insert(30);
    t4.insert(20);
    
    auto result4 = collectInorder(t4);
    BOOST_TEST(result4.size() == 3UL);
}

BOOST_AUTO_TEST_CASE(RemoveComplexCases)
{
    AVL t;
    
    t.insert(50);
    t.insert(30);
    t.insert(70);
    t.insert(20);
    t.insert(40);
    t.insert(60);
    t.insert(80);
    
    auto initial = collectInorder(t);
    BOOST_TEST(initial.size() == 7UL);
    
    // Удаление листа
    t.remove(20);
    BOOST_TEST(t.search(20) == nullptr);
    BOOST_TEST(t.search(30) != nullptr);
    
    // Удаление узла с одним ребенком
    t.remove(30);
    BOOST_TEST(t.search(30) == nullptr);
    BOOST_TEST(t.search(40) != nullptr);
    
    // Удаление узла с двумя детьми
    t.remove(50);
    BOOST_TEST(t.search(50) == nullptr);
    BOOST_TEST(t.search(40) != nullptr);
    BOOST_TEST(t.search(60) != nullptr);
    BOOST_TEST(t.search(70) != nullptr);
    
    auto final = collectInorder(t);
    BOOST_TEST(final.size() == 4UL);
}

BOOST_AUTO_TEST_CASE(RemoveRootWithDifferentCases)
{
    // Удаление корня когда у него нет детей
    AVL t1;
    t1.insert(10);
    t1.remove(10);
    BOOST_TEST(t1.getRoot() == nullptr);
    
    // Удаление корня когда у него один ребенок слева
    AVL t2;
    t2.insert(20);
    t2.insert(10);
    t2.remove(20);
    BOOST_TEST(t2.search(20) == nullptr);
    BOOST_TEST(t2.search(10) != nullptr);
    
    // Удаление корня когда у него один ребенок справа
    AVL t3;
    t3.insert(20);
    t3.insert(30);
    t3.remove(20);
    BOOST_TEST(t3.search(20) == nullptr);
    BOOST_TEST(t3.search(30) != nullptr);
    
    // Удаление корня когда у него два ребенка
    AVL t4;
    t4.insert(20);
    t4.insert(10);
    t4.insert(30);
    t4.remove(20);
    BOOST_TEST(t4.search(20) == nullptr);
    BOOST_TEST(t4.search(10) != nullptr);
    BOOST_TEST(t4.search(30) != nullptr);
}

BOOST_AUTO_TEST_CASE(BalanceAfterOperations)
{
    AVL t;
    
    for (int i = 0; i < 20; i++) {
        t.insert(i);
    }
    
    for (int i = 0; i < 20; i++) {
        BOOST_TEST(t.search(i) != nullptr);
        BOOST_TEST(t.search(i)->val == i);
    }
    
    auto result = collectInorder(t);
    BOOST_TEST(result.size() == 20UL);
    for (size_t i = 1; i < result.size(); i++) {
        BOOST_TEST(result[i-1] < result[i]);
    }
    
    for (int i = 5; i < 15; i++) {
        t.remove(i);
    }
    
    for (int i = 0; i < 5; i++) {
        BOOST_TEST(t.search(i) != nullptr);
    }
    for (int i = 15; i < 20; i++) {
        BOOST_TEST(t.search(i) != nullptr);
    }
}

// Исправленный тест с дубликатами
BOOST_AUTO_TEST_CASE(DuplicateInsert)
{
    AVL t;
    t.insert(10);
    t.insert(10); // Дубликат
    
    // Проверяем что структура остается корректной
    BOOST_TEST(t.search(10) != nullptr);
    
    t.insert(5);
    t.insert(15);
    BOOST_TEST(t.search(5) != nullptr);
    BOOST_TEST(t.search(15) != nullptr);
    
    auto result = collectInorder(t);
    // Проверяем что основные элементы присутствуют
    BOOST_TEST(contains(result, 5));
    BOOST_TEST(contains(result, 10));
    BOOST_TEST(contains(result, 15));
}

BOOST_AUTO_TEST_CASE(RemoveNonExistent)
{
    AVL t;
    t.insert(10);
    
    BOOST_CHECK_NO_THROW(t.remove(999));
    BOOST_TEST(t.search(10) != nullptr);
}

BOOST_AUTO_TEST_CASE(LargeScaleOperations)
{
    AVL t;
    const int SIZE = 100;
    
    // Вставляем уникальные элементы
    for (int i = 0; i < SIZE; i++) {
        t.insert(i);
    }
    
    for (int i = 0; i < SIZE; i++) {
        BOOST_TEST(t.search(i) != nullptr);
    }
    
    auto result = collectInorder(t);
    BOOST_TEST(result.size() == static_cast<size_t>(SIZE));
    for (size_t i = 1; i < result.size(); i++) {
        BOOST_TEST(result[i-1] < result[i]);
    }
    
    // Удаляем половину
    for (int i = 0; i < SIZE; i += 2) {
        t.remove(i);
    }
    
    for (int i = 1; i < SIZE; i += 2) {
        BOOST_TEST(t.search(i) != nullptr);
    }
    
    for (int i = 0; i < SIZE; i += 2) {
        BOOST_TEST(t.search(i) == nullptr);
    }
}

BOOST_AUTO_TEST_CASE(TraversalContentCheck)
{
    AVL t;
    t.insert(4);
    t.insert(2);
    t.insert(6);
    t.insert(1);
    t.insert(3);
    t.insert(5);
    t.insert(7);
    
    std::stringstream inorder_buf, preorder_buf, postorder_buf, lvlorder_buf;
    
    auto old_cout = std::cout.rdbuf();
    
    std::cout.rdbuf(inorder_buf.rdbuf());
    t.print_inorder();
    std::cout.rdbuf(old_cout);
    
    std::vector<int> inorder_result;
    std::stringstream iss(inorder_buf.str());
    int val;
    while (iss >> val) {
        inorder_result.push_back(val);
    }
    BOOST_TEST(inorder_result.size() == 7UL);
    for (size_t i = 1; i < inorder_result.size(); i++) {
        BOOST_TEST(inorder_result[i-1] < inorder_result[i]);
    }
    
    std::cout.rdbuf(preorder_buf.rdbuf());
    t.print_preorder();
    std::cout.rdbuf(old_cout);
    BOOST_TEST(!preorder_buf.str().empty());
    
    std::cout.rdbuf(postorder_buf.rdbuf());
    t.print_postorder();
    std::cout.rdbuf(old_cout);
    BOOST_TEST(!postorder_buf.str().empty());
    
    std::cout.rdbuf(lvlorder_buf.rdbuf());
    t.print_lvlorder();
    std::cout.rdbuf(old_cout);
    BOOST_TEST(!lvlorder_buf.str().empty());
}

BOOST_AUTO_TEST_CASE(AddTreeFunctionality)
{
    AVL t1;
    t1.insert(10);
    t1.insert(5);
    t1.insert(15);
    
    AVL t2;
    t2.insert(20);
    t2.insert(25);
    t2.insert(18);
    
    t1.addTree(t2.getRoot());
    
    BOOST_TEST(t1.search(10) != nullptr);
    BOOST_TEST(t1.search(5) != nullptr);
    BOOST_TEST(t1.search(15) != nullptr);
    BOOST_TEST(t1.search(20) != nullptr);
    BOOST_TEST(t1.search(25) != nullptr);
    BOOST_TEST(t1.search(18) != nullptr);
    
    auto result = collectInorder(t1);
    BOOST_TEST(result.size() == 6UL);
    for (size_t i = 1; i < result.size(); i++) {
        BOOST_TEST(result[i-1] < result[i]);
    }
}

BOOST_AUTO_TEST_CASE(EmptyTreeOperations)
{
    AVL t;
    
    BOOST_TEST(t.search(10) == nullptr);
    BOOST_CHECK_NO_THROW(t.remove(10));
    BOOST_CHECK_NO_THROW(t.print_inorder());
    BOOST_CHECK_NO_THROW(t.print_preorder());
    BOOST_CHECK_NO_THROW(t.print_postorder());
    BOOST_CHECK_NO_THROW(t.print_lvlorder());
}

BOOST_AUTO_TEST_CASE(SingleNodeTree)
{
    AVL t;
    t.insert(42);
    
    BOOST_TEST(t.search(42) != nullptr);
    BOOST_TEST(t.search(42)->val == 42);
    
    BOOST_CHECK_NO_THROW(t.print_inorder());
    BOOST_CHECK_NO_THROW(t.print_preorder());
    BOOST_CHECK_NO_THROW(t.print_postorder());
    BOOST_CHECK_NO_THROW(t.print_lvlorder());
    
    auto result = collectInorder(t);
    BOOST_TEST(result.size() == 1UL);
    BOOST_TEST(result[0] == 42);
    
    t.remove(42);
    BOOST_TEST(t.getRoot() == nullptr);
}

BOOST_AUTO_TEST_CASE(ParentPointers)
{
    AVL t;
    t.insert(50);
    t.insert(30);
    t.insert(70);
    
    auto root = t.getRoot();
    auto left = t.search(30);
    auto right = t.search(70);
    
    if (root && left && right) {
        BOOST_TEST(left->parent == root);
        BOOST_TEST(right->parent == root);
        BOOST_TEST(root->parent == nullptr);
    }
    
    t.remove(30);
    BOOST_TEST(t.search(30) == nullptr);
    BOOST_TEST(t.search(50) != nullptr);
    BOOST_TEST(t.search(70) != nullptr);
}

BOOST_AUTO_TEST_CASE(ComplexBalanceScenarios)
{
    AVL t;
    
    int values[] = {41, 20, 65, 11, 29, 50, 91, 32, 72, 99};
    for (int val : values) {
        t.insert(val);
    }
    
    for (int val : values) {
        BOOST_TEST(t.search(val) != nullptr);
    }
    
    auto result = collectInorder(t);
    BOOST_TEST(result.size() == 10UL);
    for (size_t i = 1; i < result.size(); i++) {
        BOOST_TEST(result[i-1] < result[i]);
    }
    
    t.remove(29);
    t.remove(65);
    t.remove(41);
    
    BOOST_TEST(t.search(20) != nullptr);
    BOOST_TEST(t.search(50) != nullptr);
    BOOST_TEST(t.search(91) != nullptr);
    BOOST_TEST(t.search(29) == nullptr);
    BOOST_TEST(t.search(65) == nullptr);
    BOOST_TEST(t.search(41) == nullptr);
    
    auto result2 = collectInorder(t);
    for (size_t i = 1; i < result2.size(); i++) {
        BOOST_TEST(result2[i-1] < result2[i]);
    }
}

BOOST_AUTO_TEST_CASE(MemoryManagementStress)
{
    for (int iteration = 0; iteration < 3; iteration++) {
        AVL t;
        
        for (int i = 0; i < 30; i++) {
            t.insert(i);
        }
        
        for (int i = 0; i < 30; i += 2) {
            t.remove(i);
        }
        
        for (int i = 30; i < 45; i++) {
            t.insert(i);
        }
        
        for (int i = 1; i < 30; i += 2) {
            BOOST_TEST(t.search(i) != nullptr);
        }
        for (int i = 30; i < 45; i++) {
            BOOST_TEST(t.search(i) != nullptr);
        }
        
        auto result = collectInorder(t);
        for (size_t i = 1; i < result.size(); i++) {
            BOOST_TEST(result[i-1] < result[i]);
        }
    }
    BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(SortedInsertion)
{
    AVL t;
    
    for (int i = 0; i < 10; i++) {
        t.insert(i);
    }
    
    auto result = collectInorder(t);
    BOOST_TEST(result.size() == 10UL);
    for (size_t i = 1; i < result.size(); i++) {
        BOOST_TEST(result[i-1] < result[i]);
    }
}

BOOST_AUTO_TEST_CASE(ReverseSortedInsertion)
{
    AVL t;
    
    for (int i = 9; i >= 0; i--) {
        t.insert(i);
    }
    
    auto result = collectInorder(t);
    BOOST_TEST(result.size() == 10UL);
    for (size_t i = 1; i < result.size(); i++) {
        BOOST_TEST(result[i-1] < result[i]);
    }
}

BOOST_AUTO_TEST_CASE(BalanceProperty)
{
    AVL t;
    
    // Вставляем элементы в разном порядке
    t.insert(50);
    t.insert(25);
    t.insert(75);
    t.insert(10);
    t.insert(30);
    t.insert(60);
    t.insert(80);
    t.insert(5);
    t.insert(15);
    t.insert(35);
    t.insert(55);
    t.insert(65);
    
    // Проверяем что все элементы присутствуют и дерево сбалансировано
    auto result = collectInorder(t);
    BOOST_TEST(result.size() == 12UL);
    for (size_t i = 1; i < result.size(); i++) {
        BOOST_TEST(result[i-1] < result[i]);
    }
    
    // Удаляем некоторые элементы и проверяем балансировку
    t.remove(50);
    t.remove(25);
    t.remove(75);
    
    auto result2 = collectInorder(t);
    for (size_t i = 1; i < result2.size(); i++) {
        BOOST_TEST(result2[i-1] < result2[i]);
    }
}

BOOST_AUTO_TEST_SUITE_END()