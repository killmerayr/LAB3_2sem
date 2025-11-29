#include <boost/test/unit_test.hpp>
#include "../../sd/db_list/db_list.hpp"

BOOST_AUTO_TEST_SUITE(DoubleListSuite)

BOOST_AUTO_TEST_CASE(PushAndIterate)
{
    DoublyList l;
    l.push_back("a");
    l.push_back("b");
    BOOST_TEST(l.get_at(0));
    BOOST_TEST(l.get_at(1));
}

BOOST_AUTO_TEST_CASE(DeleteAndCheck)
{
    DoublyList l;
    l.push_back("x");
    l.del("x");
    l.push_front("y");
    l.push_front("z");
    BOOST_TEST(l.get_at(0)->data == "z");
    BOOST_TEST(l.get_size() == 2);
}

BOOST_AUTO_TEST_CASE(FindTest)
{
    DoublyList l;
    l.push_back("a");
    l.push_back("b");
    BOOST_TEST(l.find("a") != nullptr);
    BOOST_TEST(l.find("c") == nullptr);
}

BOOST_AUTO_TEST_CASE(PushFrontBackTest)
{
    DoublyList l;
    l.push_front("1");
    l.push_back("2");
    l.push_front("0");
    BOOST_TEST(l.get_at(0)->data == "0");
    BOOST_TEST(l.get_at(2)->data == "2");
}

BOOST_AUTO_TEST_CASE(InsertAfterBeforeTest)
{
    DoublyList l;
    l.push_back("a");
    l.push_back("b");
    l.insert_after("a", "x");
    BOOST_TEST(l.get_at(1)->data == "x");

    l.insert_before("b", "y");
    BOOST_TEST(l.get_at(2)->data == "y");

    // попытка вставки по несуществующему ключу
    l.insert_after("zzz", "fail");
    l.insert_before("zzz", "fail");
    BOOST_TEST(l.get_size() == 4);
}

BOOST_AUTO_TEST_CASE(DeleteVariantsTest)
{
    DoublyList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");

    l.del("b");
    BOOST_TEST(l.get_size() == 2);

    // попытка удалить несуществующий
    l.del("nope");
    BOOST_TEST(l.get_size() == 2);
}

BOOST_AUTO_TEST_CASE(DeleteHeadTailTest)
{
    DoublyList l;
    l.push_back("x");
    l.del_head();
    BOOST_TEST(l.is_empty());

    l.push_back("1");
    l.push_back("2");
    l.del_tail();
    BOOST_TEST(l.get_size() == 1);
    BOOST_TEST(l.get_at(0)->data == "1");
}

BOOST_AUTO_TEST_CASE(DeleteBeforeAfterTest)
{
    DoublyList l;
    l.push_back("a");
    l.push_back("b");
    l.push_back("c");

    l.del_after("a"); // удаляется "b"
    BOOST_TEST(l.get_at(1)->data == "c");

    l.del_before("c"); // удаляется "a"
    BOOST_TEST(l.get_at(0)->data == "c");

    // не должно упасть
    l.del_after("c");
    l.del_before("c");
}

BOOST_AUTO_TEST_CASE(GetAtTest)
{
    DoublyList l;
    l.push_back("a");
    l.push_back("b");
    BOOST_TEST(l.get_at(0)->data == "a");
    BOOST_TEST(l.get_at(1)->data == "b");
    BOOST_TEST(l.get_at(-1) == nullptr);
    BOOST_TEST(l.get_at(10) == nullptr);
}

BOOST_AUTO_TEST_CASE(PrintForwardBackward)
{
    DoublyList l;
    l.push_back("a");
    l.push_back("b");

    std::stringstream buf;
    std::streambuf* old = std::cout.rdbuf(buf.rdbuf());
    l.print_forward();
    l.print_backward();
    std::cout.rdbuf(old);

    BOOST_TEST(!buf.str().empty());
}

BOOST_AUTO_TEST_SUITE_END()