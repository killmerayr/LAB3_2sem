#include <catch2/catch_all.hpp>
#include "../../sd/list/list.hpp"
#include <string>
#include <chrono>

TEST_CASE("Конструктор и is_empty", "[List]") {
    List l;
    REQUIRE(l.is_empty() == true);
    REQUIRE(l.get_size() == 0);
}

TEST_CASE("push_back и get_size", "[List]") {
    List l;
    l.push_back("a");
    l.push_back("b");
    REQUIRE(l.get_size() == 2);
    REQUIRE(l.get_at(0)->data == "a");
    REQUIRE(l.get_at(1)->data == "b");
}

TEST_CASE("push_front", "[List]") {
    List l;
    l.push_front("a");
    l.push_front("b");
    REQUIRE(l.get_size() == 2);
    REQUIRE(l.get_at(0)->data == "b");
    REQUIRE(l.get_at(1)->data == "a");
}

TEST_CASE("insert_after", "[List]") {
    List l;
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

TEST_CASE("insert_before", "[List]") {
    List l;
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

TEST_CASE("del_head и del_tail", "[List]") {
    List l;
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

TEST_CASE("del_after", "[List]") {
    List l;
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

TEST_CASE("del_before", "[List]") {
    List l;
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

TEST_CASE("del по значению", "[List]") {
    List l;
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

TEST_CASE("find и get_at", "[List]") {
    List l;
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

TEST_CASE("List — edge cases for full coverage", "[List][extra]") {
    List l;

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
    l.insert_after("B", "C");  // вставка после tail
    REQUIRE(l.get_size() == 2);
    REQUIRE(l.get_at(1)->data == "C");

    l.insert_before("B", "A"); // вставка перед head
    REQUIRE(l.get_size() == 3);
    REQUIRE(l.get_at(0)->data == "A");

    // --- del_before/after крайние случаи ---
    l.del_after("C"); // удаление после tail (ничего)
    REQUIRE(l.get_size() == 3);

    l.del_before("A"); // удаление перед head (ничего)
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

TEST_CASE("List — stress test with many operations", "[List][stress]") {
    List l;
    
    // --- много push_back операций ---
    for (int i = 0; i < 50; ++i) {
        l.push_back("elem" + std::to_string(i));
    }
    REQUIRE(l.get_size() == 50);
    
    // --- поиск элементов ---
    for (int i = 0; i < 50; ++i) {
        auto node = l.find("elem" + std::to_string(i));
        REQUIRE(node != nullptr);
    }
    
    // --- удаление части элементов ---
    for (int i = 0; i < 25; ++i) {
        l.del("elem" + std::to_string(i));
    }
    REQUIRE(l.get_size() == 25);
    
    // --- проверка остальных ---
    for (int i = 25; i < 50; ++i) {
        auto node = l.find("elem" + std::to_string(i));
        REQUIRE(node != nullptr);
    }
}

TEST_CASE("List — complex insertions", "[List][extra2]") {
    List l;
    l.push_back("b");
    l.push_back("d");
    
    // --- insert_before и insert_after в середину ---
    l.insert_after("b", "c");
    REQUIRE(l.get_size() == 3);
    REQUIRE(l.get_at(1)->data == "c");
    
    l.insert_before("d", "c2");
    REQUIRE(l.get_size() == 4);
    
    // --- проверка порядка ---
    REQUIRE(l.get_at(0)->data == "b");
    REQUIRE(l.get_at(1)->data == "c");
    REQUIRE(l.get_at(2)->data == "c2");
    REQUIRE(l.get_at(3)->data == "d");
    
    // --- del_after и del_before ---
    l.del_after("b");  // удалит "c"
    REQUIRE(l.get_size() == 3);
    REQUIRE(l.get_at(1)->data == "c2");
    
    l.del_before("d");  // удалит "c2"
    REQUIRE(l.get_size() == 2);
    REQUIRE(l.get_at(0)->data == "b");
    REQUIRE(l.get_at(1)->data == "d");
}

// ===== BENCHMARKS =====
TEST_CASE("BENCHMARK_List_PushBack", "[benchmark]") {
    auto start = std::chrono::high_resolution_clock::now();
    List l;
    for (int i = 0; i < 10000; ++i) l.push_back("elem_" + std::to_string(i));
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    INFO("push_back x10000: " << ms << " ms");
}

TEST_CASE("BENCHMARK_List_Find", "[benchmark]") {
    List l;
    for (int i = 0; i < 1000; ++i) l.push_back("elem_" + std::to_string(i));
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 50000; ++i) l.find("elem_500");
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    INFO("find x50000: " << ms << " ms");
}