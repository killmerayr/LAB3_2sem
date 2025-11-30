#include <boost/test/unit_test.hpp>
#include <sstream>
#include <iostream>
#include <chrono>
#include "../../sd/stack/stack.hpp"

BOOST_AUTO_TEST_SUITE(StackSuite)

BOOST_AUTO_TEST_CASE(ConstructorAndDestructor)
{
    Stack s;
    BOOST_TEST(s.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(PushPop)
{
    Stack s;
    s.push("a");
    s.push("b");
    BOOST_TEST(s.top() == "b");
    s.pop();
    BOOST_TEST(s.top() == "a");
}

BOOST_AUTO_TEST_CASE(IsEmpty)
{
    Stack s;
    BOOST_TEST(s.is_empty());
    s.push("x");
    BOOST_TEST(!s.is_empty());
    s.pop();
    BOOST_TEST(s.is_empty());
}

BOOST_AUTO_TEST_CASE(PushToEmptyStack)
{
    Stack s;
    s.push("first");
    BOOST_TEST(s.is_empty() == false);
    BOOST_TEST(s.top() == "first");
}

BOOST_AUTO_TEST_CASE(PushMultipleElements)
{
    Stack s;
    s.push("a");
    s.push("b");
    s.push("c");
    BOOST_TEST(s.top() == "c");
    
    s.pop();
    BOOST_TEST(s.top() == "b");
    
    s.pop();
    BOOST_TEST(s.top() == "a");
}

BOOST_AUTO_TEST_CASE(PopFromEmptyStack)
{
    Stack s;
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    s.pop();
    std::cout.rdbuf(old);
    
    BOOST_TEST(!buffer.str().empty()); // должно быть сообщение об ошибке
    BOOST_TEST(s.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(PopSingleElement)
{
    Stack s;
    s.push("only");
    s.pop();
    BOOST_TEST(s.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(PopMultipleElements)
{
    Stack s;
    s.push("first");
    s.push("second");
    s.push("third");
    
    s.pop();
    BOOST_TEST(s.top() == "second");
    
    s.pop();
    BOOST_TEST(s.top() == "first");
    
    s.pop();
    BOOST_TEST(s.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(TopOnEmptyStack)
{
    Stack s;
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    std::string result = s.top();
    std::cout.rdbuf(old);
    
    BOOST_TEST(result == "");
    BOOST_TEST(!buffer.str().empty()); // должно быть сообщение об ошибке
}

BOOST_AUTO_TEST_CASE(TopAfterOperations)
{
    Stack s;
    s.push("initial");
    BOOST_TEST(s.top() == "initial");
    
    s.push("new");
    BOOST_TEST(s.top() == "new"); // top меняется
    
    s.pop();
    BOOST_TEST(s.top() == "initial");
}

BOOST_AUTO_TEST_CASE(PrintEmptyStack)
{
    Stack s;
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    s.print();
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    BOOST_TEST(!output.empty()); // вывод должен быть (пустая строка с endl)
}

BOOST_AUTO_TEST_CASE(PrintSingleElement)
{
    Stack s;
    s.push("single");
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    s.print();
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    BOOST_TEST(!output.empty());
    BOOST_TEST(output.find("single") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(PrintMultipleElements)
{
    Stack s;
    s.push("first");
    s.push("second");
    s.push("third");
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    s.print();
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    BOOST_TEST(!output.empty());
    BOOST_TEST(output.find("first") != std::string::npos);
    BOOST_TEST(output.find("second") != std::string::npos);
    BOOST_TEST(output.find("third") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(ComplexOperationsSequence)
{
    Stack s;
    
    // Добавляем несколько элементов
    s.push("a");
    s.push("b");
    s.push("c");
    
    // Проверяем и удаляем
    BOOST_TEST(s.top() == "c");
    s.pop();
    
    BOOST_TEST(s.top() == "b");
    s.push("d");
    
    BOOST_TEST(s.top() == "d");
    s.pop();
    
    BOOST_TEST(s.top() == "b");
    s.pop();
    
    BOOST_TEST(s.top() == "a");
    s.pop();
    
    BOOST_TEST(s.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(DestructorWithElements)
{
    // Создаем стек с элементами и проверяем что деструктор работает
    Stack* s = new Stack();
    s->push("test1");
    s->push("test2");
    s->push("test3");
    
    delete s; // Должен корректно очистить память
    // Если не падает - успех
    BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(TopNodeIntegrity)
{
    Stack s;
    
    // Проверяем что top_node всегда корректный
    s.push("first");
    BOOST_TEST(s.top() == "first");
    
    s.push("second");
    BOOST_TEST(s.top() == "second");
    
    s.push("third");
    BOOST_TEST(s.top() == "third");
    
    s.pop(); // удаляем "third"
    BOOST_TEST(s.top() == "second");
    
    s.pop(); // удаляем "second"  
    BOOST_TEST(s.top() == "first");
    
    s.push("fourth");
    BOOST_TEST(s.top() == "fourth");
}

BOOST_AUTO_TEST_CASE(MixedOperations)
{
    Stack s;
    
    // Чередуем добавления и удаления
    s.push("a");
    s.pop();
    BOOST_TEST(s.is_empty() == true);
    
    s.push("b");
    s.push("c");
    s.pop();
    BOOST_TEST(s.top() == "b");
    
    s.push("d");
    s.push("e");
    s.pop();
    BOOST_TEST(s.top() == "d");
    
    s.pop();
    BOOST_TEST(s.top() == "b");
    
    s.pop();
    BOOST_TEST(s.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(StressTest)
{
    Stack s;
    const int NUM_OPERATIONS = 1000;
    
    // Добавляем много элементов
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        s.push("element_" + std::to_string(i));
    }
    
    // Проверяем последний элемент
    BOOST_TEST(s.top() == "element_" + std::to_string(NUM_OPERATIONS - 1));
    
    // Удаляем все элементы
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        s.pop();
    }
    
    BOOST_TEST(s.is_empty() == true);
    
    // Проверяем операции на пустом стеке после стресс-теста
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    s.pop();
    std::cout.rdbuf(old);
    BOOST_TEST(!buffer.str().empty());
}

BOOST_AUTO_TEST_CASE(EmptyStackAfterDestructor)
{
    Stack* s = new Stack();
    s->push("test");
    s->push("test2");
    delete s;
    
    // Создаем новый стек - должен быть пустым
    Stack s2;
    BOOST_TEST(s2.is_empty() == true);
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    s2.top();
    std::cout.rdbuf(old);
    BOOST_TEST(!buffer.str().empty());
}

BOOST_AUTO_TEST_CASE(OrderPreservationLIFO)
{
    Stack s;
    
    // Проверяем что порядок LIFO сохраняется
    s.push("1");
    s.push("2");
    s.push("3");
    s.push("4");
    s.push("5");
    
    BOOST_TEST(s.top() == "5");
    s.pop();
    
    BOOST_TEST(s.top() == "4");
    s.pop();
    
    BOOST_TEST(s.top() == "3");
    s.pop();
    
    BOOST_TEST(s.top() == "2");
    s.pop();
    
    BOOST_TEST(s.top() == "1");
    s.pop();
    
    BOOST_TEST(s.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(MemoryManagement)
{
    Stack s;
    
    // Многократное использование стека
    for (int cycle = 0; cycle < 10; ++cycle) {
        for (int i = 0; i < 10; ++i) {
            s.push("cycle_" + std::to_string(cycle) + "_elem_" + std::to_string(i));
        }
        
        for (int i = 0; i < 10; ++i) {
            s.pop();
        }
        
        BOOST_TEST(s.is_empty() == true);
    }
    
    // Финальная проверка
    s.push("final");
    BOOST_TEST(s.top() == "final");
    s.pop();
    BOOST_TEST(s.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(PrintAfterEachOperation)
{
    Stack s;
    
    // Печать после каждой операции для проверки целостности
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    s.print(); // пустой
    
    s.push("a");
    s.print(); // один элемент
    
    s.push("b");
    s.print(); // два элемента
    
    s.pop();
    s.print(); // один элемент
    
    s.pop();
    s.print(); // пустой
    
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    BOOST_TEST(!output.empty());
    BOOST_TEST(output.find("a") != std::string::npos);
    BOOST_TEST(output.find("b") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(StackBehaviorLIFO)
{
    Stack s;
    
    // Классическое поведение LIFO
    s.push("1");
    s.push("2");
    s.push("3");
    
    // Должны извлекаться в обратном порядке
    BOOST_TEST(s.top() == "3");
    s.pop();
    
    BOOST_TEST(s.top() == "2");
    s.pop();
    
    BOOST_TEST(s.top() == "1");
    s.pop();
    
    BOOST_TEST(s.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(RepeatedPushPop)
{
    Stack s;
    
    // Многократные операции push/pop
    for (int i = 0; i < 5; i++) {
        s.push("item");
        BOOST_TEST(s.top() == "item");
        s.pop();
        BOOST_TEST(s.is_empty() == true);
    }
}

BOOST_AUTO_TEST_CASE(LargeNumberOfElements)
{
    Stack s;
    const int COUNT = 500;
    
    // Добавляем много элементов
    for (int i = 0; i < COUNT; i++) {
        s.push(std::to_string(i));
    }
    
    // Проверяем порядок извлечения
    for (int i = COUNT - 1; i >= 0; i--) {
        BOOST_TEST(s.top() == std::to_string(i));
        s.pop();
    }
    
    BOOST_TEST(s.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(EmptyStackOperations)
{
    Stack s;
    
    // Все операции на пустом стеке
    BOOST_TEST(s.is_empty() == true);
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    s.pop(); // не должно падать
    s.top(); // не должно падать
    s.print(); // не должно падать
    
    std::cout.rdbuf(old);
    
    // Должны быть сообщения об ошибках для pop и top
    std::string output = buffer.str();
    BOOST_TEST(output.find("Стек пуст") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(SingleElementStack)
{
    Stack s;
    
    // Все операции со стеком из одного элемента
    s.push("single");
    BOOST_TEST(s.is_empty() == false);
    BOOST_TEST(s.top() == "single");
    
    s.pop();
    BOOST_TEST(s.is_empty() == true);
    
    // Повторные операции после очистки
    s.push("new_single");
    BOOST_TEST(s.top() == "new_single");
    s.pop();
    BOOST_TEST(s.is_empty() == true);
}

// ===== БЕНЧМАРКИ =====
BOOST_AUTO_TEST_CASE(BENCHMARK_Push, * boost::unit_test::label("benchmark"))
{
    auto start = std::chrono::high_resolution_clock::now();
    
    Stack s;
    for (int i = 0; i < 50000; ++i) {
        s.push("element_" + std::to_string(i));
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    BOOST_TEST_MESSAGE("push x50000: " << duration.count() << " ms");
}

BOOST_AUTO_TEST_CASE(BENCHMARK_PushPop, * boost::unit_test::label("benchmark"))
{
    Stack s;
    for (int i = 0; i < 10000; ++i) {
        s.push("elem_" + std::to_string(i));
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 5000; ++i) {
        s.pop();
        s.push("new_" + std::to_string(i));
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    BOOST_TEST_MESSAGE("push/pop x5000: " << duration.count() << " ms");
}

BOOST_AUTO_TEST_SUITE_END()