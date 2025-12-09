#include "stack.hpp"
#include <iostream>
using namespace std;

// Конструктор
Stack::Stack() : top_node(nullptr) {}

// Деструктор
Stack::~Stack() {
  while (!is_empty()) {
    pop();
  }
}

// Проверка на пустоту
bool Stack::is_empty() const { return top_node == nullptr; }

// Поместить элемент на стек
void Stack::push(const string &val) {
  SNode *node = new SNode(val);
  node->next = top_node;
  top_node = node;
}

// Удалить верхний элемент
void Stack::pop() {
  if (is_empty()) {
    cout << "Стек пуст!\n";
    return;
  }
  SNode *tmp = top_node;
  top_node = top_node->next;
  delete tmp;
}

// Получить верхний элемент
string Stack::top() const {
  if (is_empty()) {
    cout << "Стек пуст!\n";
    return "";
  }
  return top_node->data;
}

// Вывести стек
void Stack::print() const {
  SNode *cur = top_node;
  while (cur) {
    cout << cur->data << " ";
    cur = cur->next;
  }
  cout << endl;
}

// Текстовая сериализация
void Stack::serialize(std::ostream &out) const {
  // Подсчитаем размер
  int size = 0;
  SNode *cur = top_node;
  while (cur) {
    size++;
    cur = cur->next;
  }

  out << size << "\n";

  // Сохраняем от верхушки вниз
  cur = top_node;
  while (cur) {
    out << cur->data << "\n";
    cur = cur->next;
  }
}

// Текстовая десериализация
void Stack::deserialize(std::istream &in) {
  int size = 0;
  in >> size;
  in.ignore(); // пропустить перевод строки

  // Очищаем стек
  while (!is_empty()) {
    pop();
  }

  // Загружаем элементы в обратном порядке (чтобы восстановить структуру)
  std::vector<std::string> temp;
  for (int i = 0; i < size; ++i) {
    std::string val;
    std::getline(in, val);
    temp.push_back(val);
  }

  // Добавляем в обратном порядке для восстановления стека
  for (int i = size - 1; i >= 0; --i) {
    push(temp[i]);
  }
}