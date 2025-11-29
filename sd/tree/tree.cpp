#include "tree.hpp"

// ----------------- Деструктор -----------------
AVL::~AVL() {
    function<void(AVLNode*)> del = [&](AVLNode* n) {
        if (!n) return;
        del(n->left);
        del(n->right);
        delete n;
    };
    del(root);
}

// ----------------- Повороты -----------------
AVLNode* AVL::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* t2 = y->left;

    y->left = x;
    x->right = t2;

    if (t2) t2->parent = x;
    y->parent = x->parent;

    if (!x->parent) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    x->parent = y;

    updateHeight(x);
    updateHeight(y);
    return y;
}

AVLNode* AVL::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* t2 = x->right;

    x->right = y;
    y->left = t2;

    if (t2) t2->parent = y;
    x->parent = y->parent;

    if (!y->parent) root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;
    y->parent = x;

    updateHeight(y);
    updateHeight(x);
    return x;
}

// ----------------- Вставка -----------------
AVLNode* AVL::insert(AVLNode* node, int val, AVLNode* parent) {
    if (!node) return new AVLNode(val, parent);

    if (val < node->val)
        node->left = insert(node->left, val, node);
    else
        node->right = insert(node->right, val, node);

    updateHeight(node);
    int balance = getBalance(node);

    // LL
    if (balance > 1 && val < node->left->val)
        return rightRotate(node);
    // RR
    if (balance < -1 && val > node->right->val)
        return leftRotate(node);
    // LR
    if (balance > 1 && val > node->left->val) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // RL
    if (balance < -1 && val < node->right->val) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

// ----------------- Удаление -----------------
AVLNode* AVL::search(int val) const {
    AVLNode* n = root;
    while (n) {
        if (val == n->val) return n;
        n = val < n->val ? n->left : n->right;
    }
    return nullptr;
}

void AVL::transplant(AVLNode* u, AVLNode* v) {
    if (!u->parent) root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    if (v) v->parent = u->parent;
}

AVLNode* AVL::findMin(AVLNode* n) const {
    while (n->left) n = n->left;
    return n;
}

AVLNode* AVL::remove(AVLNode* node, int val) {
    node = search(val);
    if (!node) return root;

    if (!node->left && !node->right) {
        transplant(node, nullptr);
        delete node;
    } else if (node->left && !node->right) {
        transplant(node, node->left);
        delete node;
    } else if (!node->left && node->right) {
        transplant(node, node->right);
        delete node;
    } else {
        AVLNode* succ = findMin(node->right);
        node->val = succ->val;
        if (succ->right)
            transplant(succ, succ->right);
        else
            transplant(succ, nullptr);
        delete succ;
    }

    // Балансировка вверх по дереву
    AVLNode* cur = node ? node->parent : nullptr;
    while (cur) {
        updateHeight(cur);
        int balance = getBalance(cur);
        if (balance > 1) {
            if (getBalance(cur->left) >= 0)
                cur = rightRotate(cur);
            else {
                cur->left = leftRotate(cur->left);
                cur = rightRotate(cur);
            }
        } else if (balance < -1) {
            if (getBalance(cur->right) <= 0)
                cur = leftRotate(cur);
            else {
                cur->right = rightRotate(cur->right);
                cur = leftRotate(cur);
            }
        }
        if (!cur->parent) root = cur;
        cur = cur->parent;
    }
    return root;
}

// ----------------- Обходы -----------------
void AVL::inorder(AVLNode* node, void (*visit)(int)) const {
    if (!node) return;
    inorder(node->left, visit);
    visit(node->val);
    inorder(node->right, visit);
}

void AVL::preorder(AVLNode* node, void (*visit)(int)) const {
    if (!node) return;
    visit(node->val);
    preorder(node->left, visit);
    preorder(node->right, visit);
}

void AVL::postorder(AVLNode* node, void (*visit)(int)) const {
    if (!node) return;
    postorder(node->left, visit);
    postorder(node->right, visit);
    visit(node->val);
}

void AVL::lvlorder(AVLNode* node, void (*visit)(int)) const {
    if (!node) return;
    queue<AVLNode*> q;
    q.push(node);
    while (!q.empty()) {
        AVLNode* cur = q.front(); q.pop();
        visit(cur->val);
        if (cur->left) q.push(cur->left);
        if (cur->right) q.push(cur->right);
    }
}

// ----------------- Вспомогательные -----------------
void AVL::addTree(AVLNode* other_root) {
    if (!other_root) return;
    addTree(other_root->left);
    insert(other_root->val);
    addTree(other_root->right);
}

// ----------------- Вывод -----------------
void AVL::print_inorder() const { inorder(root, [](int v){ cout << v << " "; }); cout << endl; }
void AVL::print_preorder() const { preorder(root, [](int v){ cout << v << " "; }); cout << endl; }
void AVL::print_postorder() const { postorder(root, [](int v){ cout << v << " "; }); cout << endl; }
void AVL::print_lvlorder() const { lvlorder(root, [](int v){ cout << v << " "; }); cout << endl; }