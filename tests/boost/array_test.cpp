#include <boost/test/unit_test.hpp>
#include <sstream>
#include <iostream>
#include <chrono>
#include "../../sd/array/array.hpp"

BOOST_AUTO_TEST_SUITE(ArraySuite)

BOOST_AUTO_TEST_CASE(PushBackAndGet)
{
    Mass arr;
    arr.push_back("a");
    arr.push_back("b");
    BOOST_TEST(arr.get_at(0) == "a");
    BOOST_TEST(arr.get_at(1) == "b");
}

BOOST_AUTO_TEST_CASE(InsertAndDelete)
{
    Mass arr;
    arr.push_back("x");
    arr.insert_at(0, "y");
    BOOST_TEST(arr.get_at(0) == "y");
    arr.del_at(0);
    BOOST_TEST(arr.get_at(0) == "x");
}

BOOST_AUTO_TEST_CASE(ReplaceAndSize)
{
    Mass arr;
    arr.push_back("v");
    arr.replace_at(0, "w");
    BOOST_TEST(arr.get_at(0) == "w");
    BOOST_TEST(arr.get_size() == 1);
}

BOOST_AUTO_TEST_CASE(IsEmptyTest)
{
    Mass arr;
    BOOST_TEST(arr.is_empty() == true);
    arr.push_back("a");
    BOOST_TEST(arr.is_empty() == false);
    arr.del_at(0);
    BOOST_TEST(arr.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(ReserveTest)
{
    Mass arr;
    arr.push_back("a");
    arr.reserve(1);  // ничего не должно измениться
    BOOST_TEST(arr.get_size() == 1);
    arr.reserve(10);
    BOOST_TEST(arr.get_size() == 1);  // элемент сохранён
    BOOST_TEST(arr.get_at(0) == "a");
}

BOOST_AUTO_TEST_CASE(InsertInvalidIndexTest)
{
    Mass arr;
    arr.push_back("a");
    arr.insert_at(-1, "x");   // не должно вставиться
    arr.insert_at(5, "y");    // не должно вставиться
    BOOST_TEST(arr.get_size() == 1);
    BOOST_TEST(arr.get_at(0) == "a");
}

BOOST_AUTO_TEST_CASE(DeleteInvalidIndexTest)
{
    Mass arr;
    arr.push_back("a");
    arr.del_at(-1);
    arr.del_at(5);
    BOOST_TEST(arr.get_size() == 1);
    
    // Удаление из пустого массива
    Mass empty_arr;
    empty_arr.del_at(0);
    empty_arr.del_at(-1);
    BOOST_TEST(empty_arr.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(GetInvalidIndexTest)
{
    Mass arr;
    BOOST_TEST(arr.get_at(-1) == "");
    BOOST_TEST(arr.get_at(10) == "");
    
    arr.push_back("test");
    BOOST_TEST(arr.get_at(1) == ""); // за пределами
    BOOST_TEST(arr.get_at(-1) == "");
}

BOOST_AUTO_TEST_CASE(ReplaceInvalidIndexTest)
{
    Mass arr;
    arr.push_back("a");
    arr.replace_at(-1, "x");
    arr.replace_at(10, "y");
    BOOST_TEST(arr.get_at(0) == "a"); // значение не изменилось
}

BOOST_AUTO_TEST_CASE(PrintTest)
{
    Mass arr;
    arr.push_back("hello");
    std::stringstream buf;
    std::streambuf* old = std::cout.rdbuf(buf.rdbuf()); // перенаправление cout
    arr.print();
    std::cout.rdbuf(old);
    BOOST_TEST(!buf.str().empty());
}

BOOST_AUTO_TEST_CASE(FullFlow)
{
    Mass arr;
    arr.push_back("a");
    arr.push_back("b");
    arr.insert_at(1, "x");
    arr.replace_at(0, "z");
    arr.del_at(2);
    BOOST_TEST(arr.get_size() == 2);
    BOOST_TEST(arr.get_at(0) == "z");
    BOOST_TEST(arr.get_at(1) == "x");
}

// Новые тесты для полного покрытия

BOOST_AUTO_TEST_CASE(ConstructorDestructorTest)
{
    Mass arr;
    BOOST_TEST(arr.is_empty() == true);
    BOOST_TEST(arr.get_size() == 0);
}

BOOST_AUTO_TEST_CASE(PushBackWithResizeTest)
{
    Mass arr;
    // Добавляем элементы чтобы вызвать увеличение capacity
    for (int i = 0; i < 10; i++) {
        arr.push_back("elem" + std::to_string(i));
    }
    BOOST_TEST(arr.get_size() == 10);
    for (int i = 0; i < 10; i++) {
        BOOST_TEST(arr.get_at(i) == "elem" + std::to_string(i));
    }
}

BOOST_AUTO_TEST_CASE(InsertAtDifferentPositions)
{
    Mass arr;
    // Вставка в пустой массив
    arr.insert_at(0, "first");
    BOOST_TEST(arr.get_at(0) == "first");
    
    // Вставка в начало
    arr.insert_at(0, "new_first");
    BOOST_TEST(arr.get_at(0) == "new_first");
    BOOST_TEST(arr.get_at(1) == "first");
    
    // Вставка в конец
    arr.insert_at(2, "last");
    BOOST_TEST(arr.get_at(2) == "last");
    
    // Вставка в середину
    arr.insert_at(1, "middle");
    BOOST_TEST(arr.get_at(1) == "middle");
    BOOST_TEST(arr.get_size() == 4);
}

BOOST_AUTO_TEST_CASE(DeleteAtDifferentPositions)
{
    Mass arr;
    arr.push_back("a");
    arr.push_back("b");
    arr.push_back("c");
    arr.push_back("d");
    
    // Удаление из середины
    arr.del_at(1);
    BOOST_TEST(arr.get_size() == 3);
    BOOST_TEST(arr.get_at(0) == "a");
    BOOST_TEST(arr.get_at(1) == "c");
    BOOST_TEST(arr.get_at(2) == "d");
    
    // Удаление из начала
    arr.del_at(0);
    BOOST_TEST(arr.get_size() == 2);
    BOOST_TEST(arr.get_at(0) == "c");
    BOOST_TEST(arr.get_at(1) == "d");
    
    // Удаление из конца
    arr.del_at(1);
    BOOST_TEST(arr.get_size() == 1);
    BOOST_TEST(arr.get_at(0) == "c");
}

BOOST_AUTO_TEST_CASE(ReplaceAtDifferentPositions)
{
    Mass arr;
    arr.push_back("old1");
    arr.push_back("old2");
    arr.push_back("old3");
    
    arr.replace_at(0, "new1");
    BOOST_TEST(arr.get_at(0) == "new1");
    
    arr.replace_at(1, "new2");
    BOOST_TEST(arr.get_at(1) == "new2");
    
    arr.replace_at(2, "new3");
    BOOST_TEST(arr.get_at(2) == "new3");
}

BOOST_AUTO_TEST_CASE(ReserveEdgeCases)
{
    Mass arr;
    // reserve с capacity = 0
    arr.reserve(5);
    BOOST_TEST(arr.is_empty() == true);
    
    // reserve с new_capacity <= capacity
    arr.push_back("test");
    int current_size = arr.get_size();
    arr.reserve(1); // меньше текущего capacity
    BOOST_TEST(arr.get_size() == current_size);
    BOOST_TEST(arr.get_at(0) == "test");
}

BOOST_AUTO_TEST_CASE(EmptyArrayOperations)
{
    Mass arr;
    BOOST_TEST(arr.is_empty() == true);
    BOOST_TEST(arr.get_size() == 0);
    BOOST_TEST(arr.get_at(0) == "");
    BOOST_TEST(arr.get_at(-1) == "");
    BOOST_TEST(arr.get_at(100) == "");
    
    // Операции не должны падать
    arr.del_at(0);
    arr.del_at(-1);
    arr.replace_at(0, "test");
    arr.replace_at(-1, "test");
    
    BOOST_TEST(arr.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(SingleElementOperations)
{
    Mass arr;
    arr.push_back("single");
    
    BOOST_TEST(arr.get_at(0) == "single");
    BOOST_TEST(arr.get_at(-1) == "");
    BOOST_TEST(arr.get_at(1) == "");
    
    arr.replace_at(0, "modified");
    BOOST_TEST(arr.get_at(0) == "modified");
    
    arr.replace_at(-1, "invalid");
    arr.replace_at(1, "invalid");
    BOOST_TEST(arr.get_at(0) == "modified"); // не изменилось
    
    arr.del_at(0);
    BOOST_TEST(arr.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(ReadMethodTest)
{
    Mass arr;
    
    // Мокаем ввод
    std::istringstream test_input("3\nElement1\nElement2\nElement3\n");
    std::streambuf* old_cin = std::cin.rdbuf(test_input.rdbuf());
    
    std::stringstream output_buffer;
    std::streambuf* old_cout = std::cout.rdbuf(output_buffer.rdbuf());
    
    arr.read();
    
    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);
    
    BOOST_TEST(arr.get_size() == 3);
    BOOST_TEST(arr.get_at(0) == "Element1");
    BOOST_TEST(arr.get_at(1) == "Element2");
    BOOST_TEST(arr.get_at(2) == "Element3");
}

BOOST_AUTO_TEST_CASE(ReadZeroElementsTest)
{
    Mass arr;
    
    std::istringstream test_input("0\n");
    std::streambuf* old_cin = std::cin.rdbuf(test_input.rdbuf());
    
    std::stringstream output_buffer;
    std::streambuf* old_cout = std::cout.rdbuf(output_buffer.rdbuf());
    
    arr.read();
    
    std::cin.rdbuf(old_cin);
    std::cout.rdbuf(old_cout);
    
    BOOST_TEST(arr.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(ComplexSequenceTest)
{
    Mass arr;
    
    // Комплексная последовательность операций
    arr.push_back("start");
    arr.insert_at(0, "before_start");
    arr.push_back("end");
    arr.insert_at(2, "middle");
    
    BOOST_TEST(arr.get_size() == 4);
    BOOST_TEST(arr.get_at(0) == "before_start");
    BOOST_TEST(arr.get_at(1) == "start");
    BOOST_TEST(arr.get_at(2) == "middle");
    BOOST_TEST(arr.get_at(3) == "end");
    
    arr.replace_at(1, "modified_start");
    arr.del_at(2);
    
    BOOST_TEST(arr.get_size() == 3);
    BOOST_TEST(arr.get_at(0) == "before_start");
    BOOST_TEST(arr.get_at(1) == "modified_start");
    BOOST_TEST(arr.get_at(2) == "end");
    
    // Печать не должна падать
    std::stringstream buf;
    std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
    arr.print();
    std::cout.rdbuf(old);
    BOOST_TEST(!buf.str().empty());
}

BOOST_AUTO_TEST_CASE(MultipleInsertDeleteTest)
{
    Mass arr;
    
    // Множественные вставки и удаления
    for (int i = 0; i < 10; i++) {
        arr.push_back("elem" + std::to_string(i));
    }
    
    BOOST_TEST(arr.get_size() == 10);
    
    // Удаляем каждый второй элемент
    for (int i = 4; i >= 0; i--) {
        arr.del_at(i * 2);
    }
    
    BOOST_TEST(arr.get_size() == 5);
    
    // Вставляем новые элементы
    for (int i = 0; i < 3; i++) {
        arr.insert_at(i * 2, "new" + std::to_string(i));
    }
    
    BOOST_TEST(arr.get_size() == 8);
    
    // Проверяем что не падает
    std::stringstream buf;
    std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
    arr.print();
    std::cout.rdbuf(old);
}

// ===== БЕНЧМАРКИ =====
BOOST_AUTO_TEST_CASE(BENCHMARK_PushBack, * boost::unit_test::label("benchmark"))
{
    auto start = std::chrono::high_resolution_clock::now();
    
    Mass arr;
    for (int i = 0; i < 10000; ++i) {
        arr.push_back("element_" + std::to_string(i));
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    BOOST_TEST_MESSAGE("push_back x10000: " << duration.count() << " ms");
    BOOST_TEST(arr.get_size() == 10000);
}

BOOST_AUTO_TEST_CASE(BENCHMARK_InsertAt, * boost::unit_test::label("benchmark"))
{
    Mass arr;
    for (int i = 0; i < 1000; ++i) {
        arr.push_back("elem_" + std::to_string(i));
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 100; ++i) {
        arr.insert_at(500, "inserted_" + std::to_string(i));
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    BOOST_TEST_MESSAGE("insert_at x100 (middle): " << duration.count() << " ms");
    BOOST_TEST(arr.get_size() == 1100);
}

BOOST_AUTO_TEST_CASE(BENCHMARK_DeleteAt, * boost::unit_test::label("benchmark"))
{
    Mass arr;
    for (int i = 0; i < 5000; ++i) {
        arr.push_back("elem_" + std::to_string(i));
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 1000; ++i) {
        arr.del_at(arr.get_size() / 2);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    BOOST_TEST_MESSAGE("del_at x1000 (middle): " << duration.count() << " ms");
    BOOST_TEST(arr.get_size() == 4000);
}

BOOST_AUTO_TEST_CASE(BENCHMARK_GetAt, * boost::unit_test::label("benchmark"))
{
    Mass arr;
    for (int i = 0; i < 5000; ++i) {
        arr.push_back("elem_" + std::to_string(i));
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 100000; ++i) {
        std::string val = arr.get_at(i % arr.get_size());
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    BOOST_TEST_MESSAGE("get_at x100000: " << duration.count() << " ms");
}

BOOST_AUTO_TEST_CASE(BENCHMARK_MixedOperations, * boost::unit_test::label("benchmark"))
{
    auto start = std::chrono::high_resolution_clock::now();
    
    Mass arr;
    for (int i = 0; i < 1000; ++i) {
        arr.push_back("elem_" + std::to_string(i));
    }
    
    for (int i = 0; i < 500; ++i) {
        arr.insert_at(arr.get_size() / 2, "new_" + std::to_string(i));
        arr.del_at(0);
        arr.replace_at(arr.get_size() - 1, "modified");
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    BOOST_TEST_MESSAGE("mixed operations (insert+delete+replace x500): " << duration.count() << " ms");
}

BOOST_AUTO_TEST_SUITE_END()