package datastructures

import (
	"bytes"
	"fmt"
	"testing"
)

// Tests for Array

func TestArrayPushBackAndGet(t *testing.T) {
	arr := NewArray()
	arr.PushBack("hello")
	arr.PushBack("world")

	val, _ := arr.GetAt(0)
	if val != "hello" {
		t.Errorf("expected 'hello', got '%s'", val)
	}

	val, _ = arr.GetAt(1)
	if val != "world" {
		t.Errorf("expected 'world', got '%s'", val)
	}
}

func TestArrayInsertAndDelete(t *testing.T) {
	arr := NewArray()
	arr.PushBack("x")
	arr.InsertAt(0, "y")

	val, _ := arr.GetAt(0)
	if val != "y" {
		t.Errorf("expected 'y', got '%s'", val)
	}

	arr.DelAt(0)
	val, _ = arr.GetAt(0)
	if val != "x" {
		t.Errorf("expected 'x', got '%s'", val)
	}
}

func TestArrayReplace(t *testing.T) {
	arr := NewArray()
	arr.PushBack("old")
	arr.ReplaceAt(0, "new")

	val, _ := arr.GetAt(0)
	if val != "new" {
		t.Errorf("expected 'new', got '%s'", val)
	}
}

func TestArrayIsEmpty(t *testing.T) {
	arr := NewArray()
	if !arr.IsEmpty() {
		t.Error("expected empty array")
	}

	arr.PushBack("item")
	if arr.IsEmpty() {
		t.Error("expected non-empty array")
	}
}

func TestArrayBinarySerialization(t *testing.T) {
	arr := NewArray()
	arr.PushBack("hello")
	arr.PushBack("world")
	arr.PushBack("test")

	var buf bytes.Buffer
	if err := arr.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	arr2 := NewArray()
	if err := arr2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if arr2.GetSize() != 3 {
		t.Errorf("expected size 3, got %d", arr2.GetSize())
	}

	val, _ := arr2.GetAt(0)
	if val != "hello" {
		t.Errorf("expected 'hello', got '%s'", val)
	}
}

func TestArrayLargeBinarySerialization(t *testing.T) {
	arr := NewArray()
	for i := 0; i < 100; i++ {
		arr.PushBack(fmt.Sprintf("item%d", i))
	}

	var buf bytes.Buffer
	if err := arr.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	arr2 := NewArray()
	if err := arr2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if arr2.GetSize() != 100 {
		t.Errorf("expected size 100, got %d", arr2.GetSize())
	}
}

func TestArrayInsertAtBoundary(t *testing.T) {
	arr := NewArray()
	arr.PushBack("a")
	arr.PushBack("b")

	arr.InsertAt(2, "c")

	val, _ := arr.GetAt(2)
	if val != "c" {
		t.Errorf("expected 'c', got '%s'", val)
	}
}

func TestArrayReserveAndGrow(t *testing.T) {
	arr := NewArray()
	arr.Reserve(20)

	for i := 0; i < 25; i++ {
		arr.PushBack(fmt.Sprintf("item%d", i))
	}

	if arr.GetSize() != 25 {
		t.Errorf("expected size 25, got %d", arr.GetSize())
	}
}

func TestArrayInsertAtWithGrowth(t *testing.T) {
	arr := NewArray()
	// Initially empty, capacity 0
	arr.InsertAt(0, "first")

	if arr.GetSize() != 1 {
		t.Errorf("expected size 1, got %d", arr.GetSize())
	}

	for i := 0; i < 10; i++ {
		arr.InsertAt(0, fmt.Sprintf("item%d", i))
	}

	if arr.GetSize() != 11 {
		t.Errorf("expected size 11, got %d", arr.GetSize())
	}
}

func TestArrayInsertAtErrors(t *testing.T) {
	arr := NewArray()
	arr.PushBack("a")

	if err := arr.InsertAt(-1, "x"); err != ErrIndexOutOfRange {
		t.Error("expected ErrIndexOutOfRange for negative index")
	}

	if err := arr.InsertAt(3, "x"); err != ErrIndexOutOfRange {
		t.Error("expected ErrIndexOutOfRange for index > size")
	}
}

func TestArrayDelAtErrors(t *testing.T) {
	arr := NewArray()
	arr.PushBack("a")

	if err := arr.DelAt(-1); err != ErrIndexOutOfRange {
		t.Error("expected ErrIndexOutOfRange for negative index")
	}

	if err := arr.DelAt(5); err != ErrIndexOutOfRange {
		t.Error("expected ErrIndexOutOfRange for out of range")
	}
}

func TestArrayGetAtErrors(t *testing.T) {
	arr := NewArray()
	arr.PushBack("a")

	if _, err := arr.GetAt(-1); err != ErrIndexOutOfRange {
		t.Error("expected ErrIndexOutOfRange for negative index")
	}

	if _, err := arr.GetAt(5); err != ErrIndexOutOfRange {
		t.Error("expected ErrIndexOutOfRange for out of range")
	}
}

func TestArrayReplaceAtErrors(t *testing.T) {
	arr := NewArray()
	arr.PushBack("a")

	if err := arr.ReplaceAt(-1, "x"); err != ErrIndexOutOfRange {
		t.Error("expected ErrIndexOutOfRange for negative index")
	}

	if err := arr.ReplaceAt(5, "x"); err != ErrIndexOutOfRange {
		t.Error("expected ErrIndexOutOfRange for out of range")
	}
}

// Benchmarks for Array

func BenchmarkArrayPushBack(b *testing.B) {
	arr := NewArray()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		arr.PushBack("item")
	}
}

func BenchmarkArrayInsertAt(b *testing.B) {
	arr := NewArray()
	for i := 0; i < 1000; i++ {
		arr.PushBack("item")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		arr.InsertAt(500, "new")
	}
}

func BenchmarkArrayGetAt(b *testing.B) {
	arr := NewArray()
	for i := 0; i < 10000; i++ {
		arr.PushBack("item")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		arr.GetAt(5000)
	}
}
