#include <catch2/catch_all.hpp>
#include <sstream>
#include "../../sd/array/array.hpp"
#include "../../sd/list/list.hpp"
#include "../../sd/stack/stack.hpp"
#include "../../sd/queue/queue.hpp"
#include "../../sd/db_list/db_list.hpp"
#include "../../sd/hash/hash.hpp"
#include "../../sd/tree/tree.hpp"

TEST_CASE("Array Binary Serialization", "[serialization]")
{
    Mass original;
    original.push_back("hello");
    original.push_back("world");
    original.push_back("test");
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    Mass restored;
    restored.deserialize(iss);
    
    REQUIRE(restored.get_size() == 3);
    REQUIRE(restored.get_at(0) == "hello");
    REQUIRE(restored.get_at(1) == "world");
    REQUIRE(restored.get_at(2) == "test");
}

TEST_CASE("Array Binary Serialization Empty", "[serialization]")
{
    Mass original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    Mass restored;
    restored.deserialize(iss);
    
    REQUIRE(restored.get_size() == 0);
    REQUIRE(restored.is_empty() == true);
}

TEST_CASE("HashTable Binary Serialization", "[serialization]")
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
    
    REQUIRE(restored.get_size() == 3);
    REQUIRE(restored.Find("alice") == true);
    REQUIRE(restored.Find("bob") == true);
    REQUIRE(restored.Find("charlie") == true);
}

TEST_CASE("HashTable Binary Serialization Empty", "[serialization]")
{
    HashTable original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    HashTable restored;
    restored.deserialize(iss);
    
    REQUIRE(restored.get_size() == 0);
}

TEST_CASE("AVL Binary Serialization", "[serialization]")
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
    
    REQUIRE(restored.search(50) != nullptr);
    REQUIRE(restored.search(30) != nullptr);
    REQUIRE(restored.search(70) != nullptr);
    REQUIRE(restored.search(20) != nullptr);
    REQUIRE(restored.search(40) != nullptr);
    REQUIRE(restored.search(999) == nullptr);
}

TEST_CASE("AVL Binary Serialization Empty", "[serialization]")
{
    AVL original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    AVL restored;
    restored.deserialize(iss);
    
    REQUIRE(restored.getRoot() == nullptr);
}

TEST_CASE("List Text Serialization", "[serialization]")
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
    
    REQUIRE(restored.get_size() == 3);
    REQUIRE(restored.get_at(0)->data == "first");
    REQUIRE(restored.get_at(1)->data == "second");
    REQUIRE(restored.get_at(2)->data == "third");
}

TEST_CASE("List Text Serialization Empty", "[serialization]")
{
    List original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    List restored;
    restored.deserialize(iss);
    
    REQUIRE(restored.is_empty() == true);
}

TEST_CASE("Stack Text Serialization", "[serialization]")
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
    
    REQUIRE(restored.top() == "gamma");
    restored.pop();
    REQUIRE(restored.top() == "beta");
    restored.pop();
    REQUIRE(restored.top() == "alpha");
}

TEST_CASE("Stack Text Serialization Empty", "[serialization]")
{
    Stack original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    Stack restored;
    restored.deserialize(iss);
    
    REQUIRE(restored.is_empty() == true);
}

TEST_CASE("Queue Text Serialization", "[serialization]")
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
    
    REQUIRE(restored.front() == "one");
    restored.pop();
    REQUIRE(restored.front() == "two");
    restored.pop();
    REQUIRE(restored.front() == "three");
}

TEST_CASE("Queue Text Serialization Empty", "[serialization]")
{
    Queue original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    Queue restored;
    restored.deserialize(iss);
    
    REQUIRE(restored.is_empty() == true);
}

TEST_CASE("DoublyList Text Serialization", "[serialization]")
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
    
    REQUIRE(restored.get_size() == 3);
    REQUIRE(restored.get_at(0)->data == "node1");
    REQUIRE(restored.get_at(1)->data == "node2");
    REQUIRE(restored.get_at(2)->data == "node3");
}

TEST_CASE("DoublyList Text Serialization Empty", "[serialization]")
{
    DoublyList original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    DoublyList restored;
    restored.deserialize(iss);
    
    REQUIRE(restored.is_empty() == true);
}
