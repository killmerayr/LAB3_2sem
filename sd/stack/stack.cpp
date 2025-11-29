#include "stack.hpp"
#include <iostream>
using namespace std;

// Конструктор
Stack::Stack() : top_node(nullptr) {}

// Деструктор
Stack::~Stack() {
    while (!is_empty()) {
        pop();
    }
}

// Проверка на пустоту
bool Stack::is_empty() const {
    return top_node == nullptr;
}

// Поместить элемент на стек
void Stack::push(const string& val) {
    SNode* node = new SNode(val);
    node->next = top_node;
    top_node = node;
}

// Удалить верхний элемент
void Stack::pop() {
    if (is_empty()) {
        cout << "Стек пуст!\n";
        return;
    }
    SNode* tmp = top_node;
    top_node = top_node->next;
    delete tmp;
}

// Получить верхний элемент
string Stack::top() const {
    if (is_empty()) {
        cout << "Стек пуст!\n";
        return "";
    }
    return top_node->data;
}

// Вывести стек
void Stack::print() const {
    SNode* cur = top_node;
    while (cur) {
        cout << cur->data << " ";
        cur = cur->next;
    }
    cout << endl;

}