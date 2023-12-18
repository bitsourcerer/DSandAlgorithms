#include <memory>
#include <type_traits>
#include <optional>
#include <cstdint>
#include <execution>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <iomanip>

#include "Vector.hpp"

namespace util
{

template <class, typename = void>
struct can_output_to_stream : std::false_type {};

template <class T>
struct can_output_to_stream< T, std::void_t< decltype(std::declval<std::ostream&>() << std::declval<T>())> > : std::true_type {};

template <class T> inline constexpr bool is_ostream_valid_v = can_output_to_stream<T>::value;
// template <class T> struct is_ostream_valid_v
// {
//     enum { value = std::is_same_v<can_output_to_stream<T>, std::true_type> };
// };

}

namespace ds
{

template <typename TX, typename TY>
struct KVPair
{
    KVPair() = default;
    KVPair(const KVPair &cp) : key(cp.key), value(cp.value) { }
    KVPair(KVPair &&mv) : KVPair(std::move(mv.key), std::move(mv.value)) { }
    KVPair(const TX& k, const TY& v) noexcept : key(k), value(v) { }
    KVPair(TX &&k, TY &&v) noexcept : key(std::move(k)), value(std::move(v)) { }
    TX key;
    TY value;
};

template <typename TX, typename TY>
KVPair<TX, TY> createPair(TX &&first, TY &&second)
{
    using TypeX = std::remove_reference_t<TX>;
    using TypeY = std::remove_reference_t<TY>;
    // TX and TY are deduced to be either lvalue reference or rvalue reference.
    return KVPair(std::forward<TypeX>(first), std::forward<TypeY>(second));
}

template <typename K, typename V, class H = std::hash<K>>
class HashTable
{
    static_assert(std::is_same_v<K, std::remove_cv_t<K>> &&
    std::is_same_v<V, std::remove_cv_t<V>>, "Template type cannot be const or volatile!");

    class Iterator;
    using Key = typename std::remove_reference<K>::type;
    using Value = typename std::remove_reference<V>::type;
    using Node = KVPair<const Key, Value>;
    using Element = std::optional<Node>;
    using Memory = Vector<Element>;
    using Hasher = H;
    using Index = std::size_t;
    using Iter = Iterator;
    using CIter = const Iterator;
    // using Index = typename Memory::size_type;
    // using Memory = std::allocator<Pair>;

    static const Index cap = 107;
    static constexpr float maxload = 0.8;
    static constexpr float growthfactor = 1.61803398875; // golden ratio

public:
    // std compliant typedefines
    typedef Key key_type;
    typedef Value mapped_type;
    typedef Node element_type;
    typedef Memory allocator_type;
    typedef Index size_type;
    typedef Iter iterator;
    typedef CIter const_iterator;
    typedef Node& reference;
    typedef const Node& const_reference;

    HashTable(Index capacity = cap);
    ~HashTable();

    bool empty() const noexcept(noexcept(size()));
    Index size() const noexcept;
    Index capacity() const;
    double load() const;
    std::ostream& dump(std::ostream&, bool verbose = false) const; // verbosely dump table information

    Value& operator[](const Key&);
    bool insert(const Node&);
    bool remove(const Key&);
    void clear();
    // const Value& operator[](Key) const = delete;
    // Pair<bool, Iterator> find(const Key&) const;

    Iter begin();
    Iter end();

    CIter begin() const;
    CIter end() const;

private:
    mutable Memory elements;
    Index length;
    Hasher hasher; // can be const?
    // double maxload; // Soon to be moved to template param

    void rebuild(); // rebuild with greater capacity
    Index hash(const Key &key) const;
    static bool elementExists(const Element& optional) { return optional.has_value(); }

    class Iterator : std::forward_iterator_tag
    {
        using Distance = std::ptrdiff_t;
    public:
        Iterator() = delete;
        Iterator(Memory &mem, Index index) : data(mem), id(index) { }
        Iterator& operator+=(Index n) {
            advance(n);
            return *this;
        }
        friend Iterator operator+(const Iterator &lhs, Index n) {
            Iterator result = lhs;
            result += n;
            return result;
        }
        Iterator& operator++() {
            // for(Index i = offset(), c = data.size(); !(++ptr)->has_value() && i < c; ++i); //data.size() is same as table.capacity();
            ++id;
            findNext();
            return *this;
        }
        Iterator operator++(int) {
            Iterator result = *this;
            ++*this;
            return result;
        }
        friend bool operator==(const Iterator &lhs, const Iterator &rhs) {
            return lhs.id == rhs.id;
        }
        friend bool operator!=(const Iterator& lhs, const Iterator& rhs)
        {
            return !(lhs == rhs);
        }

        Distance offset() const {
            return id;
        }

        Node& operator*() { checkValid(); return data.at(id).value(); }
        const Node& operator*() const { checkValid(); return data[id].value(); }

        Node* operator->() { checkValid(); return std::addressof(getElement(id).value()); } //data[id].value()
        const Node* operator->() const { checkValid(); return std::addressof(getElement(id).value()); }

        explicit operator bool(){ return id != data.size() && data.at(id).has_value(); }

    private:
        Memory &data;
        Element *ptr = nullptr;
        Index id;

        void checkValid() const {
            if(!getElement(id).has_value()) throw std::invalid_argument("Dereference of invalid iterator!");
            // !data.at(id).has_value()
        }

        void advance(Distance n) {
            for(Index i = 0; i < n; ++i) { ++id; findNext();}
        }

        void findNext() {
            while(id < data.size() && !data[id].has_value()) ++id; // data.size() is equivalent of Map.capacity()
            // id += std::distance(std::begin(data) + id, std::find_if(std::execution::par, std::begin(data) + id + 1, std::end(data), elementExists));
        }

        auto getElement(Index idx) const -> Element& {
            try {
                return data.at(idx);
            } catch(...) {
                std::cerr << "Use of invalid iterator! @ " << __func__ << std::endl;
                std::terminate();
            }
        }
    };
};

template <typename K, typename V, class H>
HashTable<K, V, H>::HashTable(Index cap) : elements(cap), length(0)
{
}

template <typename K, typename V, class H>
HashTable<K, V, H>::~HashTable()
{
}

template <typename K, typename V, class H>
inline bool HashTable<K, V, H>::empty() const noexcept(noexcept(size()))
{
    return !size();
}

template <typename K, typename V, class H>
inline typename HashTable<K, V, H>::Index HashTable<K, V, H>::size() const noexcept
{
    // return std::count_if(std::execution::par, std::begin(elements), std::end(elements), HashTable::elementExists);
    return length;
}

template <typename K, typename V, class H>
inline typename HashTable<K, V, H>::Index HashTable<K, V, H>::capacity() const
{
    return elements.size();
}

template <typename K, typename V, class H>
inline typename HashTable<K, V, H>::Index HashTable<K, V, H>::hash(const Key &key) const
{
    return hasher(key) % capacity();
}

template <typename K, typename V, class H>
inline double HashTable<K, V, H>::load() const
{
    // Index sz = size(), cap = capacity();
    // return size() ? capacity() / size() : size();
    return static_cast<decltype(maxload)>(size()) / capacity();
}

template <typename K, typename V, typename H>
std::ostream& HashTable<K, V, H>::dump(std::ostream &os, bool verbose) const
{
    static bool warn = true;
    std::ostream::iostate old = os.rdstate();
    os.clear();

    os << "TABLE -> " << "Capacity: " << capacity() << "\tSize: " << size() << "\tLoad: " << load() << '\n';
    // os << std::boolalpha << util::is_ostream_valid_v<Key> << '\t' << util::is_ostream_valid_v<Value> << '\n';
    if(warn && (!util::is_ostream_valid_v<Key> || !util::is_ostream_valid_v<Value>)) { os << "cant output Types to stream!\n"; warn = false; }

    if constexpr(util::is_ostream_valid_v<Key> && util::is_ostream_valid_v<Value>)
    {
        if(empty()) goto ret;
        os << std::setw(45) << std::setfill('-') << std::left << "K" << std::right << "V\n" << std::internal << std::setfill(' ');

       for(const Node &n : *this)
       os << '|' << std::setw(20) << std::left << n.key
       << " || " << std::setw(20) << std::right << n.value
       << '|' << '\n';

       os << std::setw(47) << std::setfill('-') << '\0' << std::setfill(' ');
    }

    ret:
    os.setstate(old);
    return os;
}

template <typename K, typename V, class H>
typename HashTable<K, V, H>::Value& HashTable<K, V, H>::operator[](const Key &key)
{
    if(load() > HashTable::maxload) rebuild();
    Index index = hash(key);
    Element &current = elements.at(index);

    if(!elementExists(current)) { current.emplace(key, Value()); ++length; } //current = {key, Value()}; ds::createPair(key, Value());
    return current.value().value; // std::optional dependent! gotta change in future..
}

template <typename K, typename V, class H>
bool HashTable<K, V, H>::insert(const Node &pair)
{
    Index index = hash(pair.key);
    Element& current = elements.at(index);

    if(elementExists(current)) return false;
    if(load() > maxload) rebuild();
    // current = {pair.first, pair.second};
    current.emplace(pair.key, pair.value);
    // Node& node = current.value();
    ++length;
    return true;
}

template <typename K, typename V, class H>
bool HashTable<K, V, H>::remove(const Key &key)
{
    Element &current = elements.at(hash(key));
    if(!elementExists(current)) return false;

    current.reset();
    --length;
    return true;
}

template <typename K, typename V, class H>
void HashTable<K, V, H>::clear()
{
    // Index oldcap = capacity();
    // elements.clear();
    // elements.resize(oldcap);
    std::for_each(std::execution::par, std::begin(elements), std::end(elements), [](Element &e) -> void {
        if(HashTable::elementExists(e)) e.reset();
    });
    length = 0;
}

template <typename K, typename V, class H>
typename HashTable<K, V, H>::Iter HashTable<K, V, H>::begin()
{
    // Element &candidate;
    // for(Element& e : elements) if(e.has_value()) candidate = e;
    // return Iterator(candidate);
    // return Iter(elements, *std::find_if(std::execution::par, std::begin(elements), std::end(elements), elementExists));
    return Iter(elements, std::distance(std::begin(elements), std::find_if(std::begin(elements), std::end(elements), elementExists)));
}

template <typename K, typename V, class H>
typename HashTable<K, V, H>::Iter HashTable<K, V, H>::end()
{
    // Iter(this, *++(std::find_if(std::execution::par, std::rbegin(elements), std::rend(elements), elementExists).base())); //base() ?
    // return begin() += length;
    return Iter(elements, capacity());
}

template <typename K, typename V, class H>
typename HashTable<K, V, H>::CIter HashTable<K, V, H>::begin() const
{
    return CIter(elements, std::distance(std::begin(elements), std::find_if(std::begin(elements), std::end(elements), elementExists)));
}

template <typename K, typename V, class H>
typename HashTable<K, V, H>::CIter HashTable<K, V, H>::end() const
{
    return CIter(elements, capacity());
}

template <typename K, typename V, class H>
void HashTable<K, V, H>::rebuild()
{
    Index oldcap = capacity();
    Index newcap = static_cast<Index>(oldcap * growthfactor);

    Memory newmem(newcap);
    for(Element &elem : elements)
        if(elementExists(elem))
        {
            Node&& node = std::move(elem.value());
            Index newindex = hasher(node.key) % newcap;
            Element& newelem = newmem.at(newindex);
            newelem.emplace(std::move(node));
            // newmem[newindex].emplace(std::move(elem.value()));
        }
    // std::copy_if(std::execution::par,
    // std::make_move_iterator(std::begin(elements)),
    // std::make_move_iterator(std::end(elements)),
    // std::begin(newmem), elementExists);

    // std::move(std::execution::par, std::begin(elements), std::end(elements), std::begin(newmem));
    elements = std::move(newmem);
    length = std::count_if(std::execution::par, std::begin(elements), std::end(elements), elementExists);
}

} /* Namespace : ds */

/*
Index oldcap = capacity();
    Index newcap = oldcap + oldcap * HashTable::maxload;

    Memory newmem(newcap);
    // std::copy(std::execution::par, std::make_move_iterator(std::begin(elements)), std::make_move_iterator(std::end(elements)), std::begin(newmem));
    std::move(std::execution::par, std::begin(elements), std::end(elements), std::begin(newmem));
    elements = std::move(newmem);
*/

/*
    * std::count_if(std::execeution::par, std::begin(elements), std::end(elements), [](const Element& elem) { return elem.has_value();})
    * std::count(elements.cbegin(), elements.cend(), !std::nullopt);
    * OR
    * capacity() - std::count(elements.cbegin(), elements.cend(), std::nullopt);
    * But that would be less generic and only confined to standard optional
    * 
    * 
    * Iterator& operator+=(Index n)
            if(n > offset()) throw std::out_of_range("Iterator offset out of range! offset: " + std::to_string(offset()));
            Index o = 1;
            do
            {
                if((++ptr)->has_value()) ++o;
            } while (o != n);
            
            ptr += n;
*/
// return length = std::count_if(std::execution::par, std::begin(elements), std::end(elements), HashTable::valueExists); //this function is const