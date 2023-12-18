#pragma once

#define ABSTRACT 0

using std::max;

enum class TraversalFashion : TraversalEnumeratorType
{
	In_Order,
	Pre_Order,
	Post_Order
};

nullptr_t null = nullptr;

template <typename T>
class AbstractBinaryTree
{
	using Type = std::remove_reference_t<T>;

public:
	class Node;
	
	AbstractBinaryTree();
	AbstractBinaryTree(const std::initializer_list<Type> &lst);
	virtual ~AbstractBinaryTree();

	virtual void insert(const Type&) = ABSTRACT;
	virtual Node find(const Type &) const = ABSTRACT;
	virtual void remove() = ABSTRACT;

	virtual unsigned size() const;
	virtual unsigned height() const;

protected:
	struct Node
	{
		Node();
		Node(Type value, Node *const lchild = nullptr, Node *const rchild = nullptr);
		~Node();

		Type data;
		Node *left, *right;

		Node& operator = (const Type &val);
	};

	Node *root;
	mutable uint32_t counter;
};

template <typename T>
AbstractBinaryTree<T>::Node::Node() : data(), left(nullptr), right(nullptr) { }

template <typename T>
AbstractBinaryTree<T>::Node::Node(Type value, Node *const lchild, Node *const rchild)
 : data(), left(lchild), right(rchild)
 {
 	;
 }

 template <typename T>
AbstractBinaryTree<T>::Node::~Node()
{
	--count;
}

 template <typename T>
 typename AbstractBinaryTree<T>::Node& AbstractBinaryTree<T>::Node::operator = (const Type &val)
 {
 	data = val;
 }

template <typename T>
AbstractBinaryTree<T>::AbstractBinaryTree() : root(nullptr), count(0) { ; }

template <typename T>
AbstractBinaryTree<T>::AbstractBinaryTree(const std::initializer_list<Type> &lst) : /* root(new Node(lst[0])), */ count(lst.size())
{
	for(const Type& elem : lst) insert(lst);
}

template <typename T>
AbstractBinaryTree<T>::~AbstractBinaryTree()
{
	if(!root) return;
	delete root;
}

template <typename T>
inline unsigned AbstractBinaryTree<T>::size() const
{
	return count;
}

template <typename T>
unsigned AbstractBinaryTree<T>::height(Node *const node = root) const
{
	if (!node) return 0;
 
    unsigned lh = height(node->left);
    unsigned rh = height(node->right);

    return max(lh. rh) + 1;
}

template <typename T>
class BinarySearchTree : public AbstractBinaryTree<T>
{
	using Base = AbstractBinaryTree<T>;
	using Node = typename Base::Node;
	using Type = std::remove_reference_t<T>;

public:
	BinarySearchTree() = default;
	BinarySearchTree(const std::initializer_list<Type> &lst);
	~BinarySearchTree();

	void insert(const Type&) override;
	Node& put(Node* &const, const Type&);
	Node find(const Type &) const override;
	void remove() override;

	Type max() const;
	Type min() const;

private:
	//using Base::root;
};

template <typename T>
BinarySearchTree<T>::BinarySearchTree(const std::initializer_list<Type> &lst)
: AbstractBinaryTree(lst)
{
	;
}

template <typename T>
void BinarySearchTree<T>::insert(const Type &val)
{
	root = put(root, val);
}

template <typename T>
BinarySearchTree<T>::Node& BinarySearchTree<T>::put(Node *&const node, const Type &val)
{
	if(!node) return new Node(val);
	Node *candidate = node;

	if(val < candidate->data) insert(candidate->left, val);
	else if(val > candidate->data) insert(candidate->right, val);
	else candidate->data = val;

	return candidate;
}

template <typename T>
BinarySearchTree<T>::Node BinarySearchTree<T>::find(const Type &val) const
{
	Node *sought = root;

	while(sought)
	{
		if(sought->data > val) sought = sought->left;
		else if (sought->data < val) sought = sought->right;
		else break;
	}

	return sought;
}

/*
if(val < parent->data) insert(parent->left);
	else if(val > parent->data) insert(parent->right);
	else return parent;


template <typename T>
BinarySearchTree<T>::Node& BinarySearchTree<T>::put(Node *&const node, const Type &val)
{
	while(node) //(node && node->data != val)
	{
		if(val > node->data) node = node->right;
		else if(val < node->data) node = node->right;
		else break;
	}

	return node = new Node(val);
}
*/