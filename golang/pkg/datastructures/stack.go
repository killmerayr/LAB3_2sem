package datastructures

import (
	"bufio"
	"io"
	"strconv"
	"strings"
)

// Stack - стек (LIFO)
type Stack struct {
	items []string
}

// NewStack создаёт новый пустой стек
func NewStack() *Stack {
	return &Stack{
		items: make([]string, 0),
	}
}

// IsEmpty проверяет, пуст ли стек
func (s *Stack) IsEmpty() bool {
	return len(s.items) == 0
}

// Push добавляет элемент на вершину стека
func (s *Stack) Push(val string) {
	s.items = append(s.items, val)
}

// Pop удаляет элемент с вершины стека
func (s *Stack) Pop() error {
	if s.IsEmpty() {
		return ErrEmpty
	}
	s.items = s.items[:len(s.items)-1]
	return nil
}

// Top получает элемент с вершины стека
func (s *Stack) Top() (string, error) {
	if s.IsEmpty() {
		return "", ErrEmpty
	}
	return s.items[len(s.items)-1], nil
}

// Size возвращает размер стека
func (s *Stack) Size() int {
	return len(s.items)
}

// Serialize сохраняет стек в текстовом формате
func (s *Stack) Serialize(w io.Writer) error {
	// Сохраняем размер
	if _, err := w.Write([]byte(strconv.Itoa(len(s.items)) + "\n")); err != nil {
		return err
	}

	// Сохраняем элементы от вершины вниз
	for i := len(s.items) - 1; i >= 0; i-- {
		if _, err := w.Write([]byte(s.items[i] + "\n")); err != nil {
			return err
		}
	}
	return nil
}

// Deserialize загружает стек из текстового формата
func (s *Stack) Deserialize(r io.Reader) error {
	// Очищаем текущий стек
	s.items = make([]string, 0)

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

	// Читаем элементы в обратном порядке
	items := make([]string, size)
	for i := 0; i < size; i++ {
		if !scanner.Scan() {
			return scanner.Err()
		}
		val := strings.TrimSpace(scanner.Text())
		items[i] = val
	}

	// Добавляем в обратном порядке для восстановления структуры
	for i := size - 1; i >= 0; i-- {
		s.Push(items[i])
	}

	return nil
}
