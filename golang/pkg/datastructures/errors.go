package datastructures

import "errors"

// ErrIndexOutOfRange - ошибка выхода за границы индекса
var ErrIndexOutOfRange = errors.New("index out of range")

// ErrEmpty - ошибка при обращении к пустой структуре
var ErrEmpty = errors.New("structure is empty")

// ErrNotFound - ошибка когда элемент не найден
var ErrNotFound = errors.New("element not found")
