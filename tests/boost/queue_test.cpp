#include <boost/test/unit_test.hpp>
#include <sstream>
#include <iostream>
#include <chrono>
#include "../../sd/queue/queue.hpp"

BOOST_AUTO_TEST_SUITE(QueueSuite)

BOOST_AUTO_TEST_CASE(ConstructorAndDestructor)
{
    Queue q;
    BOOST_TEST(q.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(EnqueueDequeue)
{
    Queue q;
    q.push("x");
    q.push("y");
    BOOST_TEST(q.front() == "x");
    q.pop();
    BOOST_TEST(q.front() == "y");
}

BOOST_AUTO_TEST_CASE(IsEmpty)
{
    Queue q;
    BOOST_TEST(q.is_empty());
    q.push("a");
    BOOST_TEST(!q.is_empty());
    q.pop();
    BOOST_TEST(q.is_empty());
}

BOOST_AUTO_TEST_CASE(PushToEmptyQueue)
{
    Queue q;
    q.push("first");
    BOOST_TEST(q.is_empty() == false);
    BOOST_TEST(q.front() == "first");
}

BOOST_AUTO_TEST_CASE(PushMultipleElements)
{
    Queue q;
    q.push("a");
    q.push("b");
    q.push("c");
    BOOST_TEST(q.front() == "a");
}

BOOST_AUTO_TEST_CASE(PopFromEmptyQueue)
{
    Queue q;
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    q.pop();
    std::cout.rdbuf(old);
    
    BOOST_TEST(!buffer.str().empty()); // должно быть сообщение об ошибке
    BOOST_TEST(q.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(PopSingleElement)
{
    Queue q;
    q.push("only");
    q.pop();
    BOOST_TEST(q.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(PopMultipleElements)
{
    Queue q;
    q.push("first");
    q.push("second");
    q.push("third");
    
    q.pop();
    BOOST_TEST(q.front() == "second");
    
    q.pop();
    BOOST_TEST(q.front() == "third");
    
    q.pop();
    BOOST_TEST(q.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(FrontOnEmptyQueue)
{
    Queue q;
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    std::string result = q.front();
    std::cout.rdbuf(old);
    
    BOOST_TEST(result == "");
    BOOST_TEST(!buffer.str().empty()); // должно быть сообщение об ошибке
}

BOOST_AUTO_TEST_CASE(FrontAfterOperations)
{
    Queue q;
    q.push("initial");
    BOOST_TEST(q.front() == "initial");
    
    q.push("new");
    BOOST_TEST(q.front() == "initial"); // front не меняется
    
    q.pop();
    BOOST_TEST(q.front() == "new");
}

BOOST_AUTO_TEST_CASE(PrintEmptyQueue)
{
    Queue q;
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    q.print();
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    BOOST_TEST(!output.empty());
    BOOST_TEST(output.find("Очередь пуста") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(PrintSingleElement)
{
    Queue q;
    q.push("single");
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    q.print();
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    BOOST_TEST(!output.empty());
    BOOST_TEST(output.find("single") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(PrintMultipleElements)
{
    Queue q;
    q.push("first");
    q.push("second");
    q.push("third");
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    q.print();
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    BOOST_TEST(!output.empty());
    BOOST_TEST(output.find("first") != std::string::npos);
    BOOST_TEST(output.find("second") != std::string::npos);
    BOOST_TEST(output.find("third") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(ComplexOperationsSequence)
{
    Queue q;
    
    // Добавляем несколько элементов
    q.push("a");
    q.push("b");
    q.push("c");
    
    // Проверяем и удаляем
    BOOST_TEST(q.front() == "a");
    q.pop();
    
    BOOST_TEST(q.front() == "b");
    q.push("d");
    
    BOOST_TEST(q.front() == "b");
    q.pop();
    
    BOOST_TEST(q.front() == "c");
    q.pop();
    
    BOOST_TEST(q.front() == "d");
    q.pop();
    
    BOOST_TEST(q.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(DestructorWithElements)
{
    // Создаем очередь с элементами и проверяем что деструктор работает
    Queue* q = new Queue();
    q->push("test1");
    q->push("test2");
    q->push("test3");
    
    delete q; // Должен корректно очистить память
    // Если не падает - успех
    BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(LastPointerIntegrity)
{
    Queue q;
    
    // Проверяем что last всегда корректный
    q.push("first");
    BOOST_TEST(q.front() == "first");
    
    q.push("second");
    q.push("third");
    
    // После добавлений last должен указывать на "third"
    q.pop(); // удаляем "first"
    BOOST_TEST(q.front() == "second");
    
    q.pop(); // удаляем "second"  
    BOOST_TEST(q.front() == "third");
    
    q.push("fourth");
    BOOST_TEST(q.front() == "third");
}

BOOST_AUTO_TEST_CASE(MixedOperations)
{
    Queue q;
    
    // Чередуем добавления и удаления
    q.push("a");
    q.pop();
    BOOST_TEST(q.is_empty() == true);
    
    q.push("b");
    q.push("c");
    q.pop();
    BOOST_TEST(q.front() == "c");
    
    q.push("d");
    q.push("e");
    q.pop();
    BOOST_TEST(q.front() == "d");
    
    q.pop();
    BOOST_TEST(q.front() == "e");
    
    q.pop();
    BOOST_TEST(q.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(StressTest)
{
    Queue q;
    const int NUM_OPERATIONS = 1000;
    
    // Добавляем много элементов
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        q.push("element_" + std::to_string(i));
    }
    
    // Проверяем первый элемент
    BOOST_TEST(q.front() == "element_0");
    
    // Удаляем все элементы
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        q.pop();
    }
    
    BOOST_TEST(q.is_empty() == true);
    
    // Проверяем операции на пустой очереди после стресс-теста
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    q.pop();
    std::cout.rdbuf(old);
    BOOST_TEST(!buffer.str().empty());
}

BOOST_AUTO_TEST_CASE(EmptyQueueAfterDestructor)
{
    Queue* q = new Queue();
    q->push("test");
    q->push("test2");
    delete q;
    
    // Создаем новую очередь - должна быть пустой
    Queue q2;
    BOOST_TEST(q2.is_empty() == true);
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    q2.front();
    std::cout.rdbuf(old);
    BOOST_TEST(!buffer.str().empty());
}

BOOST_AUTO_TEST_CASE(OrderPreservation)
{
    Queue q;
    
    // Проверяем что порядок FIFO сохраняется
    q.push("1");
    q.push("2");
    q.push("3");
    q.push("4");
    q.push("5");
    
    BOOST_TEST(q.front() == "1");
    q.pop();
    
    BOOST_TEST(q.front() == "2");
    q.pop();
    
    BOOST_TEST(q.front() == "3");
    q.pop();
    
    BOOST_TEST(q.front() == "4");
    q.pop();
    
    BOOST_TEST(q.front() == "5");
    q.pop();
    
    BOOST_TEST(q.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(MemoryManagement)
{
    Queue q;
    
    // Многократное использование очереди
    for (int cycle = 0; cycle < 10; ++cycle) {
        for (int i = 0; i < 10; ++i) {
            q.push("cycle_" + std::to_string(cycle) + "_elem_" + std::to_string(i));
        }
        
        for (int i = 0; i < 10; ++i) {
            q.pop();
        }
        
        BOOST_TEST(q.is_empty() == true);
    }
    
    // Финальная проверка
    q.push("final");
    BOOST_TEST(q.front() == "final");
    q.pop();
    BOOST_TEST(q.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(PrintAfterEachOperation)
{
    Queue q;
    
    // Печать после каждой операции для проверки целостности
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    q.print(); // пустая
    
    q.push("a");
    q.print(); // один элемент
    
    q.push("b");
    q.print(); // два элемента
    
    q.pop();
    q.print(); // один элемент
    
    q.pop();
    q.print(); // пустая
    
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    BOOST_TEST(!output.empty());
    BOOST_TEST(output.find("a") != std::string::npos);
    BOOST_TEST(output.find("b") != std::string::npos);
}

// ===== БЕНЧМАРКИ =====
BOOST_AUTO_TEST_CASE(BENCHMARK_Push, * boost::unit_test::label("benchmark"))
{
    auto start = std::chrono::high_resolution_clock::now();
    
    Queue q;
    for (int i = 0; i < 50000; ++i) {
        q.push("element_" + std::to_string(i));
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    BOOST_TEST_MESSAGE("push x50000: " << duration.count() << " ms");
}

BOOST_AUTO_TEST_CASE(BENCHMARK_PushPop, * boost::unit_test::label("benchmark"))
{
    Queue q;
    for (int i = 0; i < 10000; ++i) {
        q.push("elem_" + std::to_string(i));
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 5000; ++i) {
        q.pop();
        q.push("new_" + std::to_string(i));
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    BOOST_TEST_MESSAGE("push/pop x5000: " << duration.count() << " ms");
}

BOOST_AUTO_TEST_SUITE_END()