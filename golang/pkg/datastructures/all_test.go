package datastructures

import (
	"bytes"
	"fmt"
	"io"
	"testing"
	"time"
)

// FailingReader fails after N bytes
type FailingReader struct {
	data []byte
	pos  int
	fail int
}

func NewFailingReader(data []byte, failAt int) *FailingReader {
	return &FailingReader{
		data: data,
		pos:  0,
		fail: failAt,
	}
}

func (fr *FailingReader) Read(p []byte) (n int, err error) {
	if fr.pos >= fr.fail {
		return 0, io.EOF
	}
	if fr.pos+len(p) > fr.fail {
		n = fr.fail - fr.pos
	} else {
		n = len(p)
	}
	copy(p, fr.data[fr.pos:fr.pos+n])
	fr.pos += n
	return
}

// ========== ARRAY TESTS ==========
// Additional tests for better coverage

func TestArrayInsertAtMultiplePositions(t *testing.T) {
	arr := NewArray()
	arr.PushBack("a")
	arr.PushBack("d")

	arr.InsertAt(1, "c")
	arr.InsertAt(1, "b")

	if arr.GetSize() != 4 {
		t.Errorf("expected size 4, got %d", arr.GetSize())
	}

	val, _ := arr.GetAt(1)
	if val != "b" {
		t.Errorf("expected 'b', got '%s'", val)
	}
}

func TestArrayDelAtEdgeCases(t *testing.T) {
	arr := NewArray()
	arr.PushBack("a")
	arr.PushBack("b")
	arr.PushBack("c")

	arr.DelAt(0)

	val, _ := arr.GetAt(0)
	if val != "b" {
		t.Errorf("expected 'b', got '%s'", val)
	}
}

func TestArrayGetAtIndexBoundary(t *testing.T) {
	arr := NewArray()
	for i := 0; i < 10; i++ {
		arr.PushBack(fmt.Sprintf("item%d", i))
	}

	val, _ := arr.GetAt(9)
	if val != "item9" {
		t.Errorf("expected 'item9', got '%s'", val)
	}
}

// ========== LIST TESTS ==========
// Note: Array tests are in array_test.go

func TestListPushBack(t *testing.T) {
	list := NewList()
	list.PushBack("first")
	list.PushBack("second")

	if list.GetSize() != 2 {
		t.Errorf("expected size 2, got %d", list.GetSize())
	}

	node := list.GetAt(0)
	if node.data != "first" {
		t.Errorf("expected 'first', got '%s'", node.data)
	}
}

func TestListPushFront(t *testing.T) {
	list := NewList()
	list.PushFront("first")
	list.PushFront("second")

	node := list.GetAt(0)
	if node.data != "second" {
		t.Errorf("expected 'second', got '%s'", node.data)
	}
}

func TestListFind(t *testing.T) {
	list := NewList()
	list.PushBack("alice")
	list.PushBack("bob")

	node := list.Find("bob")
	if node == nil || node.data != "bob" {
		t.Error("expected to find 'bob'")
	}

	node = list.Find("charlie")
	if node != nil {
		t.Error("expected not to find 'charlie'")
	}
}

func TestListDelete(t *testing.T) {
	list := NewList()
	list.PushBack("a")
	list.PushBack("b")
	list.PushBack("c")

	if err := list.Del("b"); err != nil {
		t.Errorf("delete error: %v", err)
	}
	if list.GetSize() != 2 {
		t.Errorf("expected size 2, got %d", list.GetSize())
	}
}

func TestListDeleteNotFound(t *testing.T) {
	list := NewList()
	list.PushBack("a")

	if err := list.Del("z"); err != ErrNotFound {
		t.Error("expected ErrNotFound")
	}
}

func TestListTextSerialization(t *testing.T) {
	list := NewList()
	list.PushBack("first")
	list.PushBack("second")
	list.PushBack("third")

	var buf bytes.Buffer
	if err := list.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	list2 := NewList()
	if err := list2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if list2.GetSize() != 3 {
		t.Errorf("expected size 3, got %d", list2.GetSize())
	}
}

func TestListTextSerializationEmpty(t *testing.T) {
	list := NewList()

	var buf bytes.Buffer
	if err := list.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	list2 := NewList()
	if err := list2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if list2.GetSize() != 0 {
		t.Errorf("expected size 0, got %d", list2.GetSize())
	}
}

func TestListGetAt(t *testing.T) {
	list := NewList()
	list.PushBack("a")
	list.PushBack("b")
	list.PushBack("c")

	node := list.GetAt(1)
	if node.data != "b" {
		t.Errorf("expected 'b', got '%s'", node.data)
	}

	node = list.GetAt(2)
	if node.data != "c" {
		t.Errorf("expected 'c', got '%s'", node.data)
	}
}

func TestListFindEmpty(t *testing.T) {
	list := NewList()
	node := list.Find("anything")
	if node != nil {
		t.Error("expected nil when finding in empty list")
	}
}

func TestListDeleteHeadElement(t *testing.T) {
	list := NewList()
	list.PushBack("head")
	list.PushBack("tail")

	if err := list.Del("head"); err != nil {
		t.Errorf("delete error: %v", err)
	}

	if list.GetSize() != 1 {
		t.Errorf("expected size 1, got %d", list.GetSize())
	}

	node := list.GetAt(0)
	if node.data != "tail" {
		t.Errorf("expected 'tail', got '%s'", node.data)
	}
}

func TestListLargeTextSerialization(t *testing.T) {
	list := NewList()
	for i := 0; i < 100; i++ {
		list.PushBack(fmt.Sprintf("item%d", i))
	}

	var buf bytes.Buffer
	if err := list.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	list2 := NewList()
	if err := list2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if list2.GetSize() != 100 {
		t.Errorf("expected size 100, got %d", list2.GetSize())
	}
}

func BenchmarkListPushBack(b *testing.B) {
	list := NewList()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.PushBack("item")
	}
}

func BenchmarkListFind(b *testing.B) {
	list := NewList()
	for i := 0; i < 10000; i++ {
		list.PushBack("item")
	}
	list.PushBack("target")
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.Find("target")
	}
}

func TestListGetSize(t *testing.T) {
	list := NewList()
	if list.GetSize() != 0 {
		t.Error("expected size 0 for empty list")
	}

	list.PushBack("a")
	if list.GetSize() != 1 {
		t.Error("expected size 1")
	}

	list.PushBack("b")
	if list.GetSize() != 2 {
		t.Error("expected size 2")
	}
}

func TestListMultipleDeletes(t *testing.T) {
	list := NewList()
	list.PushBack("a")
	list.PushBack("b")
	list.PushBack("c")

	list.Del("a")
	list.Del("c")

	if list.GetSize() != 1 {
		t.Errorf("expected size 1, got %d", list.GetSize())
	}

	node := list.GetAt(0)
	if node.data != "b" {
		t.Errorf("expected 'b', got '%s'", node.data)
	}
}

// ========== DOUBLY LIST TESTS ==========

func TestDoublyListPushBack(t *testing.T) {
	dl := NewDoublyList()
	dl.PushBack("first")
	dl.PushBack("second")

	if dl.GetSize() != 2 {
		t.Errorf("expected size 2, got %d", dl.GetSize())
	}
}

func TestDoublyListTextSerialization(t *testing.T) {
	dl := NewDoublyList()
	dl.PushBack("node1")
	dl.PushBack("node2")

	var buf bytes.Buffer
	if err := dl.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	dl2 := NewDoublyList()
	if err := dl2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if dl2.GetSize() != 2 {
		t.Errorf("expected size 2, got %d", dl2.GetSize())
	}
}

func TestDoublyListTextSerializationEmpty(t *testing.T) {
	dl := NewDoublyList()

	var buf bytes.Buffer
	if err := dl.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	dl2 := NewDoublyList()
	if err := dl2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if dl2.GetSize() != 0 {
		t.Errorf("expected size 0, got %d", dl2.GetSize())
	}
}

func TestDoublyListPushFront(t *testing.T) {
	dl := NewDoublyList()
	dl.PushFront("first")
	dl.PushFront("second")

	node := dl.GetAt(0)
	if node.data != "second" {
		t.Errorf("expected 'second', got '%s'", node.data)
	}
}

func TestDoublyListFind(t *testing.T) {
	dl := NewDoublyList()
	dl.PushBack("a")
	dl.PushBack("b")
	dl.PushBack("c")

	node := dl.Find("b")
	if node == nil || node.data != "b" {
		t.Error("expected to find 'b'")
	}

	node = dl.Find("z")
	if node != nil {
		t.Error("expected not to find 'z'")
	}
}

func TestDoublyListDelete(t *testing.T) {
	dl := NewDoublyList()
	dl.PushBack("x")
	dl.PushBack("y")
	dl.PushBack("z")

	if err := dl.Del("y"); err != nil {
		t.Errorf("delete error: %v", err)
	}

	if dl.GetSize() != 2 {
		t.Errorf("expected size 2, got %d", dl.GetSize())
	}
}

func BenchmarkDoublyListPushBack(b *testing.B) {
	dl := NewDoublyList()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		dl.PushBack("item")
	}
}

func TestDoublyListGetAt(t *testing.T) {
	dl := NewDoublyList()
	dl.PushBack("first")
	dl.PushBack("second")
	dl.PushBack("third")

	node := dl.GetAt(1)
	if node.data != "second" {
		t.Errorf("expected 'second', got '%s'", node.data)
	}
}

func TestDoublyListFindEmpty(t *testing.T) {
	dl := NewDoublyList()
	node := dl.Find("anything")
	if node != nil {
		t.Error("expected nil when finding in empty list")
	}
}

func TestDoublyListDeleteNotFound(t *testing.T) {
	dl := NewDoublyList()
	dl.PushBack("a")

	if err := dl.Del("z"); err != ErrNotFound {
		t.Error("expected ErrNotFound")
	}
}

func TestDoublyListGetSize(t *testing.T) {
	dl := NewDoublyList()
	if dl.GetSize() != 0 {
		t.Error("expected size 0")
	}

	dl.PushBack("a")
	if dl.GetSize() != 1 {
		t.Error("expected size 1")
	}
}

func TestDoublyListLargeTextSerialization(t *testing.T) {
	dl := NewDoublyList()
	for i := 0; i < 50; i++ {
		dl.PushBack(fmt.Sprintf("node%d", i))
	}

	var buf bytes.Buffer
	if err := dl.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	dl2 := NewDoublyList()
	if err := dl2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if dl2.GetSize() != 50 {
		t.Errorf("expected size 50, got %d", dl2.GetSize())
	}
}

func TestDoublyListDeleteTail(t *testing.T) {
	dl := NewDoublyList()
	dl.PushBack("a")
	dl.PushBack("b")
	dl.PushBack("c")

	if err := dl.Del("c"); err != nil {
		t.Errorf("delete error: %v", err)
	}

	if dl.GetSize() != 2 {
		t.Errorf("expected size 2, got %d", dl.GetSize())
	}
}

// ========== STACK TESTS ==========

func TestStackPushPop(t *testing.T) {
	stack := NewStack()
	stack.Push("first")
	stack.Push("second")

	val, _ := stack.Top()
	if val != "second" {
		t.Errorf("expected 'second', got '%s'", val)
	}

	stack.Pop()
	val, _ = stack.Top()
	if val != "first" {
		t.Errorf("expected 'first', got '%s'", val)
	}
}

func TestStackTextSerialization(t *testing.T) {
	stack := NewStack()
	stack.Push("alpha")
	stack.Push("beta")
	stack.Push("gamma")

	var buf bytes.Buffer
	if err := stack.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	stack2 := NewStack()
	if err := stack2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	val, _ := stack2.Top()
	if val != "gamma" {
		t.Errorf("expected 'gamma', got '%s'", val)
	}
}

func TestStackTextSerializationEmpty(t *testing.T) {
	stack := NewStack()

	var buf bytes.Buffer
	if err := stack.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	stack2 := NewStack()
	if err := stack2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if stack2.Size() != 0 {
		t.Errorf("expected size 0, got %d", stack2.Size())
	}
}

func TestStackIsEmpty(t *testing.T) {
	stack := NewStack()
	if !stack.IsEmpty() {
		t.Error("expected empty stack")
	}

	stack.Push("item")
	if stack.IsEmpty() {
		t.Error("expected non-empty stack")
	}
}

func TestStackPopEmpty(t *testing.T) {
	stack := NewStack()
	if err := stack.Pop(); err != ErrEmpty {
		t.Error("expected ErrEmpty on pop from empty stack")
	}
}

func TestStackTopEmpty(t *testing.T) {
	stack := NewStack()
	if _, err := stack.Top(); err != ErrEmpty {
		t.Error("expected ErrEmpty on top of empty stack")
	}
}

func BenchmarkStackPush(b *testing.B) {
	stack := NewStack()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		stack.Push("item")
	}
}

func TestStackSize(t *testing.T) {
	stack := NewStack()
	if stack.Size() != 0 {
		t.Error("expected size 0")
	}

	stack.Push("a")
	if stack.Size() != 1 {
		t.Error("expected size 1")
	}

	stack.Push("b")
	if stack.Size() != 2 {
		t.Error("expected size 2")
	}
}

func TestStackMultiplePushPop(t *testing.T) {
	stack := NewStack()
	stack.Push("a")
	stack.Push("b")
	stack.Push("c")

	stack.Pop()
	val, _ := stack.Top()
	if val != "b" {
		t.Errorf("expected 'b', got '%s'", val)
	}

	stack.Pop()
	val, _ = stack.Top()
	if val != "a" {
		t.Errorf("expected 'a', got '%s'", val)
	}
}

func TestStackLargeTextSerialization(t *testing.T) {
	stack := NewStack()
	for i := 0; i < 100; i++ {
		stack.Push(fmt.Sprintf("item%d", i))
	}

	var buf bytes.Buffer
	if err := stack.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	stack2 := NewStack()
	if err := stack2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if stack2.Size() != 100 {
		t.Errorf("expected size 100, got %d", stack2.Size())
	}
}

// ========== QUEUE TESTS ==========

func TestQueuePushPop(t *testing.T) {
	queue := NewQueue()
	queue.Push("first")
	queue.Push("second")

	val, _ := queue.Front()
	if val != "first" {
		t.Errorf("expected 'first', got '%s'", val)
	}

	queue.Pop()
	val, _ = queue.Front()
	if val != "second" {
		t.Errorf("expected 'second', got '%s'", val)
	}
}

func TestQueueTextSerialization(t *testing.T) {
	queue := NewQueue()
	queue.Push("one")
	queue.Push("two")
	queue.Push("three")

	var buf bytes.Buffer
	if err := queue.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	queue2 := NewQueue()
	if err := queue2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	val, _ := queue2.Front()
	if val != "one" {
		t.Errorf("expected 'one', got '%s'", val)
	}
}

func TestQueueTextSerializationEmpty(t *testing.T) {
	queue := NewQueue()

	var buf bytes.Buffer
	if err := queue.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	queue2 := NewQueue()
	if err := queue2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if queue2.Size() != 0 {
		t.Errorf("expected size 0, got %d", queue2.Size())
	}
}

func TestQueueIsEmpty(t *testing.T) {
	queue := NewQueue()
	if !queue.IsEmpty() {
		t.Error("expected empty queue")
	}

	queue.Push("item")
	if queue.IsEmpty() {
		t.Error("expected non-empty queue")
	}
}

func TestQueuePopEmpty(t *testing.T) {
	queue := NewQueue()
	if err := queue.Pop(); err != ErrEmpty {
		t.Error("expected ErrEmpty on pop from empty queue")
	}
}

func TestQueueFrontEmpty(t *testing.T) {
	queue := NewQueue()
	if _, err := queue.Front(); err != ErrEmpty {
		t.Error("expected ErrEmpty on front of empty queue")
	}
}

func BenchmarkQueuePush(b *testing.B) {
	queue := NewQueue()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		queue.Push("item")
	}
}

func TestQueueSize(t *testing.T) {
	queue := NewQueue()
	if queue.Size() != 0 {
		t.Error("expected size 0")
	}

	queue.Push("a")
	if queue.Size() != 1 {
		t.Error("expected size 1")
	}

	queue.Push("b")
	if queue.Size() != 2 {
		t.Error("expected size 2")
	}
}

func TestQueueMultiplePushPop(t *testing.T) {
	queue := NewQueue()
	queue.Push("a")
	queue.Push("b")
	queue.Push("c")

	queue.Pop()
	val, _ := queue.Front()
	if val != "b" {
		t.Errorf("expected 'b', got '%s'", val)
	}

	queue.Pop()
	val, _ = queue.Front()
	if val != "c" {
		t.Errorf("expected 'c', got '%s'", val)
	}
}

func TestQueueLargeTextSerialization(t *testing.T) {
	queue := NewQueue()
	for i := 0; i < 100; i++ {
		queue.Push(fmt.Sprintf("item%d", i))
	}

	var buf bytes.Buffer
	if err := queue.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	queue2 := NewQueue()
	if err := queue2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if queue2.Size() != 100 {
		t.Errorf("expected size 100, got %d", queue2.Size())
	}
}

// ========== HASH TABLE TESTS ==========

func TestHashTableAdd(t *testing.T) {
	ht := NewHashTable(8)
	ok := ht.Add("alice")
	if !ok {
		t.Error("expected Add to succeed")
	}

	ok = ht.Add("alice")
	if ok {
		t.Error("expected duplicate Add to fail")
	}
}

func TestHashTableFind(t *testing.T) {
	ht := NewHashTable(8)
	ht.Add("alice")

	if !ht.Find("alice") {
		t.Error("expected to find 'alice'")
	}

	if ht.Find("bob") {
		t.Error("expected not to find 'bob'")
	}
}

func TestHashTableBinarySerialization(t *testing.T) {
	ht := NewHashTable(8)
	ht.Add("alice")
	ht.Add("bob")
	ht.Add("charlie")

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if ht2.GetSize() != 3 {
		t.Errorf("expected size 3, got %d", ht2.GetSize())
	}

	if !ht2.Find("alice") || !ht2.Find("bob") || !ht2.Find("charlie") {
		t.Error("expected to find all elements")
	}
}

func TestHashTableBinarySerializationEmpty(t *testing.T) {
	ht := NewHashTable(8)

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if ht2.GetSize() != 0 {
		t.Errorf("expected size 0, got %d", ht2.GetSize())
	}
}

func TestHashTableRemove(t *testing.T) {
	ht := NewHashTable(8)
	ht.Add("alice")
	ht.Add("bob")

	ok := ht.Remove("alice")
	if !ok {
		t.Error("expected Remove to succeed")
	}

	if ht.Find("alice") {
		t.Error("expected not to find 'alice' after removal")
	}

	if ht.GetSize() != 1 {
		t.Errorf("expected size 1, got %d", ht.GetSize())
	}
}

func TestHashTableRemoveNotFound(t *testing.T) {
	ht := NewHashTable(8)
	ht.Add("alice")

	ok := ht.Remove("nonexistent")
	if ok {
		t.Error("expected Remove to fail for non-existent element")
	}
}

func TestHashTableGetCapacity(t *testing.T) {
	ht := NewHashTable(16)
	if ht.GetCapacity() != 16 {
		t.Errorf("expected capacity 16, got %d", ht.GetCapacity())
	}
}

func TestHashTableNewWithZeroCapacity(t *testing.T) {
	ht := NewHashTable(0)
	if ht.GetCapacity() != 8 {
		t.Errorf("expected default capacity 8, got %d", ht.GetCapacity())
	}
}

func TestHashTableNewWithNegativeCapacity(t *testing.T) {
	ht := NewHashTable(-5)
	if ht.GetCapacity() != 8 {
		t.Errorf("expected default capacity 8, got %d", ht.GetCapacity())
	}
}

func TestHashTableSerializeDeserializeExact(t *testing.T) {
	ht := NewHashTable(8)
	ht.Add("key1")
	ht.Add("key2")
	ht.Add("key3")

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if ht2.GetSize() != 3 {
		t.Errorf("expected size 3, got %d", ht2.GetSize())
	}

	if !ht2.Find("key1") || !ht2.Find("key2") || !ht2.Find("key3") {
		t.Error("expected to find all keys")
	}
}

func TestHashTableLargeSerializeDeserialize(t *testing.T) {
	ht := NewHashTable(8)
	for i := 0; i < 200; i++ {
		ht.Add(fmt.Sprintf("key%d", i))
	}

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if ht2.GetSize() != 200 {
		t.Errorf("expected size 200, got %d", ht2.GetSize())
	}
}

func TestHashTableSerializeEmptyValidation(t *testing.T) {
	ht := NewHashTable(8)

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if ht2.GetSize() != 0 {
		t.Errorf("expected size 0, got %d", ht2.GetSize())
	}
}

func TestHashTableSerializeOneElement(t *testing.T) {
	ht := NewHashTable(8)
	ht.Add("single")

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if !ht2.Find("single") {
		t.Error("expected to find 'single'")
	}
}

func TestHashTableSerializeVariousKeys(t *testing.T) {
	ht := NewHashTable(8)
	keys := []string{"", "a", "abc", "x y z", "123"}
	for _, k := range keys {
		ht.Add(k)
	}

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	for _, k := range keys {
		if !ht2.Find(k) {
			t.Errorf("expected to find '%s'", k)
		}
	}
}

func TestHashTableSerializeTwoElement(t *testing.T) {
	ht := NewHashTable(8)
	ht.Add("first")
	ht.Add("second")

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if !ht2.Find("first") || !ht2.Find("second") {
		t.Error("expected to find both elements")
	}
}

func TestHashTableSerializeThreeElement(t *testing.T) {
	ht := NewHashTable(8)
	ht.Add("one")
	ht.Add("two")
	ht.Add("three")

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if ht2.GetSize() != 3 {
		t.Errorf("expected size 3, got %d", ht2.GetSize())
	}
}

func TestHashTableSerializeMany(t *testing.T) {
	ht := NewHashTable(8)
	for i := 0; i < 50; i++ {
		ht.Add(fmt.Sprintf("item%d", i))
	}

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if ht2.GetSize() != 50 {
		t.Errorf("expected size 50, got %d", ht2.GetSize())
	}
}

func TestHashTableHashFunction(t *testing.T) {
	ht := NewHashTable(16)
	// Add values and verify they're found (indirectly tests Hash function)
	for i := 0; i < 100; i++ {
		ht.Add(fmt.Sprintf("key%d", i))
	}

	// All should be found (Hash function was used internally)
	for i := 0; i < 100; i++ {
		if !ht.Find(fmt.Sprintf("key%d", i)) {
			t.Errorf("expected to find key%d", i)
		}
	}
}

func TestHashTableHashFunctionDirect(t *testing.T) {
	ht := NewHashTable(16)

	// Test that Hash function returns valid results within capacity
	keys := []string{"test", "hello", "world", "key123", "xyz", ""}
	for _, key := range keys {
		hash := ht.Hash(key)
		if hash >= uint64(ht.GetCapacity()) {
			t.Errorf("Hash(%q) = %d, expected < %d", key, hash, ht.GetCapacity())
		}
	}

	// Test that Hash function is deterministic
	key := "consistent_key"
	hash1 := ht.Hash(key)
	hash2 := ht.Hash(key)
	if hash1 != hash2 {
		t.Errorf("Hash(%q) not deterministic: %d != %d", key, hash1, hash2)
	}

	// Test with different capacities
	for capacity := 8; capacity <= 128; capacity *= 2 {
		ht2 := NewHashTable(capacity)
		testKeys := []string{"a", "bb", "ccc", "dddd"}
		for _, k := range testKeys {
			hash := ht2.Hash(k)
			if hash >= uint64(ht2.GetCapacity()) {
				t.Errorf("Hash with capacity %d: Hash(%q) = %d, expected < %d", capacity, k, hash, ht2.GetCapacity())
			}
		}
	}
}

func TestHashTableSerializeWithManyKeys(t *testing.T) {
	ht := NewHashTable(8)
	// Add many keys to ensure loop iterations are counted
	for i := 0; i < 75; i++ {
		ht.Add(fmt.Sprintf("key%d", i))
	}

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if ht2.GetSize() != 75 {
		t.Errorf("expected size 75, got %d", ht2.GetSize())
	}

	// Verify all keys are present
	for i := 0; i < 75; i++ {
		if !ht2.Find(fmt.Sprintf("key%d", i)) {
			t.Errorf("expected to find key%d after deserialization", i)
		}
	}
}

func TestHashTableSerializeComplexKeys(t *testing.T) {
	ht := NewHashTable(8)
	complexKeys := []string{
		"",
		"a",
		"hello world",
		"123456789",
		"!@#$%^&*()",
		"newline\ntest",
		"tab\t test",
	}

	for _, k := range complexKeys {
		ht.Add(k)
	}

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	for _, k := range complexKeys {
		if !ht2.Find(k) {
			t.Errorf("expected to find '%s' after deserialization", k)
		}
	}
}

func BenchmarkHashTableAdd(b *testing.B) {
	ht := NewHashTable(8)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht.Add("item")
	}
}

func BenchmarkHashTableFind(b *testing.B) {
	ht := NewHashTable(8)
	for i := 0; i < 10000; i++ {
		ht.Add("item")
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht.Find("item")
	}
}

func TestHashTableDuplicateAdd(t *testing.T) {
	ht := NewHashTable(8)
	ok1 := ht.Add("alice")
	ok2 := ht.Add("alice")

	if !ok1 {
		t.Error("first add should succeed")
	}
	if ok2 {
		t.Error("duplicate add should fail")
	}
}

func TestHashTableMultipleRemove(t *testing.T) {
	ht := NewHashTable(8)
	ht.Add("a")
	ht.Add("b")
	ht.Add("c")

	ht.Remove("a")
	ht.Remove("c")

	if ht.GetSize() != 1 {
		t.Errorf("expected size 1, got %d", ht.GetSize())
	}

	if !ht.Find("b") {
		t.Error("expected to find 'b'")
	}
}

func TestHashTableRehashing(t *testing.T) {
	ht := NewHashTable(2)
	ht.Add("a")
	ht.Add("b")
	ht.Add("c")
	ht.Add("d")
	ht.Add("e")

	// After adding many elements, capacity should have increased
	if ht.GetCapacity() <= 2 {
		t.Error("expected capacity to increase after rehashing")
	}

	// All elements should still be findable
	if !ht.Find("a") || !ht.Find("c") || !ht.Find("e") {
		t.Error("expected to find elements after rehashing")
	}
}

func TestHashTableLargeBinarySerialization(t *testing.T) {
	ht := NewHashTable(8)
	for i := 0; i < 100; i++ {
		ht.Add(fmt.Sprintf("item%d", i))
	}

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if ht2.GetSize() != 100 {
		t.Errorf("expected size 100, got %d", ht2.GetSize())
	}
}

// ========== AVL TREE TESTS ==========

func TestAVLInsertAndSearch(t *testing.T) {
	avl := NewAVLTree()
	avl.Insert(50)
	avl.Insert(30)
	avl.Insert(70)

	if avl.Search(50) == nil {
		t.Error("expected to find 50")
	}
	if avl.Search(30) == nil {
		t.Error("expected to find 30")
	}
	if avl.Search(999) != nil {
		t.Error("expected not to find 999")
	}
}

func TestAVLBinarySerialization(t *testing.T) {
	avl := NewAVLTree()
	avl.Insert(50)
	avl.Insert(30)
	avl.Insert(70)
	avl.Insert(20)
	avl.Insert(40)

	var buf bytes.Buffer
	if err := avl.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	avl2 := NewAVLTree()
	if err := avl2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if avl2.Search(50) == nil {
		t.Error("expected to find 50")
	}
	if avl2.Search(30) == nil {
		t.Error("expected to find 30")
	}
}

func TestAVLBinarySerializationEmpty(t *testing.T) {
	avl := NewAVLTree()

	var buf bytes.Buffer
	if err := avl.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	avl2 := NewAVLTree()
	if err := avl2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if avl2.Search(999) != nil {
		t.Error("expected not to find 999 in empty tree")
	}
}

func TestAVLInsertDuplicates(t *testing.T) {
	avl := NewAVLTree()
	avl.Insert(50)
	avl.Insert(50)
	avl.Insert(50)

	node := avl.Search(50)
	if node == nil {
		t.Error("expected to find 50")
	}
}

func TestAVLLargeInsertSequence(t *testing.T) {
	avl := NewAVLTree()
	for i := 0; i < 100; i++ {
		avl.Insert(i)
	}

	for i := 0; i < 100; i++ {
		if avl.Search(i) == nil {
			t.Errorf("expected to find %d", i)
		}
	}
}

func TestAVLSearchNotFound(t *testing.T) {
	avl := NewAVLTree()
	avl.Insert(50)
	avl.Insert(30)
	avl.Insert(70)

	if avl.Search(999) != nil {
		t.Error("expected not to find 999")
	}
}

func BenchmarkAVLInsert(b *testing.B) {
	avl := NewAVLTree()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		avl.Insert(i)
	}
}

func BenchmarkAVLSearch(b *testing.B) {
	avl := NewAVLTree()
	for i := 0; i < 10000; i++ {
		avl.Insert(i)
	}
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		avl.Search(5000)
	}
}

func TestAVLInsertMultiple(t *testing.T) {
	avl := NewAVLTree()
	values := []int{50, 25, 75, 10, 30, 60, 80}
	for _, v := range values {
		avl.Insert(v)
	}

	for _, v := range values {
		if avl.Search(v) == nil {
			t.Errorf("expected to find %d", v)
		}
	}
}

func TestAVLSearchEmptyTree(t *testing.T) {
	avl := NewAVLTree()
	if avl.Search(50) != nil {
		t.Error("expected not to find anything in empty tree")
	}
}

func TestAVLInsertUnbalancedSequence(t *testing.T) {
	avl := NewAVLTree()
	// Insert in descending order to test rotation
	for i := 100; i > 0; i-- {
		avl.Insert(i)
	}

	// All should be findable
	for i := 1; i <= 100; i++ {
		if avl.Search(i) == nil {
			t.Errorf("expected to find %d", i)
		}
	}
}

func TestAVLLargeBinarySerialization(t *testing.T) {
	avl := NewAVLTree()
	for i := 0; i < 100; i++ {
		avl.Insert(i)
	}

	var buf bytes.Buffer
	if err := avl.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	avl2 := NewAVLTree()
	if err := avl2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	// Check a few values to ensure proper reconstruction
	for i := 0; i < 100; i += 10 {
		if avl2.Search(i) == nil {
			t.Errorf("expected to find %d after deserialization", i)
		}
	}
}

// ========== ADDITIONAL EDGE CASE TESTS ==========

func TestListSerializationWithSpecialChars(t *testing.T) {
	list := NewList()
	list.PushBack("hello\nworld")
	list.PushBack("tab\there")

	var buf bytes.Buffer
	if err := list.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	list2 := NewList()
	if err := list2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if list2.GetSize() != 2 {
		t.Errorf("expected size 2, got %d", list2.GetSize())
	}
}

func TestHashTableWithRehashingAndFind(t *testing.T) {
	ht := NewHashTable(2)
	for i := 0; i < 50; i++ {
		ht.Add(fmt.Sprintf("key%d", i))
	}

	if !ht.Find("key25") {
		t.Error("expected to find 'key25' after rehashing")
	}

	if ht.Find("notexist") {
		t.Error("expected not to find 'notexist'")
	}
}

func TestListSerializeDeserializeLarge(t *testing.T) {
	list := NewList()
	for i := 0; i < 200; i++ {
		list.PushBack(fmt.Sprintf("node%d", i))
	}

	var buf bytes.Buffer
	if err := list.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	list2 := NewList()
	if err := list2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if list2.GetSize() != 200 {
		t.Errorf("expected size 200, got %d", list2.GetSize())
	}
}

func TestStackAllOperations(t *testing.T) {
	stack := NewStack()

	// Test operations on empty stack
	for i := 0; i < 50; i++ {
		stack.Push(fmt.Sprintf("val%d", i))
	}

	// Verify top
	if val, _ := stack.Top(); val != "val49" {
		t.Errorf("expected 'val49', got '%s'", val)
	}

	// Pop and check new top
	stack.Pop()
	if val, _ := stack.Top(); val != "val48" {
		t.Errorf("expected 'val48', got '%s'", val)
	}
}

func TestQueueAllOperations(t *testing.T) {
	queue := NewQueue()

	for i := 0; i < 50; i++ {
		queue.Push(fmt.Sprintf("val%d", i))
	}

	// Verify front
	if val, _ := queue.Front(); val != "val0" {
		t.Errorf("expected 'val0', got '%s'", val)
	}

	// Pop and check new front
	queue.Pop()
	if val, _ := queue.Front(); val != "val1" {
		t.Errorf("expected 'val1', got '%s'", val)
	}
}

func TestAVLComplexTree(t *testing.T) {
	avl := NewAVLTree()
	values := []int{50, 25, 75, 12, 37, 62, 87, 6, 18, 31, 43, 56, 68, 81, 93}

	for _, v := range values {
		avl.Insert(v)
	}

	for _, v := range values {
		if avl.Search(v) == nil {
			t.Errorf("expected to find %d", v)
		}
	}

	if avl.Search(999) != nil {
		t.Error("expected not to find 999")
	}
}

func TestListSingleElement(t *testing.T) {
	list := NewList()
	list.PushBack("only")

	if list.GetSize() != 1 {
		t.Error("expected size 1")
	}

	list.Del("only")
	if list.GetSize() != 0 {
		t.Error("expected size 0 after delete")
	}
}

func TestDoublyListSingleElement(t *testing.T) {
	dl := NewDoublyList()
	dl.PushBack("only")

	if dl.GetSize() != 1 {
		t.Error("expected size 1")
	}

	dl.Del("only")
	if dl.GetSize() != 0 {
		t.Error("expected size 0 after delete")
	}
}

func TestStackSingleElement(t *testing.T) {
	stack := NewStack()
	stack.Push("only")

	val, _ := stack.Top()
	if val != "only" {
		t.Errorf("expected 'only', got '%s'", val)
	}

	stack.Pop()
	if !stack.IsEmpty() {
		t.Error("expected empty after pop")
	}
}

func TestQueueSingleElement(t *testing.T) {
	queue := NewQueue()
	queue.Push("only")

	val, _ := queue.Front()
	if val != "only" {
		t.Errorf("expected 'only', got '%s'", val)
	}

	queue.Pop()
	if !queue.IsEmpty() {
		t.Error("expected empty after pop")
	}
}

func TestHashTableSingleElement(t *testing.T) {
	ht := NewHashTable(4)
	ht.Add("single")

	if !ht.Find("single") {
		t.Error("expected to find 'single'")
	}

	if ht.GetSize() != 1 {
		t.Errorf("expected size 1, got %d", ht.GetSize())
	}
}

func TestAVLSingleElement(t *testing.T) {
	avl := NewAVLTree()
	avl.Insert(42)

	if avl.Search(42) == nil {
		t.Error("expected to find 42")
	}

	if avl.Search(0) != nil {
		t.Error("expected not to find 0")
	}
}

func TestListFindMultipleMatches(t *testing.T) {
	list := NewList()
	list.PushBack("a")
	list.PushBack("b")
	list.PushBack("a")
	list.PushBack("c")

	node := list.Find("a")
	if node == nil || node.data != "a" {
		t.Error("expected to find first 'a'")
	}
}

func TestDoublyListFindMultipleMatches(t *testing.T) {
	dl := NewDoublyList()
	dl.PushBack("x")
	dl.PushBack("y")
	dl.PushBack("x")
	dl.PushBack("z")

	node := dl.Find("x")
	if node == nil || node.data != "x" {
		t.Error("expected to find first 'x'")
	}
}

func TestHashTableFindAfterRemove(t *testing.T) {
	ht := NewHashTable(4)
	ht.Add("a")
	ht.Add("b")
	ht.Add("c")

	ht.Remove("b")

	if ht.Find("b") {
		t.Error("expected not to find 'b' after removal")
	}

	if !ht.Find("a") {
		t.Error("expected to find 'a'")
	}
}

func TestAVLInsertRandom(t *testing.T) {
	avl := NewAVLTree()
	values := []int{15, 10, 20, 8, 12, 17, 25}

	for _, v := range values {
		avl.Insert(v)
	}

	for _, v := range values {
		if avl.Search(v) == nil {
			t.Errorf("expected to find %d", v)
		}
	}
}

func TestListPushFrontMultiple(t *testing.T) {
	list := NewList()
	list.PushFront("c")
	list.PushFront("b")
	list.PushFront("a")

	node := list.GetAt(0)
	if node.data != "a" {
		t.Errorf("expected 'a', got '%s'", node.data)
	}
}

func TestArrayInsertAtFirst(t *testing.T) {
	arr := NewArray()
	arr.PushBack("second")
	arr.InsertAt(0, "first")

	val, _ := arr.GetAt(0)
	if val != "first" {
		t.Errorf("expected 'first', got '%s'", val)
	}
}

func TestListSerializeWithSpaces(t *testing.T) {
	list := NewList()
	list.PushBack("hello world")
	list.PushBack("foo bar")

	var buf bytes.Buffer
	list.Serialize(&buf)

	list2 := NewList()
	list2.Deserialize(&buf)

	if list2.GetSize() != 2 {
		t.Error("expected size 2")
	}
}

func TestQueueLargeQueue(t *testing.T) {
	queue := NewQueue()
	for i := 0; i < 100; i++ {
		queue.Push(fmt.Sprintf("v%d", i))
	}

	for i := 0; i < 50; i++ {
		queue.Pop()
	}

	if queue.Size() != 50 {
		t.Errorf("expected size 50, got %d", queue.Size())
	}
}

func TestStackLargeStack(t *testing.T) {
	stack := NewStack()
	for i := 0; i < 100; i++ {
		stack.Push(fmt.Sprintf("v%d", i))
	}

	for i := 0; i < 50; i++ {
		stack.Pop()
	}

	if stack.Size() != 50 {
		t.Errorf("expected size 50, got %d", stack.Size())
	}
}

func TestListDeleteMiddle(t *testing.T) {
	list := NewList()
	list.PushBack("a")
	list.PushBack("b")
	list.PushBack("c")
	list.PushBack("d")

	list.Del("b")

	if list.GetSize() != 3 {
		t.Errorf("expected size 3, got %d", list.GetSize())
	}
}

func TestArrayDeleteFirst(t *testing.T) {
	arr := NewArray()
	arr.PushBack("first")
	arr.PushBack("second")
	arr.PushBack("third")

	arr.DelAt(0)

	val, _ := arr.GetAt(0)
	if val != "second" {
		t.Errorf("expected 'second', got '%s'", val)
	}
}

func TestArrayDeleteLast(t *testing.T) {
	arr := NewArray()
	arr.PushBack("first")
	arr.PushBack("second")
	arr.PushBack("third")

	arr.DelAt(2)

	if arr.GetSize() != 2 {
		t.Errorf("expected size 2, got %d", arr.GetSize())
	}
}

func TestHashTableMultipleAdds(t *testing.T) {
	ht := NewHashTable(4)
	ht.Add("one")
	ht.Add("two")
	ht.Add("three")
	ht.Add("four")
	ht.Add("five")

	if ht.GetSize() != 5 {
		t.Errorf("expected size 5, got %d", ht.GetSize())
	}
}

func TestAVLTwoElementsRL(t *testing.T) {
	avl := NewAVLTree()
	avl.Insert(10)
	avl.Insert(20)
	avl.Insert(15)

	if avl.Search(15) == nil {
		t.Error("expected to find 15")
	}
}

func TestAVLTwoElementsLR(t *testing.T) {
	avl := NewAVLTree()
	avl.Insert(20)
	avl.Insert(10)
	avl.Insert(15)

	if avl.Search(15) == nil {
		t.Error("expected to find 15")
	}
}

func TestAVLComplexBalancing(t *testing.T) {
	avl := NewAVLTree()
	// This sequence should trigger multiple rotations
	for i := 1; i <= 10; i++ {
		avl.Insert(i)
	}

	for i := 1; i <= 10; i++ {
		if avl.Search(i) == nil {
			t.Errorf("expected to find %d", i)
		}
	}
}

func TestDoublyListSerializeDeserializeValidation(t *testing.T) {
	dl := NewDoublyList()
	for i := 0; i < 25; i++ {
		dl.PushBack(fmt.Sprintf("item%d", i))
	}

	var buf bytes.Buffer
	dl.Serialize(&buf)

	dl2 := NewDoublyList()
	dl2.Deserialize(&buf)

	// Validate all elements
	for i := 0; i < 25; i++ {
		node := dl2.GetAt(i)
		if node == nil {
			t.Errorf("expected node at index %d", i)
		}
	}
}

func TestListSerializeDeserializeValidation(t *testing.T) {
	list := NewList()
	for i := 0; i < 25; i++ {
		list.PushBack(fmt.Sprintf("item%d", i))
	}

	var buf bytes.Buffer
	list.Serialize(&buf)

	list2 := NewList()
	list2.Deserialize(&buf)

	// Validate all elements
	for i := 0; i < 25; i++ {
		node := list2.GetAt(i)
		if node == nil {
			t.Errorf("expected node at index %d", i)
		}
	}
}

// ========== HASH TABLE COVERAGE BOOST TESTS ==========

func TestHashTableSerializeMany150(t *testing.T) {
	ht := NewHashTable(8)
	for i := 0; i < 150; i++ {
		ht.Add(fmt.Sprintf("element_%d_extended_key_name", i))
	}

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if ht2.GetSize() != 150 {
		t.Errorf("expected size 150, got %d", ht2.GetSize())
	}

	// Spot check some elements
	for i := 0; i < 150; i += 30 {
		if !ht2.Find(fmt.Sprintf("element_%d_extended_key_name", i)) {
			t.Errorf("expected to find element_%d after deserialization", i)
		}
	}
}

func TestHashTableSerializeEmpty200(t *testing.T) {
	ht := NewHashTable(8)
	// Add exactly 200 items to trigger multiple code paths
	for i := 0; i < 200; i++ {
		ht.Add(fmt.Sprintf("key_%010d", i))
	}

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	// Verify the buffer has data
	if buf.Len() == 0 {
		t.Error("expected non-empty buffer after serialization")
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if ht2.GetSize() != 200 {
		t.Errorf("expected size 200, got %d", ht2.GetSize())
	}
}

func TestHashTableSerializeSpecialCharacters(t *testing.T) {
	ht := NewHashTable(8)
	specialKeys := []string{
		"",
		" ",
		"\t",
		"\n",
		"!@#$%^&*()",
		"[]{}()<>",
		"multi\nline\nstring",
		"key_with_null\x00byte",
		"very_long_key_" + string(make([]byte, 100)),
	}

	for _, k := range specialKeys {
		ht.Add(k)
	}

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if ht2.GetSize() != len(specialKeys) {
		t.Errorf("expected size %d, got %d", len(specialKeys), ht2.GetSize())
	}

	for _, k := range specialKeys {
		if !ht2.Find(k) {
			t.Errorf("expected to find special key after deserialization")
		}
	}
}

func TestHashTableSerializeAddRemovePattern(t *testing.T) {
	ht := NewHashTable(8)
	// Add pattern: add/remove/add to test sparse table
	for i := 0; i < 100; i++ {
		ht.Add(fmt.Sprintf("item_%d", i))
	}

	// Remove every other item
	for i := 0; i < 100; i += 2 {
		ht.Remove(fmt.Sprintf("item_%d", i))
	}

	var buf bytes.Buffer
	if err := ht.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	ht2 := NewHashTable(8)
	if err := ht2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if ht2.GetSize() != 50 {
		t.Errorf("expected size 50, got %d", ht2.GetSize())
	}

	// Verify removed items are NOT found
	for i := 0; i < 100; i += 2 {
		if ht2.Find(fmt.Sprintf("item_%d", i)) {
			t.Errorf("expected NOT to find removed item_%d", i)
		}
	}

	// Verify kept items ARE found
	for i := 1; i < 100; i += 2 {
		if !ht2.Find(fmt.Sprintf("item_%d", i)) {
			t.Errorf("expected to find item_%d", i)
		}
	}
}

// ========== QUEUE AND STACK ADDITIONAL COVERAGE TESTS ==========

func TestQueueLargeItems(t *testing.T) {
	q := NewQueue()
	for i := 0; i < 100; i++ {
		q.Push(fmt.Sprintf("item_%d_with_longer_name", i))
	}

	var buf bytes.Buffer
	if err := q.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	q2 := NewQueue()
	if err := q2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if q2.Size() != 100 {
		t.Errorf("expected size 100, got %d", q2.Size())
	}

	for i := 0; i < 100; i++ {
		val, err := q2.Front()
		if err != nil || val != fmt.Sprintf("item_%d_with_longer_name", i) {
			t.Errorf("expected item_%d, got %s", i, val)
		}
		q2.Pop()
	}
}

func TestStackLargeItems(t *testing.T) {
	s := NewStack()
	for i := 0; i < 100; i++ {
		s.Push(fmt.Sprintf("item_%d_with_longer_name", i))
	}

	var buf bytes.Buffer
	if err := s.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	s2 := NewStack()
	if err := s2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if s2.Size() != 100 {
		t.Errorf("expected size 100, got %d", s2.Size())
	}

	for i := 99; i >= 0; i-- {
		val, err := s2.Top()
		if err != nil || val != fmt.Sprintf("item_%d_with_longer_name", i) {
			t.Errorf("expected item_%d, got %s", i, val)
		}
		s2.Pop()
	}
}

func TestQueueSerializeMultiline(t *testing.T) {
	q := NewQueue()
	q.Push("line1")
	q.Push("line2\nwith\nnewlines")
	q.Push("line3")

	var buf bytes.Buffer
	if err := q.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	q2 := NewQueue()
	if err := q2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if q2.Size() != 3 {
		t.Errorf("expected size 3, got %d", q2.Size())
	}
}

func TestStackSerializeMultiline(t *testing.T) {
	s := NewStack()
	s.Push("line1")
	s.Push("line2\nwith\nnewlines")
	s.Push("line3")

	var buf bytes.Buffer
	if err := s.Serialize(&buf); err != nil {
		t.Fatalf("serialize error: %v", err)
	}

	s2 := NewStack()
	if err := s2.Deserialize(&buf); err != nil {
		t.Fatalf("deserialize error: %v", err)
	}

	if s2.Size() != 3 {
		t.Errorf("expected size 3, got %d", s2.Size())
	}
}

func TestQueueDeserializeError(t *testing.T) {
	// Test error handling in deserialize
	badData := []byte("not a number\n")
	fr := NewFailingReader(badData, len(badData))

	q := NewQueue()
	if err := q.Deserialize(fr); err == nil {
		t.Error("expected error on invalid size")
	}
}

func TestStackDeserializeError(t *testing.T) {
	// Test error handling in deserialize
	badData := []byte("not a number\n")
	fr := NewFailingReader(badData, len(badData))

	s := NewStack()
	if err := s.Deserialize(fr); err == nil {
		t.Error("expected error on invalid size")
	}
}

// ========== PERFORMANCE TESTS WITH TIMING ==========

func TestPerformanceArray(t *testing.T) {
	start := time.Now()
	arr := NewArray()
	for i := 0; i < 10000; i++ {
		arr.PushBack("item")
	}
	elapsed := time.Since(start)
	t.Logf("Array push_back x10000: %v", elapsed)
}

func TestPerformanceHashTable(t *testing.T) {
	start := time.Now()
	ht := NewHashTable(8)
	for i := 0; i < 10000; i++ {
		ht.Add("item")
	}
	elapsed := time.Since(start)
	t.Logf("HashTable add x10000: %v", elapsed)
}

func TestPerformanceAVLTree(t *testing.T) {
	start := time.Now()
	avl := NewAVLTree()
	for i := 0; i < 10000; i++ {
		avl.Insert(i)
	}
	elapsed := time.Since(start)
	t.Logf("AVL insert x10000: %v", elapsed)
}
