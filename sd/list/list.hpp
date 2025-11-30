#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <string>

class List{
private:
    struct LNode{
        std::string data;
        LNode* next;
        LNode(const std::string& val) : data(val), next(nullptr) {}
    };
    LNode* head;
    LNode* tail;

public:
    List();
    ~List();

    List(const List&) = delete;
    List& operator=(const List&) = delete;

    bool is_empty() const;
    int get_size() const;

    LNode* find(const std::string& key) const;
    void push_back(const std::string& val);
    void insert_after(const std::string& key, const std::string& val);
    void push_front(const std::string& val);
    void insert_before(const std::string& key, const std::string& val);
    void del(const std::string& val);
    void print() const;
    LNode* get_at(int index) const;
    void del_tail();
    void del_head();
    void del_after(const std::string& key);
    void del_before(const std::string& key);
    
    // Текстовая сериализация и десериализация
    void serialize(std::ostream& out) const;
    void deserialize(std::istream& in);
};

#endif