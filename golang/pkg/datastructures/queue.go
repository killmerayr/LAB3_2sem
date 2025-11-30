package datastructures

import (
	"bufio"
	"io"
	"strconv"
	"strings"
)

// Queue - очередь (FIFO)
type Queue struct {
	items []string
}

// NewQueue создаёт новую пустую очередь
func NewQueue() *Queue {
	return &Queue{
		items: make([]string, 0),
	}
}

// IsEmpty проверяет, пуста ли очередь
func (q *Queue) IsEmpty() bool {
	return len(q.items) == 0
}

// Push добавляет элемент в конец очереди
func (q *Queue) Push(val string) {
	q.items = append(q.items, val)
}

// Pop удаляет элемент из начала очереди
func (q *Queue) Pop() error {
	if q.IsEmpty() {
		return ErrEmpty
	}
	q.items = q.items[1:]
	return nil
}

// Front получает элемент из начала очереди
func (q *Queue) Front() (string, error) {
	if q.IsEmpty() {
		return "", ErrEmpty
	}
	return q.items[0], nil
}

// Size возвращает размер очереди
func (q *Queue) Size() int {
	return len(q.items)
}

// Serialize сохраняет очередь в текстовом формате
func (q *Queue) Serialize(w io.Writer) error {
	// Сохраняем размер
	if _, err := w.Write([]byte(strconv.Itoa(len(q.items)) + "\n")); err != nil {
		return err
	}

	// Сохраняем элементы в прямом порядке
	for _, item := range q.items {
		if _, err := w.Write([]byte(item + "\n")); err != nil {
			return err
		}
	}
	return nil
}

// Deserialize загружает очередь из текстового формата
func (q *Queue) Deserialize(r io.Reader) error {
	// Очищаем текущую очередь
	q.items = make([]string, 0)

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

	// Читаем элементы в прямом порядке
	for i := 0; i < size; i++ {
		if !scanner.Scan() {
			return scanner.Err()
		}
		val := strings.TrimSpace(scanner.Text())
		q.Push(val)
	}

	return nil
}
