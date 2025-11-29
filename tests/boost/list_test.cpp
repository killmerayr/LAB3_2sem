#include <boost/test/unit_test.hpp>
#include <sstream>
#include <iostream>
#include "../../sd/list/list.hpp"

BOOST_AUTO_TEST_SUITE(ListSuite)

BOOST_AUTO_TEST_CASE(ConstructorAndDestructor)
{
    List l;
    BOOST_TEST(l.is_empty() == true);
    BOOST_TEST(l.get_size() == 0);
}

BOOST_AUTO_TEST_CASE(PushBackAndFind)
{
    List l;
    l.push_back("a");
    l.push_back("b");
    BOOST_CHECK(l.find("a") != nullptr);
    BOOST_CHECK(l.find("b") != nullptr);
    BOOST_CHECK(l.find("c") == nullptr);
    BOOST_TEST(l.get_size() == 2);
}

BOOST_AUTO_TEST_CASE(PushFront)
{
    List l;
    l.push_front("b");
    l.push_front("a");
    BOOST_TEST(l.find("a") != nullptr);
    BOOST_TEST(l.find("b") != nullptr);
    BOOST_TEST(l.get_size() == 2);
    
    // Проверка порядка
    auto first = l.get_at(0);
    auto second = l.get_at(1);
    BOOST_TEST(first->data == "a");
    BOOST_TEST(second->data == "b");
}

BOOST_AUTO_TEST_CASE(DelHeadAndTail)
{
    List l;
    l.push_back("x");
    l.push_back("y");
    l.del_head();
    BOOST_CHECK(l.find("x") == nullptr);
    BOOST_CHECK(l.find("y") != nullptr);
    l.del_tail();
    BOOST_CHECK(l.find("y") == nullptr);
    BOOST_TEST(l.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(InsertAfter)
{
    List l;
    l.push_back("a");
    l.push_back("c");
    
    // Вставка после существующего элемента
    l.insert_after("a", "b");
    BOOST_TEST(l.get_size() == 3);
    BOOST_TEST(l.find("b") != nullptr);
    
    // Проверка порядка
    BOOST_TEST(l.get_at(0)->data == "a");
    BOOST_TEST(l.get_at(1)->data == "b");
    BOOST_TEST(l.get_at(2)->data == "c");
    
    // Вставка после последнего элемента
    l.insert_after("c", "d");
    BOOST_TEST(l.find("d") != nullptr);
    BOOST_TEST(l.get_at(3)->data == "d");
    
    // Вставка после несуществующего элемента
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    l.insert_after("nonexistent", "x");
    std::cout.rdbuf(old);
    BOOST_TEST(!buffer.str().empty()); // должно быть сообщение об ошибке
    BOOST_TEST(l.get_size() == 4); // размер не изменился
}

BOOST_AUTO_TEST_CASE(InsertBefore)
{
    List l;
    l.push_back("b");
    l.push_back("c");
    
    // Вставка перед первым элементом
    l.insert_before("b", "a");
    BOOST_TEST(l.get_size() == 3);
    BOOST_TEST(l.find("a") != nullptr);
    BOOST_TEST(l.get_at(0)->data == "a");
    
    // Вставка в середину
    l.insert_before("c", "b2");
    BOOST_TEST(l.find("b2") != nullptr);
    BOOST_TEST(l.get_at(2)->data == "b2");
    
    // Вставка перед несуществующим элементом
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    l.insert_before("nonexistent", "x");
    std::cout.rdbuf(old);
    BOOST_TEST(!buffer.str().empty());
}

BOOST_AUTO_TEST_CASE(DeleteByValue)
{
    List l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    
    // Удаление из середины
    l.del("b");
    BOOST_TEST(l.get_size() == 2);
    BOOST_TEST(l.find("b") == nullptr);
    BOOST_TEST(l.find("a") != nullptr);
    BOOST_TEST(l.find("c") != nullptr);
    
    // Удаление первого элемента
    l.del("a");
    BOOST_TEST(l.get_size() == 1);
    BOOST_TEST(l.find("a") == nullptr);
    BOOST_TEST(l.find("c") != nullptr);
    
    // Удаление последнего элемента
    l.del("c");
    BOOST_TEST(l.is_empty() == true);
    
    // Удаление несуществующего элемента
    l.push_back("test");
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    l.del("nonexistent");
    std::cout.rdbuf(old);
    BOOST_TEST(!buffer.str().empty());
    BOOST_TEST(l.get_size() == 1); // размер не изменился
}

BOOST_AUTO_TEST_CASE(DeleteHeadEdgeCases)
{
    List l;
    
    // Удаление из пустого списка
    l.del_head();
    BOOST_TEST(l.is_empty() == true);
    
    // Удаление из списка с одним элементом
    l.push_back("only");
    l.del_head();
    BOOST_TEST(l.is_empty() == true);
    BOOST_TEST(l.get_size() == 0);
    
    // Удаление из списка с несколькими элементами
    l.push_back("first");
    l.push_back("second");
    l.push_back("third");
    l.del_head();
    BOOST_TEST(l.get_size() == 2);
    BOOST_TEST(l.find("first") == nullptr);
    BOOST_TEST(l.find("second") != nullptr);
}

BOOST_AUTO_TEST_CASE(DeleteTailEdgeCases)
{
    List l;
    
    // Удаление из пустого списка
    l.del_tail();
    BOOST_TEST(l.is_empty() == true);
    
    // Удаление из списка с одним элементом
    l.push_back("only");
    l.del_tail();
    BOOST_TEST(l.is_empty() == true);
    
    // Удаление из списка с двумя элементами
    l.push_back("first");
    l.push_back("second");
    l.del_tail();
    BOOST_TEST(l.get_size() == 1);
    BOOST_TEST(l.find("first") != nullptr);
    BOOST_TEST(l.find("second") == nullptr);
    
    // Удаление из списка с несколькими элементами
    l.push_back("second");
    l.push_back("third");
    l.del_tail();
    BOOST_TEST(l.get_size() == 2);
    BOOST_TEST(l.find("third") == nullptr);
}

BOOST_AUTO_TEST_CASE(DeleteAfter)
{
    List l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    
    // Удаление после существующего элемента
    l.del_after("a");
    BOOST_TEST(l.get_size() == 2);
    BOOST_TEST(l.find("b") == nullptr);
    BOOST_TEST(l.find("a") != nullptr);
    BOOST_TEST(l.find("c") != nullptr);
    
    // Удаление после последнего элемента (ничего не должно произойти)
    l.del_after("c");
    BOOST_TEST(l.get_size() == 2);
    
    // Удаление после несуществующего элемента
    l.del_after("nonexistent");
    BOOST_TEST(l.get_size() == 2);
    
    // Удаление когда следующий элемент - хвост
    l.del_after("a");
    BOOST_TEST(l.get_size() == 1);
    BOOST_TEST(l.find("c") == nullptr);
}

BOOST_AUTO_TEST_CASE(DeleteBefore)
{
    List l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    
    // Удаление перед существующим элементом
    l.del_before("b");
    BOOST_TEST(l.get_size() == 2);
    BOOST_TEST(l.find("a") == nullptr);
    BOOST_TEST(l.find("b") != nullptr);
    BOOST_TEST(l.find("c") != nullptr);
    
    // Удаление перед первым элементом (ничего не должно произойти)
    l.del_before("b");
    BOOST_TEST(l.get_size() == 2);
    
    // Удаление перед несуществующим элементом
    l.del_before("nonexistent");
    BOOST_TEST(l.get_size() == 2);
    
    // Удаление перед последним элементом
    l.del_before("c");
    BOOST_TEST(l.get_size() == 1);
    BOOST_TEST(l.find("b") == nullptr);
    BOOST_TEST(l.find("c") != nullptr);
}

BOOST_AUTO_TEST_CASE(GetAtIndex)
{
    List l;
    
    // Получение из пустого списка
    BOOST_TEST(l.get_at(0) == nullptr);
    BOOST_TEST(l.get_at(-1) == nullptr);
    BOOST_TEST(l.get_at(100) == nullptr);
    
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    
    // Корректные индексы
    BOOST_TEST(l.get_at(0)->data == "a");
    BOOST_TEST(l.get_at(1)->data == "b");
    BOOST_TEST(l.get_at(2)->data == "c");
    
    // Некорректные индексы
    BOOST_TEST(l.get_at(-1) == nullptr);
    BOOST_TEST(l.get_at(3) == nullptr);
    BOOST_TEST(l.get_at(100) == nullptr);
}

BOOST_AUTO_TEST_CASE(PrintTest)
{
    List l;
    
    // Печать пустого списка
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    l.print();
    std::cout.rdbuf(old);
    // Не падает - успех
    
    // Печать непустого списка
    l.push_back("hello");
    l.push_back("world");
    
    std::stringstream buffer2;
    old = std::cout.rdbuf(buffer2.rdbuf());
    l.print();
    std::cout.rdbuf(old);
    
    std::string output = buffer2.str();
    BOOST_TEST(!output.empty());
    BOOST_TEST(output.find("hello") != std::string::npos);
    BOOST_TEST(output.find("world") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(ComplexOperations)
{
    List l;
    
    // Создание сложной структуры
    l.push_back("start");
    l.push_front("before_start");
    l.push_back("end");
    l.insert_after("start", "middle");
    l.insert_before("end", "before_end");
    
    BOOST_TEST(l.get_size() == 5);
    
    // Проверка порядка
    BOOST_TEST(l.get_at(0)->data == "before_start");
    BOOST_TEST(l.get_at(1)->data == "start");
    BOOST_TEST(l.get_at(2)->data == "middle");
    BOOST_TEST(l.get_at(3)->data == "before_end");
    BOOST_TEST(l.get_at(4)->data == "end");
    
    // Удаление в разном порядке
    l.del("middle");
    l.del_head();
    l.del_tail();
    
    BOOST_TEST(l.get_size() == 2);
    BOOST_TEST(l.get_at(0)->data == "start");
    BOOST_TEST(l.get_at(1)->data == "before_end");
}

BOOST_AUTO_TEST_CASE(EdgeCases)
{
    List l;
    
    // Все операции с пустым списком
    BOOST_TEST(l.is_empty() == true);
    BOOST_TEST(l.get_size() == 0);
    BOOST_TEST(l.find("anything") == nullptr);
    BOOST_TEST(l.get_at(0) == nullptr);
    
    l.del("nonexistent");
    l.del_head();
    l.del_tail();
    l.del_after("nonexistent");
    l.del_before("nonexistent");
    
    // Остается пустым
    BOOST_TEST(l.is_empty() == true);
    
    // Работа с одним элементом
    l.push_back("single");
    BOOST_TEST(l.get_size() == 1);
    BOOST_TEST(l.find("single") != nullptr);
    
    l.del("single");
    BOOST_TEST(l.is_empty() == true);
}

BOOST_AUTO_TEST_CASE(TailPointerIntegrity)
{
    List l;
    
    // Проверка что tail всегда корректный
    l.push_back("a");
    BOOST_TEST(l.find("a") != nullptr);
    
    l.push_back("b");
    BOOST_TEST(l.find("b") != nullptr);
    
    l.push_front("c");
    BOOST_TEST(l.find("c") != nullptr);
    
    // После всех операций tail должен указывать на "b"
    BOOST_TEST(l.get_at(2)->data == "b");
    
    l.del_tail();
    // Теперь tail должен указывать на "a"
    BOOST_TEST(l.get_at(1)->data == "a");
}

BOOST_AUTO_TEST_CASE(MultipleInsertDelete)
{
    List l;
    const int NUM_OPERATIONS = 50;
    
    // Множественные добавления
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        l.push_back("elem_" + std::to_string(i));
    }
    
    BOOST_TEST(l.get_size() == NUM_OPERATIONS);
    
    // Проверяем все элементы
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        BOOST_TEST(l.find("elem_" + std::to_string(i)) != nullptr);
    }
    
    // Удаляем каждый второй элемент
    for (int i = 0; i < NUM_OPERATIONS; i += 2) {
        l.del("elem_" + std::to_string(i));
    }
    
    BOOST_TEST(l.get_size() == NUM_OPERATIONS / 2);
    
    // Проверяем оставшиеся элементы
    for (int i = 1; i < NUM_OPERATIONS; i += 2) {
        BOOST_TEST(l.find("elem_" + std::to_string(i)) != nullptr);
    }
    
    // Добавляем новые элементы
    for (int i = NUM_OPERATIONS; i < NUM_OPERATIONS + 10; ++i) {
        l.push_front("new_" + std::to_string(i));
    }
    
    BOOST_TEST(l.get_size() == (NUM_OPERATIONS / 2) + 10);
}

BOOST_AUTO_TEST_CASE(InsertBeforeHeadEdgeCase)
{
    List l;
    l.push_back("original_head");
    
    // Вставка перед головой через insert_before
    l.insert_before("original_head", "new_head");
    BOOST_TEST(l.get_size() == 2);
    BOOST_TEST(l.get_at(0)->data == "new_head");
    BOOST_TEST(l.get_at(1)->data == "original_head");
}

BOOST_AUTO_TEST_CASE(DeleteHeadThroughDelMethod)
{
    List l;
    l.push_back("head");
    l.push_back("tail");
    
    // Удаление головы через метод del
    l.del("head");
    BOOST_TEST(l.get_size() == 1);
    BOOST_TEST(l.find("head") == nullptr);
    BOOST_TEST(l.find("tail") != nullptr);
    BOOST_TEST(l.get_at(0)->data == "tail");
}

BOOST_AUTO_TEST_CASE(EmptyListAfterMultipleDeletions)
{
    List l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");
    
    // Последовательное удаление всех элементов
    l.del("a");
    BOOST_TEST(l.get_size() == 2);
    l.del("b");
    BOOST_TEST(l.get_size() == 1);
    l.del("c");
    BOOST_TEST(l.is_empty() == true);
    BOOST_TEST(l.get_size() == 0);
}

BOOST_AUTO_TEST_SUITE_END()