#include <catch2/catch_all.hpp>
#include "../../sd/queue/queue.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

TEST_CASE("Конструктор и is_empty", "[Queue]") {
    Queue q;
    REQUIRE(q.is_empty() == true);
}

TEST_CASE("push и front", "[Queue]") {
    Queue q;
    q.push("a");
    REQUIRE(q.is_empty() == false);
    REQUIRE(q.front() == "a");

    q.push("b");
    REQUIRE(q.front() == "a"); // первый элемент не меняется
}

TEST_CASE("pop", "[Queue]") {
    Queue q;
    q.push("a");
    q.push("b");

    q.pop();
    REQUIRE(q.front() == "b");
    REQUIRE(q.is_empty() == false);

    q.pop();
    REQUIRE(q.is_empty() == true);
    REQUIRE(q.front() == ""); // пустая очередь
}

TEST_CASE("pop на пустой очереди", "[Queue]") {
    Queue q;
    REQUIRE_NOTHROW(q.pop());
    REQUIRE(q.front() == "");
}

TEST_CASE("front на пустой очереди", "[Queue]") {
    Queue q;
    REQUIRE(q.front() == "");
}

TEST_CASE("несколько операций push/pop", "[Queue]") {
    Queue q;
    q.push("x");
    q.push("y");
    q.push("z");

    REQUIRE(q.front() == "x");

    q.pop();
    REQUIRE(q.front() == "y");

    q.push("w");
    REQUIRE(q.front() == "y");

    q.pop();
    REQUIRE(q.front() == "z");

    q.pop();
    REQUIRE(q.front() == "w");

    q.pop();
    REQUIRE(q.is_empty() == true);
}

TEST_CASE("Queue — edge cases for full coverage", "[Queue][extra]") {
    Queue q;

    // --- pop и front на пустой очереди ---
    std::stringstream out_empty;
    std::streambuf* old = std::cout.rdbuf(out_empty.rdbuf());
    q.pop();           // должно вывести сообщение о пустой очереди
    q.front();         // должно вывести сообщение о пустой очереди
    std::cout.rdbuf(old);
    REQUIRE(out_empty.str().find("Очередь пуста") != std::string::npos);

    // --- push и pop на одном элементе ---
    q.push("A");
    REQUIRE(q.front() == "A");
    REQUIRE(!q.is_empty());

    q.pop();           // удаление единственного элемента
    REQUIRE(q.is_empty());
    REQUIRE(q.front() == ""); // пустая очередь после pop

    // --- несколько push и pop с проверкой first/last ---
    q.push("B");
    q.push("C");
    REQUIRE(q.front() == "B");

    q.pop(); // удаляем первый
    REQUIRE(q.front() == "C");

    q.pop(); // удаляем последний
    REQUIRE(q.is_empty());
    REQUIRE(q.front() == "");
}

TEST_CASE("Queue — extensive edge cases", "[Queue][extra2]") {
    Queue q;

    // --- Проверка pop и front на пустой очереди несколько раз ---
    std::stringstream out_empty;
    std::streambuf* old = std::cout.rdbuf(out_empty.rdbuf());
    q.pop(); 
    q.front();
    q.pop(); 
    std::cout.rdbuf(old);
    REQUIRE(out_empty.str().find("Очередь пуста") != std::string::npos);

    // --- Проверка print на пустой очереди ---
    std::stringstream out_print;
    old = std::cout.rdbuf(out_print.rdbuf());
    q.print();
    std::cout.rdbuf(old);
    REQUIRE(out_print.str().find("Очередь пуста") != std::string::npos);

    // --- push и pop на одном элементе ---
    q.push("A");
    REQUIRE(q.front() == "A");
    REQUIRE(!q.is_empty());

    std::stringstream out_print1;
    old = std::cout.rdbuf(out_print1.rdbuf());
    q.print();
    std::cout.rdbuf(old);
    REQUIRE(out_print1.str().find("A") != std::string::npos);

    q.pop();
    REQUIRE(q.is_empty());
    REQUIRE(q.front() == "");

    // --- push нескольких элементов и последовательные pop ---
    q.push("B");
    q.push("C");
    q.push("D");
    REQUIRE(q.front() == "B");
    
    q.pop();
    REQUIRE(q.front() == "C");
    
    q.pop();
    REQUIRE(q.front() == "D");

    // --- push после опустошения очереди ---
    q.pop();
    REQUIRE(q.is_empty());

    q.push("E");
    q.push("F");
    REQUIRE(q.front() == "E");

    // --- pop одного элемента оставляет следующий ---
    q.pop();
    REQUIRE(q.front() == "F");

    // --- удаление последнего элемента обновляет first и last ---
    q.pop();
    REQUIRE(q.is_empty());

    // --- push/pop чередование с несколькими элементами ---
    q.push("X");
    q.push("Y");
    REQUIRE(q.front() == "X");
    q.pop();
    REQUIRE(q.front() == "Y");
    q.push("Z");
    REQUIRE(q.front() == "Y");
    q.pop();
    REQUIRE(q.front() == "Z");
    q.pop();
    REQUIRE(q.is_empty());

    // --- print непустой очереди ---
    q.push("M");
    q.push("N");
    std::stringstream out_print2;
    old = std::cout.rdbuf(out_print2.rdbuf());
    q.print();
    std::cout.rdbuf(old);
    REQUIRE(out_print2.str().find("M") != std::string::npos);
    REQUIRE(out_print2.str().find("N") != std::string::npos);
}

TEST_CASE("Queue — stress test", "[Queue][stress]") {
    Queue q;
    
    // --- много push операций ---
    for (int i = 0; i < 100; ++i) {
        q.push("elem" + std::to_string(i));
    }
    
    REQUIRE(q.front() == "elem0");
    REQUIRE(!q.is_empty());
    
    // --- последовательные pop ---
    for (int i = 0; i < 100; ++i) {
        REQUIRE(q.front() == "elem" + std::to_string(i));
        q.pop();
    }
    
    REQUIRE(q.is_empty());
}

// ===== BENCHMARKS =====
TEST_CASE("BENCHMARK_Queue_Push", "[benchmark]") {
    auto start = std::chrono::high_resolution_clock::now();
    Queue q;
    for (int i = 0; i < 50000; ++i) q.push("elem_" + std::to_string(i));
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    INFO("push x50000: " << ms << " ms");
}

TEST_CASE("BENCHMARK_Queue_PushPop", "[benchmark]") {
    Queue q;
    for (int i = 0; i < 10000; ++i) q.push("elem_" + std::to_string(i));
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 5000; ++i) { q.pop(); q.push("new_" + std::to_string(i)); }
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    INFO("push/pop x5000: " << ms << " ms");
}