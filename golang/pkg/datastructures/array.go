package datastructures

import (
	"encoding/binary"
	"io"
)

// Array - динамический массив строк
type Array struct {
	data     []string
	size     int
	capacity int
}

// NewArray создаёт новый пустой массив
func NewArray() *Array {
	return &Array{
		data:     make([]string, 0),
		size:     0,
		capacity: 0,
	}
}

// IsEmpty проверяет, пуст ли массив
func (a *Array) IsEmpty() bool {
	return a.size == 0
}

// Reserve выделяет память для массива
func (a *Array) Reserve(newCapacity int) {
	if newCapacity <= a.capacity {
		return
	}
	newData := make([]string, newCapacity)
	copy(newData, a.data[:a.size])
	a.data = newData
	a.capacity = newCapacity
}

// PushBack добавляет элемент в конец
func (a *Array) PushBack(val string) {
	if a.size == a.capacity {
		if a.capacity == 0 {
			a.Reserve(4)
		} else {
			a.Reserve(a.capacity * 2)
		}
	}
	a.data[a.size] = val
	a.size++
}

// InsertAt вставляет элемент по индексу
func (a *Array) InsertAt(index int, val string) error {
	if index < 0 || index > a.size {
		return ErrIndexOutOfRange
	}

	if a.size == a.capacity {
		if a.capacity == 0 {
			a.Reserve(4)
		} else {
			a.Reserve(a.capacity * 2)
		}
	}

	for i := a.size; i > index; i-- {
		a.data[i] = a.data[i-1]
	}
	a.data[index] = val
	a.size++
	return nil
}

// DelAt удаляет элемент по индексу
func (a *Array) DelAt(index int) error {
	if index < 0 || index >= a.size {
		return ErrIndexOutOfRange
	}

	for i := index; i < a.size-1; i++ {
		a.data[i] = a.data[i+1]
	}
	a.size--
	return nil
}

// GetAt получает элемент по индексу
func (a *Array) GetAt(index int) (string, error) {
	if index < 0 || index >= a.size {
		return "", ErrIndexOutOfRange
	}
	return a.data[index], nil
}

// ReplaceAt заменяет элемент по индексу
func (a *Array) ReplaceAt(index int, val string) error {
	if index < 0 || index >= a.size {
		return ErrIndexOutOfRange
	}
	a.data[index] = val
	return nil
}

// GetSize возвращает размер массива
func (a *Array) GetSize() int {
	return a.size
}

// Serialize сохраняет массив в бинарном формате
func (a *Array) Serialize(w io.Writer) error {
	// Сохраняем размер
	if err := binary.Write(w, binary.LittleEndian, int32(a.size)); err != nil {
		return err
	}

	// Сохраняем каждый элемент
	for i := 0; i < a.size; i++ {
		len := int32(len(a.data[i]))
		if err := binary.Write(w, binary.LittleEndian, len); err != nil {
			return err
		}
		if _, err := w.Write([]byte(a.data[i])); err != nil {
			return err
		}
	}
	return nil
}

// Deserialize загружает массив из бинарного формата
func (a *Array) Deserialize(r io.Reader) error {
	// Очищаем текущие данные
	a.data = make([]string, 0)
	a.size = 0
	a.capacity = 0

	// Загружаем размер
	var size int32
	if err := binary.Read(r, binary.LittleEndian, &size); err != nil {
		return err
	}

	// Загружаем элементы
	for i := 0; i < int(size); i++ {
		var len int32
		if err := binary.Read(r, binary.LittleEndian, &len); err != nil {
			return err
		}

		buf := make([]byte, len)
		if _, err := r.Read(buf); err != nil {
			return err
		}

		a.PushBack(string(buf))
	}
	return nil
}
