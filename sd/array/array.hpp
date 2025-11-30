#pragma once
#include <string>
#include <ostream>
#include <istream>

class Mass {
private:
    std::string* data;
    int size;
    int capacity;

public:
    Mass();
    ~Mass();

    bool is_empty() const;
    void reserve(int new_capacity);
    void push_back(const std::string& val);
    void insert_at(int index, const std::string& val);
    void del_at(int index);
    std::string get_at(int index) const;
    void replace_at(int index, const std::string& val);
    int get_size() const;
    void print() const;
    void read(); // считывание с консоли
    
    // Бинарная сериализация и десериализация
    void serialize(std::ostream& out) const;
    void deserialize(std::istream& in);
};