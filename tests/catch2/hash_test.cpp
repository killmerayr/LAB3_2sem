#include <catch2/catch_all.hpp>
#include "../../sd/hash/hash.hpp"

TEST_CASE("Конструктор и пустая таблица", "[HashTable]") {
    HashTable<string> ht;
    REQUIRE(ht.get_size() == 0);
    REQUIRE(ht.get_capacity() == 8);
}

TEST_CASE("Добавление элементов и Find", "[HashTable]") {
    HashTable<string> ht;
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
    HashTable<string> ht;
    ht.Add("a");
    ht.Add("b");

    REQUIRE(ht.Remove("a") == true);
    REQUIRE(ht.get_size() == 1);
    REQUIRE(ht.Find("a") == false);

    REQUIRE(ht.Remove("x") == false); // элемент отсутствует
    REQUIRE(ht.get_size() == 1);
}

TEST_CASE("Добавление после удаления (re-add)", "[HashTable]") {
    HashTable<string> ht;
    ht.Add("a");
    ht.Remove("a");
    REQUIRE(ht.Add("a") == true); // можно добавить снова
    REQUIRE(ht.Find("a") == true);
    REQUIRE(ht.get_size() == 1);
}

TEST_CASE("Resize при превышении нагрузки", "[HashTable]") {
    HashTable<string> ht;
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
    HashTable<string> ht;
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