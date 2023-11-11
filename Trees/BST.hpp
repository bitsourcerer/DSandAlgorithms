#pragma once

#include "BinaryTree.hpp"

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