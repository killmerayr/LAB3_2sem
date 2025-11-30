#pragma once
#include <string>
#include <ostream>
#include <istream>

class DoublyList {
private:
    struct DNode {
        std::string data;
        DNode* next;
        DNode* prev;
        DNode(const std::string& val) : data(val), next(nullptr), prev(nullptr) {}
    };

    DNode* head;
    DNode* tail;

public:
    DoublyList();
    ~DoublyList();

    bool is_empty() const;
    int get_size() const;

    DNode* find(const std::string& key) const;
    void push_back(const std::string& val);
    void push_front(const std::string& val);
    void insert_after(const std::string& key, const std::string& val);
    void insert_before(const std::string& key, const std::string& val);
    void del(const std::string& val);
    void del_head();
    void del_tail();
    void del_after(const std::string& key);
    void del_before(const std::string& key);
    DNode* get_at(int index) const;
    void print_forward() const;
    void print_backward() const;
    
    // Текстовая сериализация и десериализация
    void serialize(std::ostream& out) const;
    void deserialize(std::istream& in);
};