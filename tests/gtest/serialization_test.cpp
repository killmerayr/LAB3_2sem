#include <gtest/gtest.h>
#include <sstream>
#include "../../sd/array/array.hpp"
#include "../../sd/list/list.hpp"
#include "../../sd/stack/stack.hpp"
#include "../../sd/queue/queue.hpp"
#include "../../sd/db_list/db_list.hpp"
#include "../../sd/hash/hash.hpp"
#include "../../sd/tree/tree.hpp"

// ========== BINARY SERIALIZATION TESTS ==========

class ArraySerializationTest : public ::testing::Test {};

TEST_F(ArraySerializationTest, BinarySerializationWithData)
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
    
    EXPECT_EQ(restored.get_size(), 3);
    EXPECT_EQ(restored.get_at(0), "hello");
    EXPECT_EQ(restored.get_at(1), "world");
    EXPECT_EQ(restored.get_at(2), "test");
}

TEST_F(ArraySerializationTest, BinarySerializationEmpty)
{
    Mass original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    Mass restored;
    restored.deserialize(iss);
    
    EXPECT_EQ(restored.get_size(), 0);
    EXPECT_TRUE(restored.is_empty());
}

class HashTableSerializationTest : public ::testing::Test {};

TEST_F(HashTableSerializationTest, BinarySerializationWithData)
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
    
    EXPECT_EQ(restored.get_size(), 3);
    EXPECT_TRUE(restored.Find("alice"));
    EXPECT_TRUE(restored.Find("bob"));
    EXPECT_TRUE(restored.Find("charlie"));
}

TEST_F(HashTableSerializationTest, BinarySerializationEmpty)
{
    HashTable original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    HashTable restored;
    restored.deserialize(iss);
    
    EXPECT_EQ(restored.get_size(), 0);
}

class AVLSerializationTest : public ::testing::Test {};

TEST_F(AVLSerializationTest, BinarySerializationWithData)
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
    
    EXPECT_NE(restored.search(50), nullptr);
    EXPECT_NE(restored.search(30), nullptr);
    EXPECT_NE(restored.search(70), nullptr);
    EXPECT_NE(restored.search(20), nullptr);
    EXPECT_NE(restored.search(40), nullptr);
    EXPECT_EQ(restored.search(999), nullptr);
}

TEST_F(AVLSerializationTest, BinarySerializationEmpty)
{
    AVL original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    AVL restored;
    restored.deserialize(iss);
    
    EXPECT_EQ(restored.getRoot(), nullptr);
}

// ========== TEXT SERIALIZATION TESTS ==========

class ListSerializationTest : public ::testing::Test {};

TEST_F(ListSerializationTest, TextSerializationWithData)
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
    
    EXPECT_EQ(restored.get_size(), 3);
    EXPECT_EQ(restored.get_at(0)->data, "first");
    EXPECT_EQ(restored.get_at(1)->data, "second");
    EXPECT_EQ(restored.get_at(2)->data, "third");
}

TEST_F(ListSerializationTest, TextSerializationEmpty)
{
    List original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    List restored;
    restored.deserialize(iss);
    
    EXPECT_TRUE(restored.is_empty());
}

class StackSerializationTest : public ::testing::Test {};

TEST_F(StackSerializationTest, TextSerializationWithData)
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
    
    EXPECT_EQ(restored.top(), "gamma");
    restored.pop();
    EXPECT_EQ(restored.top(), "beta");
    restored.pop();
    EXPECT_EQ(restored.top(), "alpha");
}

TEST_F(StackSerializationTest, TextSerializationEmpty)
{
    Stack original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    Stack restored;
    restored.deserialize(iss);
    
    EXPECT_TRUE(restored.is_empty());
}

class QueueSerializationTest : public ::testing::Test {};

TEST_F(QueueSerializationTest, TextSerializationWithData)
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
    
    EXPECT_EQ(restored.front(), "one");
    restored.pop();
    EXPECT_EQ(restored.front(), "two");
    restored.pop();
    EXPECT_EQ(restored.front(), "three");
}

TEST_F(QueueSerializationTest, TextSerializationEmpty)
{
    Queue original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    Queue restored;
    restored.deserialize(iss);
    
    EXPECT_TRUE(restored.is_empty());
}

class DoublyListSerializationTest : public ::testing::Test {};

TEST_F(DoublyListSerializationTest, TextSerializationWithData)
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
    
    EXPECT_EQ(restored.get_size(), 3);
    EXPECT_EQ(restored.get_at(0)->data, "node1");
    EXPECT_EQ(restored.get_at(1)->data, "node2");
    EXPECT_EQ(restored.get_at(2)->data, "node3");
}

TEST_F(DoublyListSerializationTest, TextSerializationEmpty)
{
    DoublyList original;
    
    std::ostringstream oss;
    original.serialize(oss);
    
    std::istringstream iss(oss.str());
    DoublyList restored;
    restored.deserialize(iss);
    
    EXPECT_TRUE(restored.is_empty());
}
