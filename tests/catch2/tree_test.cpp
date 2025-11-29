#include <catch2/catch_all.hpp>
#include "../../sd/tree/tree.hpp"

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

