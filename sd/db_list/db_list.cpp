#include "db_list.hpp"
#include <iostream>
using namespace std;

DoublyList::DoublyList() : head(nullptr), tail(nullptr) {}

DoublyList::~DoublyList() {
    DNode* cur = head;
    while (cur) {
        DNode* tmp = cur;
        cur = cur->next;
        delete tmp;
    }
}

bool DoublyList::is_empty() const {
    return head == nullptr;
}

DoublyList::DNode* DoublyList::find(const string& key) const {
    DNode* cur = head;
    while (cur) {
        if (cur->data == key) return cur;
        cur = cur->next;
    }
    return nullptr;
}

int DoublyList::get_size() const {
    int count = 0;
    DNode* cur = head;
    while (cur) {
        ++count;
        cur = cur->next;
    }
    return count;
}

// Добавить в конец
void DoublyList::push_back(const string& val) {
    DNode* node = new DNode(val);
    if (is_empty()) {
        head = tail = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
}

// Добавить в начало
void DoublyList::push_front(const string& val) {
    DNode* node = new DNode(val);
    if (is_empty()) {
        head = tail = node;
    } else {
        node->next = head;
        head->prev = node;
        head = node;
    }
}

// Вставка после ключа
void DoublyList::insert_after(const string& key, const string& val) {
    DNode* cur = find(key);
    if (!cur) {
        cout << "Элемент '" << key << "' не найден.\n";
        return;
    }

    DNode* node = new DNode(val);
    node->next = cur->next;
    node->prev = cur;
    if (cur->next)
        cur->next->prev = node;
    cur->next = node;
    if (cur == tail)
        tail = node;
}

// Вставка перед ключом
void DoublyList::insert_before(const string& key, const string& val) {
    DNode* cur = find(key);
    if (!cur) {
        cout << "Элемент '" << key << "' не найден.\n";
        return;
    }

    DNode* node = new DNode(val);
    node->next = cur;
    node->prev = cur->prev;
    if (cur->prev)
        cur->prev->next = node;
    else
        head = node;
    cur->prev = node;
}

// Удалить элемент по значению
void DoublyList::del(const string& val) {
    DNode* cur = find(val);
    if (!cur) {
        cout << "Элемент '" << val << "' не найден.\n";
        return;
    }

    if (cur->prev)
        cur->prev->next = cur->next;
    else
        head = cur->next;

    if (cur->next)
        cur->next->prev = cur->prev;
    else
        tail = cur->prev;

    delete cur;
}

// Удалить голову
void DoublyList::del_head() {
    if (is_empty()) return;
    DNode* tmp = head;
    head = head->next;
    if (head)
        head->prev = nullptr;
    else
        tail = nullptr;
    delete tmp;
}

// Удалить хвост
void DoublyList::del_tail() {
    if (is_empty()) return;
    DNode* tmp = tail;
    tail = tail->prev;
    if (tail)
        tail->next = nullptr;
    else
        head = nullptr;
    delete tmp;
}

// Удалить элемент после ключа
void DoublyList::del_after(const string& key) {
    DNode* cur = find(key);
    if (!cur || !cur->next) return;
    DNode* tmp = cur->next;

    cur->next = tmp->next;
    if (tmp->next)
        tmp->next->prev = cur;
    else
        tail = cur;

    delete tmp;
}

// Удалить элемент перед ключом
void DoublyList::del_before(const string& key) {
    DNode* cur = find(key);
    if (!cur || !cur->prev) return;
    DNode* tmp = cur->prev;

    cur->prev = tmp->prev;
    if (tmp->prev)
        tmp->prev->next = cur;
    else
        head = cur;

    delete tmp;
}

// Получить элемент по индексу
DoublyList::DNode* DoublyList::get_at(int index) const {
    if (index < 0) return nullptr;
    DNode* cur = head;
    int i = 0;
    while (cur && i < index) {
        cur = cur->next;
        i++;
    }
    return cur;
}

// Печать вперёд
void DoublyList::print_forward() const {
    DNode* cur = head;
    while (cur) {
        cout << cur->data << " ";
        cur = cur->next;
    }
    cout << endl;
}

// Печать назад
void DoublyList::print_backward() const {
    DNode* cur = tail;
    while (cur) {
        cout << cur->data << " ";
        cur = cur->prev;
    }
    cout << endl;
}