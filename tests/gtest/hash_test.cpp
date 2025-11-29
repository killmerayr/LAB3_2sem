#include "gtest/gtest.h"
#include "../sd/hash/hash.hpp"
#include <string>
#include <vector>

TEST(HashTableTest, BasicAddAndFind) {
    HashTable ht;
    EXPECT_TRUE(ht.Add("10"));
    EXPECT_TRUE(ht.Add("20"));
    EXPECT_TRUE(ht.Find("10"));
    EXPECT_TRUE(ht.Find("20"));
    EXPECT_FALSE(ht.Find("30"));
}

TEST(HashTableTest, RemoveTest) {
    HashTable ht;
    ht.Add("1");
    ht.Add("2");
    EXPECT_TRUE(ht.Remove("1"));
    EXPECT_FALSE(ht.Find("1"));
    EXPECT_TRUE(ht.Find("2"));
    EXPECT_FALSE(ht.Remove("10"));
}

TEST(HashTableTest, DeletedSlotsReused) {
    HashTable ht;
    ht.Add("1");
    ht.Remove("1");
    EXPECT_TRUE(ht.Add("1"));  // должен использовать удалённый слот
    EXPECT_TRUE(ht.Find("1"));
}

TEST(HashTableTest, RehashAndResize) {
    HashTable ht;
    int n = 200;
    for (int i = 0; i < n; ++i) {
        ht.Add(std::to_string(i));
    }
    for (int i = 0; i < n; ++i) {
        EXPECT_TRUE(ht.Find(std::to_string(i)));
    }
}

TEST(HashTableTest, MultipleStrings) {
    HashTable ht;
    std::vector<std::string> words = {"apple", "banana", "orange", "kiwi", "grape", "melon", "mango"};
    for (auto &w : words) ht.Add(w);
    for (auto &w : words) EXPECT_TRUE(ht.Find(w));
}

TEST(HashTableTest, AddDuplicateTest) {
    HashTable ht;
    EXPECT_TRUE(ht.Add("x"));
    EXPECT_FALSE(ht.Add("x")); // duplicate should not be added
    EXPECT_TRUE(ht.Find("x"));
}

TEST(HashTableTest, ContinuousAddRemoveReuse) {
    HashTable ht;
    ht.Add("a");
    ht.Add("b");
    ht.Remove("a");
    ht.Add("c"); // should reuse deleted slot
    EXPECT_TRUE(ht.Find("b"));
    EXPECT_TRUE(ht.Find("c"));
    EXPECT_FALSE(ht.Find("a"));
}

TEST(HashTableTest, RemoveNonExistent) {
    HashTable ht;
    ht.Add("z");
    EXPECT_FALSE(ht.Remove("not_here"));
}

TEST(HashTableTest, FindOnDeletedSlot) {
    HashTable ht;
    ht.Add("hello");
    ht.Remove("hello");
    EXPECT_FALSE(ht.Find("hello"));  // must not find deleted
}

TEST(HashTableTest, CollisionHandlingLinearProbing) {
    HashTable ht(4); // small table to force collisions
    // разные строки, но с одинаковыми модулями hash % capacity (подбираем)
    ht.Add("a");
    ht.Add("e"); // djb2(a)=..., djb2(e)=... дадут коллизию в mod 4
    EXPECT_TRUE(ht.Find("a"));
    EXPECT_TRUE(ht.Find("e"));
}

TEST(HashTableTest, ResizeIncreasesCapacityProperly) {
    HashTable ht(4);
    int prev_capacity = ht.get_capacity();
    for (int i = 0; i < 10; ++i)
        ht.Add(std::to_string(i));
    EXPECT_GT(ht.get_capacity(), prev_capacity);
    for (int i = 0; i < 10; ++i)
        EXPECT_TRUE(ht.Find(std::to_string(i)));
}

TEST(HashTableTest, StressAddRemoveRandomLike) {
    HashTable ht;
    for (int i = 0; i < 100; i++)
        ht.Add(std::to_string(i));
    for (int i = 0; i < 50; i++)
        ht.Remove(std::to_string(i));
    for (int i = 50; i < 100; i++)
        EXPECT_TRUE(ht.Find(std::to_string(i)));
    for (int i = 0; i < 50; i++)
        EXPECT_FALSE(ht.Find(std::to_string(i)));
}

TEST(HashTableTest, RepeatedResizingAndReuse) {
    HashTable ht;
    for (int cycle = 0; cycle < 3; ++cycle) {
        for (int i = 0; i < 200; ++i)
            ht.Add("A" + std::to_string(cycle) + "_" + std::to_string(i));
        for (int i = 0; i < 200; ++i)
            ht.Remove("A" + std::to_string(cycle) + "_" + std::to_string(i));
    }
    EXPECT_EQ(ht.get_size(), 0);
}