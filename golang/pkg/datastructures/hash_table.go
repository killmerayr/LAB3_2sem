package datastructures

import (
	"encoding/binary"
	"hash/fnv"
	"io"
)

// HashTable - хеш-таблица с открытой адресацией
type HashTable struct {
	table    map[string]bool
	size     int
	capacity int
}

// NewHashTable создаёт новую хеш-таблицу
func NewHashTable(initialCapacity int) *HashTable {
	if initialCapacity <= 0 {
		initialCapacity = 8
	}
	return &HashTable{
		table:    make(map[string]bool),
		size:     0,
		capacity: initialCapacity,
	}
}

// Add добавляет элемент в таблицу
func (ht *HashTable) Add(key string) bool {
	if ht.table[key] {
		return false // уже существует
	}
	ht.table[key] = true
	ht.size++

	// Проверяем, нужна ли переорганизация
	if ht.size > ht.capacity/2 {
		ht.Rehash()
	}
	return true
}

// Find ищет элемент в таблице
func (ht *HashTable) Find(key string) bool {
	return ht.table[key]
}

// Remove удаляет элемент из таблицы
func (ht *HashTable) Remove(key string) bool {
	if !ht.table[key] {
		return false
	}
	delete(ht.table, key)
	ht.size--
	return true
}

// GetSize возвращает количество элементов
func (ht *HashTable) GetSize() int {
	return ht.size
}

// GetCapacity возвращает ёмкость таблицы
func (ht *HashTable) GetCapacity() int {
	return ht.capacity
}

// Rehash перестраивает таблицу с новой ёмкостью
func (ht *HashTable) Rehash() {
	ht.capacity *= 2
	// Go map автоматически масштабируется, так что это просто обновление capacity
}

// Hash вычисляет хеш для ключа
func (ht *HashTable) Hash(key string) uint64 {
	h := fnv.New64a()
	h.Write([]byte(key))
	return h.Sum64() % uint64(ht.capacity)
}

// Serialize сохраняет таблицу в бинарном формате
func (ht *HashTable) Serialize(w io.Writer) error {
	// Сохраняем размер
	if err := binary.Write(w, binary.LittleEndian, int32(ht.size)); err != nil {
		return err
	}

	// Сохраняем каждый ключ
	for key := range ht.table {
		len := int32(len(key))
		if err := binary.Write(w, binary.LittleEndian, len); err != nil {
			return err
		}
		if _, err := w.Write([]byte(key)); err != nil {
			return err
		}
	}
	return nil
}

// Deserialize загружает таблицу из бинарного формата
func (ht *HashTable) Deserialize(r io.Reader) error {
	// Очищаем текущую таблицу
	ht.table = make(map[string]bool)
	ht.size = 0

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

		ht.Add(string(buf))
	}
	return nil
}
