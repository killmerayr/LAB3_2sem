#include "queue.hpp"
#include <iostream>

using namespace std;

Queue::Queue() : first(nullptr), last(nullptr) {}

Queue::~Queue() {
  while (!is_empty()) {
    pop();
  }
}

bool Queue::is_empty() const { return first == nullptr; }

void Queue::push(const string &value) {
  QNode *node = new QNode(value);
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

  QNode *tmp = first;
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

  QNode *cur = first;
  while (cur) {
    cout << cur->val << " ";
    cur = cur->next;
  }
  cout << endl;
}

// Текстовая сериализация
void Queue::serialize(std::ostream &out) const {
  // Подсчитаем размер
  int size = 0;
  QNode *cur = first;
  while (cur) {
    size++;
    cur = cur->next;
  }

  out << size << "\n";

  // Сохраняем от начала к концу
  cur = first;
  while (cur) {
    out << cur->val << "\n";
    cur = cur->next;
  }
}

// Текстовая десериализация
void Queue::deserialize(std::istream &in) {
  int size = 0;
  in >> size;
  in.ignore(); // пропустить перевод строки

  // Очищаем очередь
  while (!is_empty()) {
    pop();
  }

  // Загружаем элементы в прямом порядке
  for (int i = 0; i < size; ++i) {
    std::string val;
    std::getline(in, val);
    push(val);
  }
}