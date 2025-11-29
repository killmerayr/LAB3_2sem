#include "queue.hpp"
#include <iostream>

using namespace std;

Queue::Queue() : first(nullptr), last(nullptr) {}

Queue::~Queue() {
    while (!is_empty()) {
        pop();
    }
}

bool Queue::is_empty() const {
    return first == nullptr;
}

void Queue::push(const string& value) {
    QNode* node = new QNode(value);
    if (is_empty()) {
        first = last = node;
    } else {
        last->next = node;
        last = node;
    }
}

void Queue::pop() {
    if (is_empty()) {
        cout << "Очередь пуста, удалять нечего.\n";
        return;
    }

    QNode* tmp = first;
    first = first->next;
    delete tmp;

    if (!first)
        last = nullptr;
}

string Queue::front() const {
    if (is_empty()) {
        cout << "Очередь пуста.\n";
        return "";
    }
    return first->val;
}

void Queue::print() const {
    if (is_empty()) {
        cout << "Очередь пуста.\n";
        return;
    }

    QNode* cur = first;
    while (cur) {
        cout << cur->val << " ";
        cur = cur->next;
    }
    cout << endl;
}