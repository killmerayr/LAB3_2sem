#include "list.hpp"
#include <iostream>
#include <string>

using namespace std;

List::List() : head(nullptr), tail(nullptr) {}

List::~List() {
    LNode* cur = head;
    while (cur) {
        LNode* tmp = cur;
        cur = cur->next;
        delete tmp;
    }
}

bool List::is_empty() const {
    return head == nullptr;
}

int List::get_size() const{
    int count = 0;
    LNode* cur = head;
    while(cur){
        count += 1;
        cur = cur->next;
    }
    return count;
}

List::LNode* List::find(const std::string& key) const {
    LNode* cur = head;
    while (cur) {
        if (cur->data == key) return cur;
        cur = cur->next;
    }
    return nullptr;
}

// Добавление в конец
void List::push_back(const string& val) {
    LNode* node = new LNode(val);
    if (is_empty()) {
        head = tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
}

// Добавление в начало
void List::push_front(const string& val) {
    LNode* node = new LNode(val);
    node->next = head;
    head = node;
    if (!tail) tail = node;
}

// Вставка после заданного ключа
void List::insert_after(const string& key, const string& val) {
    LNode* cur = find(key);
    if (!cur) {
        cout << "Элемент '" << key << "' не найден.\n";
        return;
    }

    LNode* node = new LNode(val);
    node->next = cur->next;
    cur->next = node;
    if (cur == tail)
        tail = node;
}

// Вставка перед заданным ключом
void List::insert_before(const string& key, const string& val) {
    if (is_empty()) return;

    if (head->data == key) {
        push_front(val);
        return;
    }

    LNode* prev = nullptr;
    LNode* cur = head;

    while (cur && cur->data != key) {
        prev = cur;
        cur = cur->next;
    }

    if (!cur) {
        cout << "Элемент '" << key << "' не найден.\n";
        return;
    }

    LNode* node = new LNode(val);
    node->next = cur;
    prev->next = node;
}

// Удаление по значению
void List::del(const string& val) {
    if (is_empty()) return;

    if (head->data == val) {
        del_head();
        return;
    }

    LNode* prev = nullptr;
    LNode* cur = head;

    while (cur && cur->data != val) {
        prev = cur;
        cur = cur->next;
    }

    if (!cur) {
        cout << "Элемент '" << val << "' не найден.\n";
        return;
    }

    prev->next = cur->next;
    if (cur == tail)
        tail = prev;
    delete cur;
}

// Вывод списка
void List::print() const {
    LNode* cur = head;
    while (cur) {
        cout << cur->data << " ";
        cur = cur->next;
    }
    cout << endl;
}

// Получить элемент по индексу
List::LNode* List::get_at(int index) const {
    if (index < 0) return nullptr;
    LNode* cur = head;
    int i = 0;
    while (cur && i < index) {
        cur = cur->next;
        i++;
    }
    return cur;
}

// Удаление головы
void List::del_head() {
    if (is_empty()) return;

    LNode* tmp = head;
    head = head->next;
    delete tmp;

    if (!head) tail = nullptr;
}

// Удаление хвоста
void List::del_tail() {
    if (is_empty()) return;

    if (head == tail) {
        delete head;
        head = tail = nullptr;
        return;
    }

    LNode* cur = head;
    while (cur->next != tail)
        cur = cur->next;

    delete tail;
    tail = cur;
    tail->next = nullptr;
}

// Удалить элемент после ключа
void List::del_after(const string& key) {
    LNode* cur = find(key);
    if (!cur || !cur->next) return;

    LNode* tmp = cur->next;
    cur->next = tmp->next;
    if (tmp == tail)
        tail = cur;
    delete tmp;
}

// Удалить элемент перед ключом
void List::del_before(const string& key) {
    if (is_empty() || head->data == key) return;

    if (head->next && head->next->data == key) {
        del_head();
        return;
    }

    LNode* prevPrev = nullptr;
    LNode* prev = head;
    LNode* cur = head->next;

    while (cur && cur->data != key) {
        prevPrev = prev;
        prev = cur;
        cur = cur->next;
    }

    if (!cur) return; // не найден ключ

    prevPrev->next = cur;
    delete prev;
}