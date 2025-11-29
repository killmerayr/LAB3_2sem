#pragma once
#include <string>

class Stack {
private:
    struct SNode {
        std::string data;
        SNode* next;
        SNode(const std::string& val) : data(val), next(nullptr) {}
    };

    SNode* top_node; // указатель на вершину стека

public:
    Stack();
    ~Stack();

    bool is_empty() const;           // проверить пустоту
    void push(const std::string& val);  // положить элемент на стек
    void pop();                          // удалить верхний элемент
    std::string top() const;             // получить верхний элемент
    void print() const;                  // вывести стек
};