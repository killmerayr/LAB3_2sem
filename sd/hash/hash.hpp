#ifndef HASH_HPP
#define HASH_HPP

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class HashTable {
private:
    enum Status { Empty, Occupied, Deleted };

    vector<pair<string, Status>> table;
    int capacity;
    int size;
    int deleted_count;

    unsigned long Hash(const string& key) const {
        unsigned long hash = 5381;
        for (char c : key)
            hash = ((hash << 5) + hash) + c;
        return hash % capacity;
    }

    void Rehash() {
        int old_capacity = capacity;
        capacity *= 2;

        vector<pair<string, Status>> old_table = table;
        table.assign(capacity, {"", Empty});
        size = 0;
        deleted_count = 0;

        for (int i = 0; i < old_capacity; i++)
            if (old_table[i].second == Occupied)
                Add(old_table[i].first);
    }

    void Resize() {
        Rehash(); // просто дублируем для тестов, у тебя вызывался Resize()
    }

public:

    int get_size() const { return size; }
    int get_capacity() const { return capacity; } // опционально, для тестов

    HashTable(int initial_capacity = 8)
        : capacity(initial_capacity), size(0), deleted_count(0) {
        table.assign(capacity, {"", Empty});
    }

    bool Add(const string& key) {
        if (Find(key)) return false;

        if (size + deleted_count >= capacity * 0.7)
            Resize();

        unsigned long index = Hash(key);
        while (table[index].second == Occupied)
            index = (index + 1) % capacity;

        table[index] = { key, Occupied };
        size++;
        return true;
    }

    bool Find(const string& key) const {
        unsigned long index = Hash(key);
        int steps = 0;

        while (steps < capacity) {
            if (table[index].second == Empty)
                return false;
            if (table[index].second == Occupied && table[index].first == key)
                return true;
            index = (index + 1) % capacity;
            steps++;
        }
        return false;
    }

    bool Remove(const string& key) {
        unsigned long index = Hash(key);
        int steps = 0;

        while (steps < capacity) {
            if (table[index].second == Empty)
                return false;
            if (table[index].second == Occupied && table[index].first == key) {
                table[index].second = Deleted;
                size--;
                deleted_count++;
                return true;
            }
            index = (index + 1) % capacity;
            steps++;
        }
        return false;
    }

    void Print() const {
        for (int i = 0; i < capacity; i++) {
            if (table[i].second == Occupied)
                cout << table[i].first << " ";
        }
        cout << endl;
    }
};

#endif