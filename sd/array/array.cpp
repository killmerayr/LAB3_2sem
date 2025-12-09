#include "array.hpp"
#include <iostream>
using namespace std;

// Конструктор
Mass::Mass() : data(nullptr), size(0), capacity(0) {}

// Деструктор
Mass::~Mass() { delete[] data; }

// Проверка на пустоту
bool Mass::is_empty() const { return size == 0; }

// Предварительное выделение памяти
void Mass::reserve(int new_capacity) {
  if (new_capacity <= capacity)
    return;
  string *new_data = new string[new_capacity];
  for (int i = 0; i < size; ++i) {
    new_data[i] = data[i];
  }
  delete[] data;
  data = new_data;
  capacity = new_capacity;
}

// Добавление элемента в конец
void Mass::push_back(const string &val) {
  if (size == capacity) {
    reserve(capacity == 0 ? 4 : capacity * 2);
  }
  data[size++] = val;
}

// Вставка элемента по индексу
void Mass::insert_at(int index, const string &val) {
  if (index < 0 || index > size)
    return;
  if (size == capacity) {
    reserve(capacity == 0 ? 4 : capacity * 2);
  }
  for (int i = size; i > index; --i) {
    data[i] = data[i - 1];
  }
  data[index] = val;
  ++size;
}

// Удаление элемента по индексу
void Mass::del_at(int index) {
  if (index < 0 || index >= size)
    return;
  for (int i = index; i < size - 1; ++i) {
    data[i] = data[i + 1];
  }
  --size;
}

// Получение элемента по индексу
string Mass::get_at(int index) const {
  if (index < 0 || index >= size)
    return "";
  return data[index];
}

// Замена элемента по индексу
void Mass::replace_at(int index, const string &val) {
  if (index < 0 || index >= size)
    return;
  data[index] = val;
}

// Получение размера массива
int Mass::get_size() const { return size; }

// Печать всех элементов
void Mass::print() const {
  for (int i = 0; i < size; ++i) {
    cout << data[i] << " ";
  }
  cout << endl;
}

// Считывание элементов с консоли
void Mass::read() {
  int n;
  cout << "Введите количество элементов: ";
  cin >> n;
  cin.ignore();

  for (int i = 0; i < n; ++i) {
    string val;
    cout << "Элемент " << i << ": ";
    getline(cin, val);
    push_back(val);
  }
}

// Бинарная сериализация
void Mass::serialize(std::ostream &out) const {
  out.write(reinterpret_cast<const char *>(&size), sizeof(int));
  for (int i = 0; i < size; ++i) {
    int len = data[i].length();
    out.write(reinterpret_cast<const char *>(&len), sizeof(int));
    out.write(data[i].c_str(), len);
  }
}

// Бинарная десериализация
void Mass::deserialize(std::istream &in) {
  int new_size = 0;
  in.read(reinterpret_cast<char *>(&new_size), sizeof(int));

  // Очищаем текущие данные
  delete[] data;
  data = nullptr;
  size = 0;
  capacity = 0;

  // Загружаем элементы
  for (int i = 0; i < new_size; ++i) {
    int len = 0;
    in.read(reinterpret_cast<char *>(&len), sizeof(int));

    char *buffer = new char[len + 1];
    in.read(buffer, len);
    buffer[len] = '\0';

    string val(buffer);
    delete[] buffer;
    push_back(val);
  }
}