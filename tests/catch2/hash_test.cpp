#include <catch2/catch_all.hpp>
#include "../../sd/hash/hash.hpp"
#include <string>
#include <chrono>

using namespace std;

TEST_CASE("Конструктор и пустая таблица", "[HashTable]") {
    HashTable ht;
    REQUIRE(ht.get_size() == 0);
    REQUIRE(ht.get_capacity() == 8);
}

TEST_CASE("Добавление элементов и Find", "[HashTable]") {
    HashTable ht;
    REQUIRE(ht.Add("a") == true);
    REQUIRE(ht.Add("b") == true);
    REQUIRE(ht.Add("c") == true);
    REQUIRE(ht.get_size() == 3);

    REQUIRE(ht.Find("a") == true);
    REQUIRE(ht.Find("b") == true);
    REQUIRE(ht.Find("c") == true);
    REQUIRE(ht.Find("x") == false);

    // повторная вставка того же элемента
    REQUIRE(ht.Add("a") == false);
}

TEST_CASE("Удаление элементов", "[HashTable]") {
    HashTable ht;
    ht.Add("a");
    ht.Add("b");

    REQUIRE(ht.Remove("a") == true);
    REQUIRE(ht.get_size() == 1);
    REQUIRE(ht.Find("a") == false);

    REQUIRE(ht.Remove("x") == false); // элемент отсутствует
    REQUIRE(ht.get_size() == 1);
}

TEST_CASE("Добавление после удаления (re-add)", "[HashTable]") {
    HashTable ht;
    ht.Add("a");
    ht.Remove("a");
    REQUIRE(ht.Add("a") == true); // можно добавить снова
    REQUIRE(ht.Find("a") == true);
    REQUIRE(ht.get_size() == 1);
}

TEST_CASE("Resize при превышении нагрузки", "[HashTable]") {
    HashTable ht;
    // добавляем больше элементов, чтобы сработал Resize
    for (int i = 0; i < 10; ++i) {
        ht.Add("val" + to_string(i));
    }
    REQUIRE(ht.get_capacity() > 8);
    REQUIRE(ht.get_size() == 10);
    for (int i = 0; i < 10; ++i)
        REQUIRE(ht.Find("val" + to_string(i)) == true);
}

TEST_CASE("Rehash при много удалённых элементов", "[HashTable]") {
    HashTable ht;
    for (int i = 0; i < 6; ++i)
        ht.Add("val" + to_string(i));

    // удаляем элементы, чтобы сработал Rehash
    for (int i = 0; i < 4; ++i)
        ht.Remove("val" + to_string(i));

    // добавляем новые элементы, чтобы инициировать Rehash
    for (int i = 6; i < 8; ++i)
        ht.Add("val" + to_string(i));

    // проверяем, что все активные элементы доступны
    for (int i = 4; i < 8; ++i)
        REQUIRE(ht.Find("val" + to_string(i)) == true);
}

TEST_CASE("HashTable — edge cases for full coverage", "[HashTable][extra]") {
    HashTable ht;
    
    // --- конструктор с параметром capacity ---
    HashTable ht2(16);
    REQUIRE(ht2.get_capacity() == 16);
    REQUIRE(ht2.get_size() == 0);
    
    // --- добавление и удаление одного элемента ---
    REQUIRE(ht.Add("single") == true);
    REQUIRE(ht.get_size() == 1);
    REQUIRE(ht.Remove("single") == true);
    REQUIRE(ht.get_size() == 0);
    
    // --- много коллизий (linear probing) ---
    for (int i = 0; i < 20; ++i) {
        ht.Add("elem" + to_string(i));
    }
    REQUIRE(ht.get_size() == 20);
    
    // --- проверка поиска всех элементов после коллизий ---
    for (int i = 0; i < 20; ++i) {
        REQUIRE(ht.Find("elem" + to_string(i)) == true);
    }
    
    // --- удаление части элементов с коллизиями ---
    for (int i = 0; i < 10; ++i) {
        ht.Remove("elem" + to_string(i));
    }
    REQUIRE(ht.get_size() == 10);
    
    // --- проверка остальных элементов ---
    for (int i = 10; i < 20; ++i) {
        REQUIRE(ht.Find("elem" + to_string(i)) == true);
    }
    for (int i = 0; i < 10; ++i) {
        REQUIRE(ht.Find("elem" + to_string(i)) == false);
    }
}

TEST_CASE("HashTable — stress test with operations", "[HashTable][stress]") {
    HashTable ht;
    
    // --- большое количество вставок и проверка поиска ---
    for (int i = 0; i < 50; ++i) {
        REQUIRE(ht.Add("item" + to_string(i)) == true);
    }
    REQUIRE(ht.get_size() == 50);
    
    // --- поиск всех элементов ---
    for (int i = 0; i < 50; ++i) {
        REQUIRE(ht.Find("item" + to_string(i)) == true);
    }
    
    // --- попытка добавить существующие (дубликаты) ---
    for (int i = 0; i < 50; ++i) {
        REQUIRE(ht.Add("item" + to_string(i)) == false);
    }
    REQUIRE(ht.get_size() == 50);
    
    // --- удаление половины ---
    for (int i = 0; i < 25; ++i) {
        REQUIRE(ht.Remove("item" + to_string(i)) == true);
    }
    REQUIRE(ht.get_size() == 25);
    
    // --- проверка что удалённые не найдутся ---
    for (int i = 0; i < 25; ++i) {
        REQUIRE(ht.Find("item" + to_string(i)) == false);
    }
    
    // --- проверка что оставшиеся найдутся ---
    for (int i = 25; i < 50; ++i) {
        REQUIRE(ht.Find("item" + to_string(i)) == true);
    }
    
    // --- повторная вставка удалённых ---
    for (int i = 0; i < 25; ++i) {
        REQUIRE(ht.Add("item" + to_string(i)) == true);
    }
    REQUIRE(ht.get_size() == 50);
}

TEST_CASE("HashTable — removal and find edge cases", "[HashTable][extra2]") {
    HashTable ht;
    
    // --- удаление несуществующего элемента из пустой таблицы ---
    REQUIRE(ht.Remove("nonexistent") == false);
    
    // --- поиск несуществующего элемента ---
    REQUIRE(ht.Find("nothere") == false);
    
    // --- одна вставка и удаление ---
    ht.Add("x");
    REQUIRE(ht.Find("x") == true);
    REQUIRE(ht.Remove("x") == true);
    REQUIRE(ht.Find("x") == false);
    REQUIRE(ht.get_size() == 0);
    
    // --- множественные удаления одного элемента (второе должно вернуть false) ---
    ht.Add("y");
    REQUIRE(ht.Remove("y") == true);
    REQUIRE(ht.Remove("y") == false);
}

// ===== BENCHMARKS =====
TEST_CASE("BENCHMARK_Hash_Add", "[benchmark]") {
    auto start = std::chrono::high_resolution_clock::now();
    HashTable h;
    for (int i = 0; i < 10000; ++i) h.Add("key_" + std::to_string(i));
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    INFO("Add x10000: " << ms << " ms");
}

TEST_CASE("BENCHMARK_Hash_Find", "[benchmark]") {
    HashTable h;
    for (int i = 0; i < 1000; ++i) h.Add("key_" + std::to_string(i));
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000; ++i) h.Find("key_" + std::to_string(i % 1000));
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    INFO("Find x100000: " << ms << " ms");
}