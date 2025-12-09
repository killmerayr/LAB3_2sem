#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <istream>
#include <ostream>
#include <queue>
using namespace std;

class AVLNode {
public:
  int val;
  AVLNode *left;
  AVLNode *right;
  AVLNode *parent;
  int height;

  AVLNode(int _val, AVLNode *p = nullptr, int _height = 1)
      : val(_val), left(nullptr), right(nullptr), parent(p), height(_height) {}
};

class AVL {
private:
  AVLNode *root;

  int getHeight(AVLNode *n) const { return n ? n->height : 0; }
  void updateHeight(AVLNode *n) {
    if (n)
      n->height = max(getHeight(n->left), getHeight(n->right)) + 1;
  }
  int getBalance(AVLNode *n) const {
    return n ? getHeight(n->left) - getHeight(n->right) : 0;
  }

  AVLNode *leftRotate(AVLNode *x);
  AVLNode *rightRotate(AVLNode *y);

  void transplant(AVLNode *u, AVLNode *v);
  AVLNode *findMin(AVLNode *n) const;

  AVLNode *insert(AVLNode *node, int val, AVLNode *parent);
  AVLNode *remove(AVLNode *node, int val);

  void inorder(AVLNode *node, void (*visit)(int)) const;
  void preorder(AVLNode *node, void (*visit)(int)) const;
  void postorder(AVLNode *node, void (*visit)(int)) const;
  void lvlorder(AVLNode *node, void (*visit)(int)) const;

public:
  AVL() : root(nullptr) {}
  ~AVL();

  void insert(int val) { root = insert(root, val, nullptr); }
  void remove(int val) { root = remove(root, val); }
  AVLNode *search(int val) const;
  void addTree(AVLNode *other_root);

  void print_inorder() const;
  void print_preorder() const;
  void print_postorder() const;
  void print_lvlorder() const;

  AVLNode *getRoot() const { return root; }

  // Бинарная сериализация и десериализация
  void serialize(std::ostream &out) const;
  void deserialize(std::istream &in);
};