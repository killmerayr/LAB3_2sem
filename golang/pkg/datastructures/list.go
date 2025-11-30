package datastructures

import (
	"bufio"
	"io"
	"strconv"
	"strings"
)

// Node представляет узел в односвязном списке
type Node struct {
	data string
	next *Node
}

// List - односвязный список
type List struct {
	head *Node
	tail *Node
	size int
}

// NewList создаёт новый пустой список
func NewList() *List {
	return &List{
		head: nil,
		tail: nil,
		size: 0,
	}
}

// IsEmpty проверяет, пуст ли список
func (l *List) IsEmpty() bool {
	return l.head == nil
}

// GetSize возвращает размер списка
func (l *List) GetSize() int {
	return l.size
}

// PushBack добавляет элемент в конец
func (l *List) PushBack(val string) {
	node := &Node{data: val}
	if l.IsEmpty() {
		l.head = node
		l.tail = node
	} else {
		l.tail.next = node
		l.tail = node
	}
	l.size++
}

// PushFront добавляет элемент в начало
func (l *List) PushFront(val string) {
	node := &Node{data: val}
	if l.IsEmpty() {
		l.head = node
		l.tail = node
	} else {
		node.next = l.head
		l.head = node
	}
	l.size++
}

// Find ищет элемент
func (l *List) Find(key string) *Node {
	current := l.head
	for current != nil {
		if current.data == key {
			return current
		}
		current = current.next
	}
	return nil
}

// GetAt получает элемент по индексу
func (l *List) GetAt(index int) *Node {
	if index < 0 || index >= l.size {
		return nil
	}
	current := l.head
	for i := 0; i < index; i++ {
		current = current.next
	}
	return current
}

// Del удаляет элемент по значению
func (l *List) Del(val string) error {
	if l.IsEmpty() {
		return ErrEmpty
	}

	if l.head.data == val {
		l.head = l.head.next
		if l.head == nil {
			l.tail = nil
		}
		l.size--
		return nil
	}

	current := l.head
	for current.next != nil {
		if current.next.data == val {
			current.next = current.next.next
			if current.next == nil {
				l.tail = current
			}
			l.size--
			return nil
		}
		current = current.next
	}
	return ErrNotFound
}

// Serialize сохраняет список в текстовом формате
func (l *List) Serialize(w io.Writer) error {
	// Сохраняем размер
	if _, err := w.Write([]byte(strconv.Itoa(l.size) + "\n")); err != nil {
		return err
	}

	// Сохраняем элементы
	current := l.head
	for current != nil {
		if _, err := w.Write([]byte(current.data + "\n")); err != nil {
			return err
		}
		current = current.next
	}
	return nil
}

// Deserialize загружает список из текстового формата
func (l *List) Deserialize(r io.Reader) error {
	// Очищаем текущий список
	l.head = nil
	l.tail = nil
	l.size = 0

	scanner := bufio.NewScanner(r)

	// Читаем размер
	if !scanner.Scan() {
		return scanner.Err()
	}
	sizeStr := strings.TrimSpace(scanner.Text())
	size, err := strconv.Atoi(sizeStr)
	if err != nil {
		return err
	}

	// Читаем элементы
	for i := 0; i < size; i++ {
		if !scanner.Scan() {
			return scanner.Err()
		}
		val := strings.TrimSpace(scanner.Text())
		l.PushBack(val)
	}

	return nil
}
