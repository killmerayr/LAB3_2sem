#pragma once
#include <string>
#include <ostream>
#include <istream>

class Queue {
private:
    struct QNode {
        std::string val;
        QNode* next;
        QNode(const std::string& v) : val(v), next(nullptr) {}
    };

    QNode* first;
    QNode* last;

public:
    Queue();                // конструктор
    ~Queue();               // деструктор

    bool is_empty() const;          // проверить пустоту
    void push(const std::string& value); // добавить в конец
    void pop();                      // удалить из начала
    std::string front() const;       // получить первый элемент
    void print() const;              // вывести все элементы
    
    // Текстовая сериализация и десериализация
    void serialize(std::ostream& out) const;
    void deserialize(std::istream& in);
};