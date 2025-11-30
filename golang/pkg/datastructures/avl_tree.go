package datastructures

import (
	"encoding/binary"
	"io"
)

// AVLNode представляет узел AVL дерева
type AVLNode struct {
	Value  int
	Left   *AVLNode
	Right  *AVLNode
	Height int
}

// AVLTree - AVL сбалансированное дерево поиска
type AVLTree struct {
	Root *AVLNode
}

// NewAVLTree создаёт новое пустое AVL дерево
func NewAVLTree() *AVLTree {
	return &AVLTree{Root: nil}
}

// Insert вставляет значение в дерево
func (avl *AVLTree) Insert(value int) {
	avl.Root = avl.insert(avl.Root, value)
}

func (avl *AVLTree) insert(node *AVLNode, value int) *AVLNode {
	if node == nil {
		return &AVLNode{Value: value, Height: 1}
	}

	if value < node.Value {
		node.Left = avl.insert(node.Left, value)
	} else if value > node.Value {
		node.Right = avl.insert(node.Right, value)
	} else {
		return node // дубликат
	}

	node.Height = 1 + max(avl.getHeight(node.Left), avl.getHeight(node.Right))

	balance := avl.getBalance(node)

	// LL
	if balance > 1 && value < node.Left.Value {
		return avl.rotateRight(node)
	}
	// RR
	if balance < -1 && value > node.Right.Value {
		return avl.rotateLeft(node)
	}
	// LR
	if balance > 1 && value > node.Left.Value {
		node.Left = avl.rotateLeft(node.Left)
		return avl.rotateRight(node)
	}
	// RL
	if balance < -1 && value < node.Right.Value {
		node.Right = avl.rotateRight(node.Right)
		return avl.rotateLeft(node)
	}

	return node
}

// Search ищет значение в дереве
func (avl *AVLTree) Search(value int) *AVLNode {
	return avl.search(avl.Root, value)
}

func (avl *AVLTree) search(node *AVLNode, value int) *AVLNode {
	if node == nil {
		return nil
	}

	if value == node.Value {
		return node
	} else if value < node.Value {
		return avl.search(node.Left, value)
	} else {
		return avl.search(node.Right, value)
	}
}

// Helper functions

func (avl *AVLTree) getHeight(node *AVLNode) int {
	if node == nil {
		return 0
	}
	return node.Height
}

func (avl *AVLTree) getBalance(node *AVLNode) int {
	if node == nil {
		return 0
	}
	return avl.getHeight(node.Left) - avl.getHeight(node.Right)
}

func (avl *AVLTree) rotateLeft(z *AVLNode) *AVLNode {
	y := z.Right
	t2 := y.Left

	y.Left = z
	z.Right = t2

	z.Height = 1 + max(avl.getHeight(z.Left), avl.getHeight(z.Right))
	y.Height = 1 + max(avl.getHeight(y.Left), avl.getHeight(y.Right))

	return y
}

func (avl *AVLTree) rotateRight(y *AVLNode) *AVLNode {
	x := y.Left
	t2 := x.Right

	x.Right = y
	y.Left = t2

	y.Height = 1 + max(avl.getHeight(y.Left), avl.getHeight(y.Right))
	x.Height = 1 + max(avl.getHeight(x.Left), avl.getHeight(x.Right))

	return x
}

func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}

// Serialize сохраняет дерево в бинарном формате
func (avl *AVLTree) Serialize(w io.Writer) error {
	return avl.serializeNode(avl.Root, w)
}

func (avl *AVLTree) serializeNode(node *AVLNode, w io.Writer) error {
	if node == nil {
		marker := int32(-999999)
		return binary.Write(w, binary.LittleEndian, marker)
	}

	// Сохраняем значение и высоту
	if err := binary.Write(w, binary.LittleEndian, int32(node.Value)); err != nil {
		return err
	}
	if err := binary.Write(w, binary.LittleEndian, int32(node.Height)); err != nil {
		return err
	}

	// Рекурсивно сохраняем левое и правое поддеревья
	if err := avl.serializeNode(node.Left, w); err != nil {
		return err
	}
	if err := avl.serializeNode(node.Right, w); err != nil {
		return err
	}

	return nil
}

// Deserialize загружает дерево из бинарного формата
func (avl *AVLTree) Deserialize(r io.Reader) error {
	avl.Root = nil
	var err error
	avl.Root, err = avl.deserializeNode(r)
	return err
}

func (avl *AVLTree) deserializeNode(r io.Reader) (*AVLNode, error) {
	var value int32
	if err := binary.Read(r, binary.LittleEndian, &value); err != nil {
		return nil, err
	}

	if value == -999999 {
		return nil, nil
	}

	var height int32
	if err := binary.Read(r, binary.LittleEndian, &height); err != nil {
		return nil, err
	}

	node := &AVLNode{
		Value:  int(value),
		Height: int(height),
	}

	var err error
	node.Left, err = avl.deserializeNode(r)
	if err != nil {
		return nil, err
	}

	node.Right, err = avl.deserializeNode(r)
	if err != nil {
		return nil, err
	}

	return node, nil
}
