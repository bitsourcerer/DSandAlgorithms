#pragma once

#include <initializer_list>
#include <optional>
#include <utility>

template <typename T>
class BinarySearchTree
{
	class BSTNode;
	using Type = std::remove_reference_t<T>;
	using Node = BSTNode;

public:
	BinarySearchTree() : root(nullptr) { };
	BinarySearchTree(const std::initializer_list<Type> &lst);
	~BinarySearchTree();

	void insert(const Type&);
	Node* insert(Node* const&, const Type&) const;
	std::optional<Node> find(const Type &) const;
	std::pair<bool, Node*> find(const Type&);
	void remove(const Type&);

	Node* findMin(Node *const&) const;
	Node* findMax(Node *const&) const;
	Node* removeMin(Node *const);
	Node *removeMax(Node *const);

	// Type max() const;
	// Type min() const;

private:
	Node *root;
	void destroy(Node *const&);

	struct BSTNode
	{
		BSTNode(const Type &value = Type()) : data(value), left(nullptr), right(nullptr) { };
		Type data;
		BSTNode *left, *right;
		operator Type() const { return data; }
	};
};

template <typename T>
BinarySearchTree<T>::BinarySearchTree(const std::initializer_list<Type> &lst)
{
	;
}

template <typename T>
BinarySearchTree<T>::~BinarySearchTree()
{
	destroy(root);
	// delete root; // to be defined
}

template <typename T>
void BinarySearchTree<T>::insert(const Type &val)
{
	root = insert(root, val);
}

template <typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::insert(Node *const &node, const Type &val) const
{
	if(!node) return new Node(val);
	Node *candidate = node;

	if(val < candidate->data) candidate->left = insert(candidate->left, val);
	else if(val > candidate->data) candidate->right = insert(candidate->right, val);
	else candidate->data = val;

	return candidate;
}

template <typename T>
std::optional<typename BinarySearchTree<T>::Node> BinarySearchTree<T>::find(const Type &val) const
{
	std::optional<Node> result;
	Node *sought = root;

	while(sought) //|| result.emplace(sought->data) && false
	{
		if(sought->data > val) sought = sought->left;
		else if (sought->data < val) sought = sought->right;
		else { result.emplace(sought->data); break; }
	}

	return result;
}

template <typename T>
std::pair<bool, typename BinarySearchTree<T>::Node*> BinarySearchTree<T>::find(const Type &val)
{
	Node *sought = root;
	while(sought)
	{
		if(sought->data > val) sought = sought->left;
		else if (sought->data < val) sought = sought->right;
		else return {true, sought};
	}
	return {false, nullptr};
}

template <typename T>
void BinarySearchTree<T>::remove(const Type &val)
{
	auto res = find(val);
	if(!res.first) return;
	Node *del = res.second;

	if(!del->left && !del->right) { delete del; return; }
	if(!del->left);
}

template <typename T>
void BinarySearchTree<T>::destroy(Node *const &node)
{
	if(!node) return;

	destroy(node->left);
	destroy(node->right);
	delete node;

	/*
		can also be done truly recursive way,
		like BSTNode class will have destructor
		which will delete both its left and right childs
		and Tree class's destructor will just delete root. 
	*/
}

template <typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::findMin(Node *const &node) const
{
	if(!node) return nullptr;
	Node* result;

	while(result->left)
		result = result->left;

	return result;
}

template <typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::findMax(Node *const &node) const
{
	if(!node) return nullptr;

	Node* result;
	while(result->right)
		result = result->right;

	return result;
}

template <typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::removeMin(Node *const node)
{
	if(!node) return nullptr;

	if(!node->left)
	{
		Node* temp = node->right;
		destroy(node);
		return temp;
	}	

	node->left = removeMin(node->left);
	return node;
}

template <typename T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::removeMax(Node *const node)
{
	if(!node) return nullptr;

	if(!node->right)
	{
		Node* temp = node->left;
		destroy(node);
		return temp;
	}

	node->right = removeMax(node->right);
	return node;
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