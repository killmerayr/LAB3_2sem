#include <catch2/catch_all.hpp>
#include "../../sd/stack/stack.hpp"
#include <iostream>

TEST_CASE("Конструктор и is_empty", "[Stack]") {
    Stack s;
    REQUIRE(s.is_empty() == true);
}

TEST_CASE("push и top", "[Stack]") {
    Stack s;
    s.push("a");
    REQUIRE(s.is_empty() == false);
    REQUIRE(s.top() == "a");

    s.push("b");
    REQUIRE(s.top() == "b"); // верхний элемент должен быть последним добавленным
}

TEST_CASE("pop", "[Stack]") {
    Stack s;
    s.push("a");
    s.push("b");

    s.pop();
    REQUIRE(s.top() == "a");
    REQUIRE(s.is_empty() == false);

    s.pop();
    REQUIRE(s.is_empty() == true);
    REQUIRE(s.top() == ""); // пустой стек
}

TEST_CASE("pop на пустом стеке", "[Stack]") {
    Stack s;
    REQUIRE_NOTHROW(s.pop());
    REQUIRE(s.top() == "");
}

TEST_CASE("top на пустом стеке", "[Stack]") {
    Stack s;
    REQUIRE(s.top() == "");
}

TEST_CASE("несколько операций push/pop", "[Stack]") {
    Stack s;
    s.push("x");
    s.push("y");
    s.push("z");

    REQUIRE(s.top() == "z");

    s.pop();
    REQUIRE(s.top() == "y");

    s.push("w");
    REQUIRE(s.top() == "w");

    s.pop();
    REQUIRE(s.top() == "y");

    s.pop();
    REQUIRE(s.top() == "x");

    s.pop();
    REQUIRE(s.is_empty() == true);
}

TEST_CASE("Stack — extensive edge cases", "[Stack][extra]") {
    Stack s;

    // --- pop и top на пустом стеке несколько раз ---
    std::stringstream out_empty;
    std::streambuf* old = std::cout.rdbuf(out_empty.rdbuf());
    s.pop();
    s.top();
    s.pop();
    std::cout.rdbuf(old);
    REQUIRE(out_empty.str().find("Стек пуст") != std::string::npos);

    // --- print на пустом стеке ---
    std::stringstream out_print;
    old = std::cout.rdbuf(out_print.rdbuf());
    s.print();
    std::cout.rdbuf(old);
    REQUIRE(out_print.str().empty()); // пустой стек ничего не выводит кроме endl

    // --- push и pop на одном элементе ---
    s.push("A");
    REQUIRE(s.top() == "A");
    REQUIRE(!s.is_empty());

    std::stringstream out_print1;
    old = std::cout.rdbuf(out_print1.rdbuf());
    s.print();
    std::cout.rdbuf(old);
    REQUIRE(out_print1.str().find("A") != std::string::npos);

    s.pop();
    REQUIRE(s.is_empty());
    REQUIRE(s.top() == "");

    // --- push нескольких элементов и последовательные pop ---
    s.push("B");
    s.push("C");
    s.push("D");
    REQUIRE(s.top() == "D");
    
    s.pop();
    REQUIRE(s.top() == "C");
    
    s.pop();
    REQUIRE(s.top() == "B");

    // --- push после опустошения стека ---
    s.pop();
    REQUIRE(s.is_empty());

    s.push("E");
    s.push("F");
    REQUIRE(s.top() == "F");

    // --- pop одного элемента оставляет верхний ---
    s.pop();
    REQUIRE(s.top() == "E");

    // --- удаление последнего элемента обновляет состояние ---
    s.pop();
    REQUIRE(s.is_empty());

    // --- push/pop чередование с несколькими элементами ---
    s.push("X");
    s.push("Y");
    REQUIRE(s.top() == "Y");
    s.pop();
    REQUIRE(s.top() == "X");
    s.push("Z");
    REQUIRE(s.top() == "Z");
    s.pop();
    REQUIRE(s.top() == "X");
    s.pop();
    REQUIRE(s.is_empty());

    // --- print непустого стека ---
    s.push("M");
    s.push("N");
    std::stringstream out_print2;
    old = std::cout.rdbuf(out_print2.rdbuf());
    s.print();
    std::cout.rdbuf(old);
    REQUIRE(out_print2.str().find("N") != std::string::npos);
    REQUIRE(out_print2.str().find("M") != std::string::npos);
}