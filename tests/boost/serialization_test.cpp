#include <boost/test/unit_test.hpp>
#include <sstream>
#include <iostream>
#include "../../sd/array/array.hpp"
#include "../../sd/list/list.hpp"
#include "../../sd/stack/stack.hpp"
#include "../../sd/queue/queue.hpp"
#include "../../sd/db_list/db_list.hpp"
#include "../../sd/hash/hash.hpp"
#include "../../sd/tree/tree.hpp"

BOOST_AUTO_TEST_SUITE(SerializationSuite)

// ========== БИНАРНЫЕ ТЕСТЫ ==========

// Array (Mass) - бинарная сериализация
BOOST_AUTO_TEST_CASE(ArrayBinarySerialization)
{
    Mass original;
    original.push_back("hello");
    original.push_back("world");
    original.push_back("test");
    
    // Сериализуем
    std::ostringstream oss;
    original.serialize(oss);
    
    // Десериализуем
    std::istringstream iss(oss.str());
    Mass restored;
    restored.deserialize(iss);
    
    // Проверяем
    BOOST_TEST(restored.get_size() == 3);
    BOOST_TEST(restored.get_at(0) == "hello");
    BOOST_TEST(restored.get_at(1) == "world");
    BOOST_TEST(restored.get_at(2) == "test");
}

BOOST_AUTO_TEST_CASE(ArrayBinarySerializationEmpty)
{
    Mass original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    Mass restored;
    restored.deserialize(iss);
    
    BOOST_TEST(restored.get_size() == 0);
    BOOST_TEST(restored.is_empty() == true);
}

// HashTable - бинарная сериализация
BOOST_AUTO_TEST_CASE(HashTableBinarySerialization)
{
    HashTable original;
    original.Add("alice");
    original.Add("bob");
    original.Add("charlie");
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    HashTable restored;
    restored.deserialize(iss);
    
    BOOST_TEST(restored.get_size() == 3);
    BOOST_TEST(restored.Find("alice") == true);
    BOOST_TEST(restored.Find("bob") == true);
    BOOST_TEST(restored.Find("charlie") == true);
}

BOOST_AUTO_TEST_CASE(HashTableBinarySerializationEmpty)
{
    HashTable original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    HashTable restored;
    restored.deserialize(iss);
    
    BOOST_TEST(restored.get_size() == 0);
}

// AVL Tree - бинарная сериализация
BOOST_AUTO_TEST_CASE(AVLBinarySerialization)
{
    AVL original;
    original.insert(50);
    original.insert(30);
    original.insert(70);
    original.insert(20);
    original.insert(40);
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    AVL restored;
    restored.deserialize(iss);
    
    // Проверяем наличие элементов
    BOOST_TEST(restored.search(50) != nullptr);
    BOOST_TEST(restored.search(30) != nullptr);
    BOOST_TEST(restored.search(70) != nullptr);
    BOOST_TEST(restored.search(20) != nullptr);
    BOOST_TEST(restored.search(40) != nullptr);
    BOOST_TEST(restored.search(999) == nullptr);
}

BOOST_AUTO_TEST_CASE(AVLBinarySerializationEmpty)
{
    AVL original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    AVL restored;
    restored.deserialize(iss);
    
    BOOST_TEST(restored.getRoot() == nullptr);
}

// ========== ТЕКСТОВЫЕ ТЕСТЫ ==========

// List - текстовая сериализация
BOOST_AUTO_TEST_CASE(ListTextSerialization)
{
    List original;
    original.push_back("first");
    original.push_back("second");
    original.push_back("third");
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    List restored;
    restored.deserialize(iss);
    
    BOOST_TEST(restored.get_size() == 3);
    BOOST_TEST(restored.get_at(0)->data == "first");
    BOOST_TEST(restored.get_at(1)->data == "second");
    BOOST_TEST(restored.get_at(2)->data == "third");
}

BOOST_AUTO_TEST_CASE(ListTextSerializationEmpty)
{
    List original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    List restored;
    restored.deserialize(iss);
    
    BOOST_TEST(restored.is_empty() == true);
}

// Stack - текстовая сериализация
BOOST_AUTO_TEST_CASE(StackTextSerialization)
{
    Stack original;
    original.push("alpha");
    original.push("beta");
    original.push("gamma");
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    Stack restored;
    restored.deserialize(iss);
    
    BOOST_TEST(restored.top() == "gamma");
    restored.pop();
    BOOST_TEST(restored.top() == "beta");
    restored.pop();
    BOOST_TEST(restored.top() == "alpha");
}

BOOST_AUTO_TEST_CASE(StackTextSerializationEmpty)
{
    Stack original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    Stack restored;
    restored.deserialize(iss);
    
    BOOST_TEST(restored.is_empty() == true);
}

// Queue - текстовая сериализация
BOOST_AUTO_TEST_CASE(QueueTextSerialization)
{
    Queue original;
    original.push("one");
    original.push("two");
    original.push("three");
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    Queue restored;
    restored.deserialize(iss);
    
    BOOST_TEST(restored.front() == "one");
    restored.pop();
    BOOST_TEST(restored.front() == "two");
    restored.pop();
    BOOST_TEST(restored.front() == "three");
}

BOOST_AUTO_TEST_CASE(QueueTextSerializationEmpty)
{
    Queue original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    Queue restored;
    restored.deserialize(iss);
    
    BOOST_TEST(restored.is_empty() == true);
}

// DoublyList - текстовая сериализация
BOOST_AUTO_TEST_CASE(DoublyListTextSerialization)
{
    DoublyList original;
    original.push_back("node1");
    original.push_back("node2");
    original.push_back("node3");
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    DoublyList restored;
    restored.deserialize(iss);
    
    BOOST_TEST(restored.get_size() == 3);
    BOOST_TEST(restored.get_at(0)->data == "node1");
    BOOST_TEST(restored.get_at(1)->data == "node2");
    BOOST_TEST(restored.get_at(2)->data == "node3");
}

BOOST_AUTO_TEST_CASE(DoublyListTextSerializationEmpty)
{
    DoublyList original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    DoublyList restored;
    restored.deserialize(iss);
    
    BOOST_TEST(restored.is_empty() == true);
}

BOOST_AUTO_TEST_SUITE_END()
