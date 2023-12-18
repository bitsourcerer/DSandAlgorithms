#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

#include <utility>
#include <memory>
#include <iostream>
#include <functional>

template <typename> class LinkedList;
template <typename> class ListIterator;

template <typename T> using NodePtr = typename LinkedList<T>::Node*;

template <typename T>
bool operator==(const ListIterator<T> &rhs, const ListIterator<T> &lhs);

template <typename T> using SearchResult = std::pair<bool, NodePtr<T>>;

template <typename T>
struct SLLNode
{
    using element_type = typename std::remove_reference<T>::type;
    friend class LinkedList<T>;
    //typename std::remove_reference<T>::type data;
    element_type data;
    mutable SLLNode *next;

    SLLNode() : data(), next(nullptr) { }
    SLLNode(const T &val) : data(val), next(nullptr) { }

    operator T() { return data; }
    SLLNode& operator=(const element_type &val) { data = val; }
};

template <typename ListType>
struct ListIterator
{
    using Node = typename LinkedList<ListType>::Node;
    friend class LinkedList<ListType>;
    friend bool operator==<ListType>(const ListIterator<ListType>&, const ListIterator<ListType>&);
public:
    ListIterator() = delete;
    ListIterator(Node &node) : ptr(&node) { }

    Node& operator*() { return *ptr; }
    Node* operator->() { return ptr; }

    ListIterator& operator++() {
        if(ptr)
            ptr = ptr->next;
            //ptr = std::make_shared<Node>(ptr->next);
        return *this;
    }
    ListIterator operator++(int) {
        ListIterator ret = *this;
        ++*this;
        return ret;
    }

private:
    Node *ptr;  //abandoned shared_ptr cuz it doesnt makes sense here.
};

template <typename T>
class LinkedList
{
    typedef ListIterator<T> Iter;
    friend class ListIterator<T>;
public:
    using Node = SLLNode<T>;
    LinkedList() = default;
    LinkedList(const LinkedList&) = delete;

    //LinkedList(const T &initial);

    void insert(Node *const pos, const T &val);
    Node* insert(const T &val);
    //void insert(SLLNode *const pos, SLLNode *node);
    void erase(Node *node);

    void assign(Node *const node, const T &val) const;
    Node& find(T val) const;

//    void push_front(const Node &node);
//    void push_back(const Node &node);

    Iter begin() const;
    Iter end() const;

    void traverse() const;

private:
    //Node *beg = NULL;
    Node *root = nullptr;
    SearchResult<T> nodeExists(const Node *const sought) const;
};

//template <typename T>
//LinkedList<T>::LinkedList(const T &initial) : root(nullptr) {  }

template <typename T>
void LinkedList<T>::insert(Node *const pos, const T &val)
{
    Node *node = root;
    while(node != pos && node)
        node = node->next;

    if(!node)
    {
        std::clog << "Invalid position!\n";
        return;
    }
    auto temp = node->next;
    node->next = new Node(val);
    node->next->next = temp;
}

template <typename T>
auto LinkedList<T>::insert(const T &val) -> typename LinkedList<T>::Node*
{
    if(root == nullptr)
    {
        root = new Node(val);
        return root;
    }
    Node *traversalNode = this->root;
    while(traversalNode->next != nullptr)
        traversalNode = traversalNode->next;

    return traversalNode->next = new Node(val);

    /*
        Node *traversalNode = this->root;
    while(traversalNode != nullptr)
        traversalNode = traversalNode->next;
    traversalNode = new Node(val);
    */
}

template <typename T>
void LinkedList<T>::erase(Node *node)
{
    Node *previousToTarget = root;
    Node *nextToTarget = node->next;

    while(previousToTarget->next != node or previousToTarget->next != nullptr)
        previousToTarget = previousToTarget->next;

    delete previousToTarget->next;
    previousToTarget->next = nextToTarget;
}

template <typename T>
void LinkedList<T>::assign(Node *const node, const T &val) const
{
    auto [exists, handle] = nodeExists(node);
    if(exists)
        handle->data = val;
    else
        std::cerr << "Given node not found in list!" << std::endl;
}

template <typename T>
SLLNode<T>& LinkedList<T>::find(T val) const
{
    Node *found = root;
    while(found)
    {
        if(val == found->data) break;
        found = found->next;
    }
    return *found;
}

template <typename T>
auto LinkedList<T>::begin() const -> typename LinkedList<T>::Iter
{
    return ListIterator<T>(*root);
}

template <typename T>
auto LinkedList<T>::end() const -> typename LinkedList<T>::Iter
{
    Iter ret = begin();
    while(ret->next != nullptr) ++ret;

    //ret.ptr += 1; // to go past the end, 1 will add sizeof(Iter) to ret;
    return ret;
}


template <typename T>
void LinkedList<T>::traverse() const
{
    if(!root) return;
    Node *node = root;
    do
    {
        std::clog << node->data << '\t';
        node = node->next;
    } while(node);
}


template <typename T>
SearchResult<T> LinkedList<T>::nodeExists(const Node *const sought) const
{
    Node *traversalNode = root;
    while(traversalNode)
    {
        if(traversalNode == sought)
            return std::make_pair(true, traversalNode);
        traversalNode = traversalNode->next;
    }

    return {false, nullptr};
}

template <typename T>
bool operator==(const ListIterator<T> &rhs, const ListIterator<T> &lhs)
{
    return std::equal_to<SLLNode<T>*>()(rhs.ptr, lhs.ptr);
    //return std::equal_to<std::shared_ptr<SLLNode<T>>>()(rhs.ptr, lhs.ptr);
    //return rhs.ptr.get() == lhs.ptr.get();
}

template <typename T>
bool operator!=(const ListIterator<T> &rhs, const ListIterator<T> &lhs)
{
    return !(rhs == lhs);
}

#endif // LINKEDLIST_H_INCLUDED
