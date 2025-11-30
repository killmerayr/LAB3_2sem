#include <catch2/catch_all.hpp>
#include "../../sd/tree/tree.hpp"
#include <sstream>
#include <string>
#include <chrono>

TEST_CASE("Конструктор и пустое дерево", "[AVL]") {
    AVL tree;
    REQUIRE(tree.search(1) == nullptr);
    REQUIRE(tree.getRoot() == nullptr);
}

TEST_CASE("Вставка элементов", "[AVL]") {
    AVL tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);

    REQUIRE(tree.search(1) != nullptr);
    REQUIRE(tree.search(2) != nullptr);
    REQUIRE(tree.search(3) != nullptr);
    REQUIRE(tree.search(4) == nullptr);
}

TEST_CASE("Удаление листьев", "[AVL]") {
    AVL tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);

    tree.remove(1);
    REQUIRE(tree.search(1) == nullptr);
    REQUIRE(tree.search(2) != nullptr);
    REQUIRE(tree.search(3) != nullptr);

    tree.remove(3);
    REQUIRE(tree.search(3) == nullptr);
    REQUIRE(tree.search(2) != nullptr);
}

TEST_CASE("Удаление узла с одним потомком", "[AVL]") {
    AVL tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);

    tree.remove(3);
    REQUIRE(tree.search(3) == nullptr);
    REQUIRE(tree.search(4) != nullptr);
}

TEST_CASE("Удаление узла с двумя потомками", "[AVL]") {
    AVL tree;
    tree.insert(2);
    tree.insert(1);
    tree.insert(3);
    tree.insert(4);

    tree.remove(2);
    REQUIRE(tree.search(2) == nullptr);
    REQUIRE(tree.search(1) != nullptr);
    REQUIRE(tree.search(3) != nullptr);
}

TEST_CASE("Повторная вставка одного и того же элемента", "[AVL]") {
    AVL tree;
    tree.insert(1);
    tree.insert(1);
    tree.insert(1);

    // если insert игнорирует дубликаты, то 1 должна быть только одна
    REQUIRE(tree.search(1) != nullptr);
    // проверяем что дубликаты не ломают дерево
    REQUIRE_NOTHROW(tree.print_inorder());
}

TEST_CASE("search на пустом дереве", "[AVL]") {
    AVL tree;
    REQUIRE(tree.search(10) == nullptr);
}

TEST_CASE("Обходы дерева (in/pre/post/lvl)", "[AVL]") {
    AVL tree;
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);

    REQUIRE_NOTHROW(tree.print_inorder());
    REQUIRE_NOTHROW(tree.print_preorder());
    REQUIRE_NOTHROW(tree.print_postorder());
    REQUIRE_NOTHROW(tree.print_lvlorder());
}

TEST_CASE("addTree и getRoot", "[AVL]") {
    AVL tree1;
    AVL tree2;

    tree1.insert(1);
    tree1.insert(3);
    tree1.insert(5);

    tree2.insert(2);
    tree2.insert(4);

    REQUIRE_NOTHROW(tree1.addTree(tree2.getRoot()));
    REQUIRE(tree1.getRoot() != nullptr);
}

TEST_CASE("LL rotation", "[AVL]") {
    AVL t;
    t.insert(3);
    t.insert(2);
    t.insert(1);
    REQUIRE(t.search(2) != nullptr); // после поворота 2 — корень
}

TEST_CASE("RR rotation", "[AVL]") {
    AVL t;
    t.insert(1);
    t.insert(2);
    t.insert(3);
    REQUIRE(t.search(2) != nullptr);
}

TEST_CASE("LR rotation", "[AVL]") {
    AVL t;
    t.insert(3);
    t.insert(1);
    t.insert(2);
    REQUIRE(t.search(2) != nullptr);
}

TEST_CASE("RL rotation", "[AVL]") {
    AVL t;
    t.insert(1);
    t.insert(3);
    t.insert(2);
    REQUIRE(t.search(2) != nullptr);
}

TEST_CASE("Удаление корня и нескольких последовательных удалений", "[AVL]") {
    AVL t;
    t.insert(2);
    t.insert(1);
    t.insert(3);
    t.remove(2);  // удаляем корень
    REQUIRE(t.search(2) == nullptr);
    t.remove(1);
    t.remove(3);
    REQUIRE(t.getRoot() == nullptr);
}

TEST_CASE("Удаление отсутствующего элемента", "[AVL]") {
    AVL t;
    t.insert(1);
    REQUIRE_NOTHROW(t.remove(777));
}

TEST_CASE("findMin возвращает минимальный элемент в поддереве", "[AVL]") {
    AVL t;
    t.insert(10);
    t.insert(5);
    t.insert(2);
    t.insert(8);
    t.insert(7);

    AVLNode* minNode = t.search(5);   // корень поддерева
    REQUIRE(minNode != nullptr);

    minNode = t.search(5);
    REQUIRE(t.getRoot() != nullptr);
    // прямой вызов findMin невозможен, но косвенно проверяется через удаление с двумя детьми
    t.remove(5);
    REQUIRE(t.search(5) == nullptr);
}

TEST_CASE("Удаление с преемником, у которого есть правый ребёнок", "[AVL]") {
    AVL t;
    t.insert(10);
    t.insert(5);
    t.insert(15);
    t.insert(13);
    t.insert(17);
    t.insert(16); // правый ребенок преемника (succ = 15)

    t.remove(10); // 10 имеет правое поддерево, там succ = 13
    REQUIRE(t.search(10) == nullptr);
    REQUIRE(t.search(13) != nullptr);
}

TEST_CASE("Удаление вызывает балансировку вверх по дереву", "[AVL]") {
    AVL t;
    t.insert(30);
    t.insert(20);
    t.insert(40);
    t.insert(10);
    t.insert(25);
    t.insert(5);  // левый перекос

    t.remove(40); // при удалении балансировка переместится вверх
    REQUIRE(t.search(40) == nullptr);
    REQUIRE(t.getRoot()->val == 20); // после ротации 20 — корень
}

TEST_CASE("addTree добавляет большое поддерево корректно", "[AVL]") {
    AVL t1, t2;
    for (int v : {8, 4, 12, 2, 6, 10, 14})
        t1.insert(v);
    for (int v : {7, 3, 9, 1})
        t2.insert(v);

    REQUIRE_NOTHROW(t1.addTree(t2.getRoot()));

    for (int v : {1,2,3,4,6,7,8,9,10,12,14})
        REQUIRE(t1.search(v) != nullptr);
}

TEST_CASE("lvlorder, preorder, inorder и postorder покрывают дерево полностью", "[AVL]") {
    AVL t;
    for (int v : {5, 3, 7, 2, 4, 6, 8}) t.insert(v);

    REQUIRE_NOTHROW(t.print_inorder());
    REQUIRE_NOTHROW(t.print_preorder());
    REQUIRE_NOTHROW(t.print_postorder());
    REQUIRE_NOTHROW(t.print_lvlorder());

    // Перенаправление вывода чтобы подсчитать элементы, которые прошёл lvlorder
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    t.print_lvlorder();

    std::cout.rdbuf(old); // восстановить cout

    std::string out = buffer.str();
    int count = 0;
    for (char c : out)
        if (std::isdigit(c)) count++;

    REQUIRE(count == 7);
}

TEST_CASE("Внутренняя логика transplant работает (косвенно через remove)", "[AVL]") {
    AVL t;
    t.insert(3);
    t.insert(2);
    t.insert(4);
    t.insert(5);

    // Здесь удаление 4 приводит к transplant(4,5)
    t.remove(4);
    REQUIRE(t.search(4) == nullptr);
    REQUIRE(t.search(5) != nullptr);
    REQUIRE(t.search(3) != nullptr);
}

TEST_CASE("Деструктор корректно очищает память", "[AVL]") {
    AVL* t = new AVL();
    for (int v : {5, 3, 7, 1, 4, 6, 8}) t->insert(v);
    REQUIRE_NOTHROW(delete t);   // проверка освобождения
}

TEST_CASE("AVL — comprehensive stress test", "[AVL][stress]") {
    AVL t;
    
    // --- много вставок ---
    for (int i = 0; i < 100; ++i) {
        t.insert(i);
    }
    REQUIRE(t.getRoot() != nullptr);
    
    // --- поиск всех элементов ---
    for (int i = 0; i < 100; ++i) {
        REQUIRE(t.search(i) != nullptr);
    }
    
    // --- удаление половины ---
    for (int i = 0; i < 50; ++i) {
        t.remove(i);
    }
    
    // --- проверка оставшихся ---
    for (int i = 50; i < 100; ++i) {
        REQUIRE(t.search(i) != nullptr);
    }
    
    // --- проверка удалённых ---
    for (int i = 0; i < 50; ++i) {
        REQUIRE(t.search(i) == nullptr);
    }
}

TEST_CASE("AVL — multiple rotations and removals", "[AVL][extra]") {
    AVL t;
    
    // --- вставляем последовательность, требующую многих ротаций ---
    for (int i : {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 65, 75, 85}) {
        t.insert(i);
    }
    
    REQUIRE(t.search(50) != nullptr);
    
    // --- удаляем разные элементы ---
    t.remove(20);
    t.remove(70);
    t.remove(50);
    
    // --- проверяем остальные ---
    for (int i : {30, 40, 60, 80, 10, 25, 35, 65, 75, 85}) {
        REQUIRE(t.search(i) != nullptr);
    }
}

TEST_CASE("AVL — edge case with single rotation", "[AVL][extra2]") {
    AVL t;
    
    // --- LL ротация ---
    t.insert(3);
    t.insert(2);
    t.insert(1);
    
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    t.print_inorder();
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    size_t pos1 = output.find("1");
    size_t pos2 = output.find("2");
    size_t pos3 = output.find("3");
    REQUIRE(pos1 < pos2);
    REQUIRE(pos2 < pos3);
}

TEST_CASE("AVL — double rotations", "[AVL][extra3]") {
    AVL t;
    
    // --- LR ротация ---
    t.insert(10);
    t.insert(5);
    t.insert(7);
    
    REQUIRE(t.search(7) != nullptr);
    REQUIRE(t.search(5) != nullptr);
    REQUIRE(t.search(10) != nullptr);
    
    // --- RL ротация ---
    AVL t2;
    t2.insert(5);
    t2.insert(10);
    t2.insert(7);
    
    REQUIRE(t2.search(7) != nullptr);
    REQUIRE(t2.search(10) != nullptr);
    REQUIRE(t2.search(5) != nullptr);
}

TEST_CASE("AVL — all traversals with complex tree", "[AVL][extra4]") {
    AVL t;
    for (int v : {50, 25, 75, 12, 37, 62, 87, 6, 18, 31, 43, 56, 68, 81, 93}) {
        t.insert(v);
    }
    
    // --- проверяем что все обходы работают без ошибок ---
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    t.print_inorder();
    t.print_preorder();
    t.print_postorder();
    t.print_lvlorder();
    
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    // проверяем что все элементы выведены
    for (int v : {50, 25, 75, 12, 37}) {
        REQUIRE(output.find(std::to_string(v)) != std::string::npos);
    }
}

// ===== BENCHMARKS =====
TEST_CASE("BENCHMARK_Tree_Insert", "[benchmark]") {
    auto start = std::chrono::high_resolution_clock::now();
    AVL tree;
    for (int i = 0; i < 5000; ++i) tree.insert(i);
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    INFO("insert x5000: " << ms << " ms");
}

TEST_CASE("BENCHMARK_Tree_Search", "[benchmark]") {
    AVL tree;
    for (int i = 0; i < 1000; ++i) tree.insert(i);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000; ++i) tree.search(i % 1000);
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    INFO("search x100000: " << ms << " ms");
}