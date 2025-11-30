#include "gtest/gtest.h"
#include "../sd/tree/tree.hpp"
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

// Вспомогательная функция для чтения вывода обхода
std::vector<int> capturePrint(const AVL& t, void (AVL::*printFunc)() const) {
    std::stringstream ss;
    std::streambuf* old = std::cout.rdbuf(ss.rdbuf()); // перенаправляем cout
    (t.*printFunc)();
    std::cout.rdbuf(old); // восстанавливаем cout

    std::vector<int> result;
    int val;
    while (ss >> val) result.push_back(val);
    return result;
}

// ------------------------ Тесты ------------------------

TEST(AVLTest, InsertAndSearch) {
    AVL t;
    t.insert(10);
    t.insert(20);
    t.insert(5);

    EXPECT_NE(t.search(10), nullptr);
    EXPECT_NE(t.search(20), nullptr);
    EXPECT_NE(t.search(5), nullptr);
    EXPECT_EQ(t.search(999), nullptr);
}

TEST(AVLTest, DeleteLeafAndNodeWithChildren) {
    AVL t;
    t.insert(50);
    t.insert(30);
    t.insert(70);
    t.insert(20);
    t.insert(40);
    t.insert(60);
    t.insert(80);

    // удалить лист
    t.remove(20);
    EXPECT_EQ(t.search(20), nullptr);

    // удалить узел с двумя детьми
    t.remove(30);
    EXPECT_EQ(t.search(30), nullptr);

    // проверить порядок после удалений
    auto in = capturePrint(t, &AVL::print_inorder);
    EXPECT_EQ(in, (std::vector<int>{40,50,60,70,80}));
}

TEST(AVLTest, Traversals) {
    AVL t;
    t.insert(40);
    t.insert(20);
    t.insert(60);
    t.insert(10);
    t.insert(30);

    auto in = capturePrint(t, &AVL::print_inorder);
    auto pre = capturePrint(t, &AVL::print_preorder);
    auto post = capturePrint(t, &AVL::print_postorder);
    auto lvl = capturePrint(t, &AVL::print_lvlorder);

    EXPECT_EQ(in,  (std::vector<int>{10,20,30,40,60}));
    EXPECT_EQ(pre, (std::vector<int>{40,20,10,30,60}));
    EXPECT_EQ(post,(std::vector<int>{10,30,20,60,40}));
    EXPECT_EQ(lvl, (std::vector<int>{40,20,60,10,30}));
}

TEST(AVLTest, AddTreeMerge) {
    AVL a, b;
    a.insert(50); a.insert(20); a.insert(70);
    b.insert(10); b.insert(60); b.insert(80);

    a.addTree(b.getRoot());

    auto in = capturePrint(a, &AVL::print_inorder);
    EXPECT_EQ(in, (std::vector<int>{10,20,50,60,70,80}));
}

TEST(AVLTest, ParentPointers) {
    AVL t;
    t.insert(40);
    t.insert(20);
    t.insert(60);
    t.insert(10);
    t.insert(30);

    auto root = t.getRoot();
    EXPECT_EQ(root->parent, nullptr);
    EXPECT_EQ(root->left->parent, root);
    EXPECT_EQ(root->right->parent, root);
}

TEST(AVLTest, HeightBalanceProperty) {
    AVL t;
    for (int i = 1; i <= 50; ++i) t.insert(i);

    std::function<void(AVLNode*)> dfs = [&](AVLNode* n){
        if (!n) return;
        int balance = n->left ? n->left->height : 0;
        balance -= n->right ? n->right->height : 0;
        EXPECT_GE(balance, -1);
        EXPECT_LE(balance, 1);
        dfs(n->left);
        dfs(n->right);
    };
    dfs(t.getRoot());
}

TEST(AVLTest, StressRandomInsertRemove) {
    AVL t;
    std::vector<int> vals = {10,20,30,40,50,25,35,45,55};
    for (int v : vals) t.insert(v);

    t.remove(20);
    t.remove(40);
    t.remove(55);

    auto in = capturePrint(t, &AVL::print_inorder);
    EXPECT_EQ(in, (std::vector<int>{10,25,30,35,45,50}));
}

// ===== BENCHMARKS =====
TEST(TreeBench, BENCHMARK_Tree_Insert) {
    auto start = std::chrono::high_resolution_clock::now();
    AVL tree;
    for (int i = 0; i < 5000; ++i) tree.insert(i);
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "\ninsert x5000: " << ms << " ms\n";
}

TEST(TreeBench, BENCHMARK_Tree_Search) {
    AVL tree;
    for (int i = 0; i < 1000; ++i) tree.insert(i);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000; ++i) tree.search(i % 1000);
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "\nsearch x100000: " << ms << " ms\n";
}

// ===== BENCHMARKS =====
TEST(AVLBench, BENCHMARK_Insert) {
    auto start = std::chrono::high_resolution_clock::now();
    AVL tree;
    for (int i = 0; i < 5000; ++i) tree.insert(i);
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "\ninsert x5000: " << ms << " ms\n";
}

TEST(AVLBench, BENCHMARK_Search) {
    AVL tree;
    for (int i = 0; i < 1000; ++i) tree.insert(i);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000; ++i) tree.search(i % 1000);
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "\nsearch x100000: " << ms << " ms\n";
}