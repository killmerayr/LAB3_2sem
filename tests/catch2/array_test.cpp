#include <catch2/catch_all.hpp>
#include "../../sd/array/array.hpp"
#include <iostream>
#include <sstream>


TEST_CASE("Mass — 100 coverage", "[array]") {
    Mass m;

    // --- is_empty / print empty ---
    REQUIRE(m.is_empty());
    std::stringstream out0;
    std::streambuf* old0 = std::cout.rdbuf(out0.rdbuf());
    m.print();
    std::cout.rdbuf(old0);

    // --- reserve (<= capacity и > capacity) ---
    m.reserve(0);     // <= capacity
    m.reserve(4);     // > capacity

    // --- push_back (обычный и с resize) ---
    m.push_back("A");
    m.push_back("B");
    m.push_back("C");
    m.push_back("D");
    m.push_back("E"); // resize

    REQUIRE(!m.is_empty());
    REQUIRE(m.get_size() == 5);

    // --- insert_at ---
    m.insert_at(-1, "X");     // invalid
    m.insert_at(999, "X");    // invalid
    m.insert_at(0, "HEAD");   // начало
    m.insert_at(3, "MID");    // середина
    m.insert_at(m.get_size(), "TAIL"); // index == size

    REQUIRE(m.get_at(0) == "HEAD");
    REQUIRE(m.get_at(3) == "MID");

    // --- replace_at valid + invalid ---
    m.replace_at(3, "MID2");  // valid
    m.replace_at(-1, "ERR");  // invalid
    m.replace_at(999, "ERR"); // invalid
    REQUIRE(m.get_at(3) == "MID2");

    // --- get_at valid + invalid ---
    REQUIRE(m.get_at(0) == "HEAD");
    REQUIRE(m.get_at(-1) == "");
    REQUIRE(m.get_at(999) == "");

    // --- del_at: middle ---
    int old_size = m.get_size();
    m.del_at(3);
    REQUIRE(m.get_size() == old_size - 1);

    // --- del_at: delete last element ---
    old_size = m.get_size();
    m.del_at(m.get_size() - 1);
    REQUIRE(m.get_size() == old_size - 1);

    // --- del_at: invalid ---
    m.del_at(-1);
    m.del_at(999);

    // --- print (non-empty) ---
    std::stringstream out2;
    std::streambuf* old2 = std::cout.rdbuf(out2.rdbuf());
    m.print();
    std::cout.rdbuf(old2);
    REQUIRE(!out2.str().empty());

    Mass m2;
    m2.push_back("X");
    m2.push_back("Y");
    m2.reserve(10); // > capacity, чтобы пройти for (копирование)
    REQUIRE(m2.get_size() == 2);

    // Для del_at полного покрытия цикла
    Mass m3;
    m3.push_back("A");
    m3.push_back("B");
    m3.push_back("C");
    m3.push_back("D");
    m3.del_at(1); // удаляем "B", чтобы цикл for работал 2 раза
    REQUIRE(m3.get_size() == 3);
    REQUIRE(m3.get_at(0) == "A");
    REQUIRE(m3.get_at(1) == "C");
    REQUIRE(m3.get_at(2) == "D");

    // Для push_back и resize ветки capacity*2
    Mass m4;
    for (int i = 0; i < 10; ++i) m4.push_back("val");
    REQUIRE(m4.get_size() == 10);
}

TEST_CASE("Дополнительно", "[array]"){
    // reserve с копированием нескольких элементов
    Mass m;
    m.push_back("A");
    m.push_back("B");
    m.reserve(5);   // for (i=0..1) выполняется полностью
    REQUIRE(m.get_size() == 2);

    // insert_at с циклом смещения >1
    m.insert_at(1, "X");  // вставка в середину
    m.insert_at(2, "Y");  // ещё одна вставка в середину
    REQUIRE(m.get_size() == 4);

    // del_at, удаление сдвигаемого элемента
    m.del_at(1); // цикл for выполнится >1 раза
    REQUIRE(m.get_size() == 3);

    // push_back с resize > capacity
    Mass m2;
    for (int i=0; i<10; i++) m2.push_back("val");  // сработают обе ветви resize
    REQUIRE(m2.get_size() == 10);
}

TEST_CASE("Mass — edge cases for full coverage", "[array][extra2]") {
    // --- reserve с пустым массивом ---
    Mass m;
    m.reserve(0);  // ветвь: new_capacity <= capacity
    m.reserve(5);  // for не выполняется, capacity увеличивается
    REQUIRE(m.get_size() == 0);

    // --- push_back на пустой массив и триггер resize ---
    m.push_back("A"); // обычная вставка
    m.push_back("B"); 
    m.push_back("C"); 
    m.push_back("D"); 
    m.push_back("E"); // resize
    REQUIRE(m.get_size() == 5);

    // --- del_at на массиве из одного элемента ---
    Mass m2;
    m2.push_back("X");
    REQUIRE(m2.get_size() == 1);
    m2.del_at(0); // удаление единственного элемента
    REQUIRE(m2.get_size() == 0);

    // --- insert_at и сдвиг элементов ---
    Mass m3;
    m3.push_back("A");
    m3.push_back("B");
    m3.push_back("C");
    m3.insert_at(1, "X"); // сдвиг 2 элемента
    REQUIRE(m3.get_at(1) == "X");
    REQUIRE(m3.get_size() == 4);

    // --- replace_at invalid index ---
    m3.replace_at(-1, "ERR");
    m3.replace_at(999, "ERR");
    REQUIRE(m3.get_at(-1) == "");
    REQUIRE(m3.get_at(999) == "");

    // --- get_at invalid index ---
    REQUIRE(m3.get_at(-1) == "");
    REQUIRE(m3.get_at(999) == "");

    // --- print пустого и непустого массива ---
    std::stringstream out_empty;
    std::streambuf* old_empty = std::cout.rdbuf(out_empty.rdbuf());
    Mass m4;
    m4.print();
    std::cout.rdbuf(old_empty);
    REQUIRE(out_empty.str().empty());

    std::stringstream out_nonempty;
    std::streambuf* old_nonempty = std::cout.rdbuf(out_nonempty.rdbuf());
    m3.print();
    std::cout.rdbuf(old_nonempty);
    REQUIRE(!out_nonempty.str().empty());
}