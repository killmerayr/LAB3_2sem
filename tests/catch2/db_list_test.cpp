#include <catch2/catch_all.hpp>
#include "../../sd/db_list/db_list.hpp"

TEST_CASE("Конструктор и is_empty", "[DoublyList]") {
    DoublyList l;
    REQUIRE(l.is_empty() == true);
    REQUIRE(l.get_size() == 0);
}

TEST_CASE("push_back и get_size", "[DoublyList]") {
    DoublyList l;
    l.push_back("a");
    l.push_back("b");
    REQUIRE(l.get_size() == 2);
    REQUIRE(l.get_at(0)->data == "a");
    REQUIRE(l.get_at(1)->data == "b");
}

TEST_CASE("push_front", "[DoublyList]") {
    DoublyList l;
    l.push_front("a");
    l.push_front("b");
    REQUIRE(l.get_size() == 2);
    REQUIRE(l.get_at(0)->data == "b");
    REQUIRE(l.get_at(1)->data == "a");
}

TEST_CASE("insert_after", "[DoublyList]") {
    DoublyList l;
    l.push_back("a");
    l.push_back("b");
    l.insert_after("a", "x");
    REQUIRE(l.get_size() == 3);
    REQUIRE(l.get_at(1)->data == "x");

    // вставка после хвоста
    l.insert_after("b", "y");
    REQUIRE(l.get_at(3)->data == "y");

    // вставка после несуществующего ключа
    l.insert_after("z", "bad"); // ничего не должно произойти
    REQUIRE(l.get_size() == 4);
}

TEST_CASE("insert_before", "[DoublyList]") {
    DoublyList l;
    l.push_back("a");
    l.push_back("b");
    l.insert_before("b", "x");
    REQUIRE(l.get_size() == 3);
    REQUIRE(l.get_at(1)->data == "x");

    // вставка перед головой
    l.insert_before("a", "y");
    REQUIRE(l.get_at(0)->data == "y");

    // вставка перед несуществующим ключом
    l.insert_before("z", "bad");
    REQUIRE(l.get_size() == 4);
}

TEST_CASE("del_head и del_tail", "[DoublyList]") {
    DoublyList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");

    l.del_head();
    REQUIRE(l.get_size() == 2);
    REQUIRE(l.get_at(0)->data == "b");

    l.del_tail();
    REQUIRE(l.get_size() == 1);
    REQUIRE(l.get_at(0)->data == "b");

    l.del_head();
    REQUIRE(l.is_empty() == true);
}

TEST_CASE("del_after", "[DoublyList]") {
    DoublyList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");

    l.del_after("a");
    REQUIRE(l.get_size() == 2);
    REQUIRE(l.get_at(1)->data == "c");

    // удаление после несуществующего ключа
    l.del_after("z");
    REQUIRE(l.get_size() == 2);
}

TEST_CASE("del_before", "[DoublyList]") {
    DoublyList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    l.push_back("d");

    l.del_before("c");
    REQUIRE(l.get_size() == 3);
    REQUIRE(l.get_at(1)->data == "c");

    // удаление перед головой (не должно ничего делать)
    l.del_before("a");
    REQUIRE(l.get_size() == 3);
}

TEST_CASE("del по значению", "[DoublyList]") {
    DoublyList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");

    l.del("b");
    REQUIRE(l.get_size() == 2);
    REQUIRE(l.get_at(1)->data == "c");

    // удаление несуществующего элемента
    l.del("x");
    REQUIRE(l.get_size() == 2);
}

TEST_CASE("find и get_at", "[DoublyList]") {
    DoublyList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");

    auto node = l.find("b");
    REQUIRE(node != nullptr);
    REQUIRE(node->data == "b");

    REQUIRE(l.find("x") == nullptr);
    REQUIRE(l.get_at(-1) == nullptr);
    REQUIRE(l.get_at(10) == nullptr);
}

TEST_CASE("DoublyList — edge cases for full coverage", "[DoublyList][extra]") {
    DoublyList l;

    // --- операции на пустом списке ---
    l.del_head();    // ничего не должно произойти
    l.del_tail();    // ничего не должно произойти
    l.del_after("x");
    l.del_before("x");
    l.del("x");
    REQUIRE(l.is_empty());

    // --- вставка в пустой список через push_front/push_back ---
    l.push_front("A");
    REQUIRE(l.get_size() == 1);
    REQUIRE(l.get_at(0)->data == "A");

    l.del_tail();
    REQUIRE(l.is_empty());

    l.push_back("B");
    REQUIRE(l.get_size() == 1);
    REQUIRE(l.get_at(0)->data == "B");

    // --- insert_after/insert_before на единственном элементе ---
    l.insert_after("B", "C");  // вставка после хвоста
    REQUIRE(l.get_size() == 2);
    REQUIRE(l.get_at(1)->data == "C");

    l.insert_before("B", "A"); // вставка перед головой
    REQUIRE(l.get_size() == 3);
    REQUIRE(l.get_at(0)->data == "A");

    // --- del_before/after крайние случаи ---
    l.del_after("C"); // удаление после хвоста (ничего)
    REQUIRE(l.get_size() == 3);

    l.del_before("A"); // удаление перед головой (ничего)
    REQUIRE(l.get_size() == 3);

    // --- del_head/del_tail на списке с 3 элементами ---
    l.del_head();
    REQUIRE(l.get_size() == 2);
    l.del_tail();
    REQUIRE(l.get_size() == 1);
    l.del_head();
    REQUIRE(l.is_empty());

    // --- find на пустом списке ---
    REQUIRE(l.find("A") == nullptr);
}