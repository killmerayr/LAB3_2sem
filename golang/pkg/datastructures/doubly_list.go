package datastructures

import (
	"bufio"
	"io"
	"strconv"
	"strings"
)

// DNode представляет узел в двусвязном списке
type DNode struct {
	data string
	next *DNode
	prev *DNode
}

// DoublyList - двусвязный список
type DoublyList struct {
	head *DNode
	tail *DNode
	size int
}

// NewDoublyList создаёт новый пустой двусвязный список
func NewDoublyList() *DoublyList {
	return &DoublyList{
		head: nil,
		tail: nil,
		size: 0,
	}
}

// IsEmpty проверяет, пуст ли список
func (dl *DoublyList) IsEmpty() bool {
	return dl.head == nil
}

// GetSize возвращает размер списка
func (dl *DoublyList) GetSize() int {
	return dl.size
}

// PushBack добавляет элемент в конец
func (dl *DoublyList) PushBack(val string) {
	node := &DNode{data: val}
	if dl.IsEmpty() {
		dl.head = node
		dl.tail = node
	} else {
		node.prev = dl.tail
		dl.tail.next = node
		dl.tail = node
	}
	dl.size++
}

// PushFront добавляет элемент в начало
func (dl *DoublyList) PushFront(val string) {
	node := &DNode{data: val}
	if dl.IsEmpty() {
		dl.head = node
		dl.tail = node
	} else {
		node.next = dl.head
		dl.head.prev = node
		dl.head = node
	}
	dl.size++
}

// Find ищет элемент
func (dl *DoublyList) Find(key string) *DNode {
	current := dl.head
	for current != nil {
		if current.data == key {
			return current
		}
		current = current.next
	}
	return nil
}

// GetAt получает элемент по индексу
func (dl *DoublyList) GetAt(index int) *DNode {
	if index < 0 || index >= dl.size {
		return nil
	}
	current := dl.head
	for i := 0; i < index; i++ {
		current = current.next
	}
	return current
}

// Del удаляет элемент по значению
func (dl *DoublyList) Del(val string) error {
	current := dl.head
	for current != nil {
		if current.data == val {
			if current.prev != nil {
				current.prev.next = current.next
			} else {
				dl.head = current.next
			}
			if current.next != nil {
				current.next.prev = current.prev
			} else {
				dl.tail = current.prev
			}
			dl.size--
			return nil
		}
		current = current.next
	}
	return ErrNotFound
}

// Serialize сохраняет список в текстовом формате
func (dl *DoublyList) Serialize(w io.Writer) error {
	// Сохраняем размер
	if _, err := w.Write([]byte(strconv.Itoa(dl.size) + "\n")); err != nil {
		return err
	}

	// Сохраняем элементы
	current := dl.head
	for current != nil {
		if _, err := w.Write([]byte(current.data + "\n")); err != nil {
			return err
		}
		current = current.next
	}
	return nil
}

// Deserialize загружает список из текстового формата
func (dl *DoublyList) Deserialize(r io.Reader) error {
	// Очищаем текущий список
	dl.head = nil
	dl.tail = nil
	dl.size = 0

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
		dl.PushBack(val)
	}

	return nil
}
