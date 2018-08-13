#pragma once

template<typename TKey, typename TValue>
class ArrayCursor;
template<typename TKey, typename TValue>
class Dictionary {
	friend class ArrayCursor<TKey,TValue>;
private:
	template<typename TKey, typename TValue>
	struct Node {
		TKey key;
		TValue value;
		Node* next;
		Node(TKey k, TValue v, Node* n = nullptr) : key(k), value(v), next(n){}
	};
	Node<TKey,TValue>* dictStart;
	Node<TKey, TValue>& findItem(TKey value);
public:
	Dictionary();
	Dictionary(Dictionary<TKey, TValue>& other);
	~Dictionary();
	ArrayCursor<TKey, TValue> operator[](TKey key);
	Dictionary& operator=(Dictionary<TKey, TValue>& other);
	bool keyExists(TKey key) const;
};
template <typename TKey, typename TValue>
class ArrayCursor {
	friend class Dictionary<TKey, TValue>;
	typedef Dictionary<TKey, TValue>::Node<TKey, TValue> Node;
private:
	Node* node;
	TKey* key;
	Dictionary<TKey, TValue>& caller;
	ArrayCursor(Dictionary<TKey, TValue>& c, Node& node);
	ArrayCursor(Dictionary<TKey, TValue>& c, TKey key);
public:
	operator TValue();
	ArrayCursor& operator =(TValue v) {
		if (node == nullptr) {
			Node* curr = new Node(*key,v,caller.dictStart);
			caller.dictStart = curr;
		}
		else {
			node->value = v;
		}
		return *this;
	}
	TValue* operator->();
};

template<typename TKey, typename TValue>
inline Dictionary<TKey,TValue>::Node<TKey, TValue>& Dictionary<TKey, TValue>::findItem(TKey key)
{
	Node<TKey, TValue>* curr = dictStart;
	while (curr != nullptr) {
		if (curr->key == key)
			return *curr;
		curr = curr->next;
	}
	return *dictStart;
}

template<typename TKey, typename TValue>
inline Dictionary<TKey, TValue>::Dictionary()
{
	dictStart = nullptr;
}

template<typename TKey, typename TValue>
inline Dictionary<TKey, TValue>::Dictionary(Dictionary<TKey, TValue>& other)
{
	Node<TKey,TValue>* iter = other.dictStart;
	if (iter == nullptr) {
		dictStart = nullptr;
		return;
	}
	Node<TKey,TValue>* prev = new Node<TKey,TValue>(iter->key,iter->value);
	dictStart = prev;
	Node<TKey, TValue>* curr;
	iter = iter->next;
	while (iter != nullptr) {
		curr = new Node<TKey, TValue>(iter->key, iter->value);
		prev->next = curr;
		prev = curr;
		iter = iter->next;
	}
}

template<typename TKey, typename TValue>
inline Dictionary<TKey, TValue>::~Dictionary()
{
	Node<TKey, TValue>* curr = dictStart;
	Node<TKey, TValue>* next;
	while (curr != nullptr) {
		next = curr->next;
		delete curr;
		curr = next;
	}
}

template<typename TKey, typename TValue>
inline ArrayCursor<TKey, TValue> Dictionary<TKey, TValue>::operator[](TKey key)
{
	return keyExists(key) ? ArrayCursor<TKey, TValue>(*this, findItem(key)) : ArrayCursor<TKey, TValue>(*this, key);
}

template<typename TKey, typename TValue>
inline Dictionary<TKey,TValue> & Dictionary<TKey, TValue>::operator=(Dictionary<TKey, TValue>& other)
{
	if (&other == this) {
		return *this;
	}
	delete dictStart;
	Node<TKey, TValue>* iter = other.dictStart;
	if (iter == nullptr) {
		dictStart = nullptr;
		return *this;
	}
	Node<TKey, TValue>* prev = new Node<TKey, TValue>(iter->key, iter->value);
	dictStart = prev;
	Node<TKey, TValue>* curr;
	iter = iter->next;
	while (iter != nullptr) {
		curr = new Node<TKey, TValue>(iter->key, iter->value);
		prev->next = curr;
		prev = curr;
		iter = iter->next;
	}
	return *this;
}

template<typename TKey, typename TValue>
inline bool Dictionary<TKey, TValue>::keyExists(TKey key) const
{
	Node<TKey, TValue>* node = dictStart;
	while (node != nullptr) {
		if (node->key == key) {
			return true;
		}
		node = node->next;
	}
	return false;
}

template<typename TKey, typename TValue>
inline ArrayCursor<TKey, TValue>::ArrayCursor(Dictionary<TKey, TValue>& c, Node& node)
	: caller(c), node(&node),key(nullptr)
{
}

template<typename TKey, typename TValue>
inline ArrayCursor<TKey, TValue>::ArrayCursor(Dictionary<TKey, TValue>& c, TKey key) 
	: caller(c),key(new TKey(key)), node(nullptr)
{
}

template<typename TKey, typename TValue>
inline ArrayCursor<TKey, TValue>::operator TValue()
{
	return node->value;
}

template<typename TKey, typename TValue>
inline TValue * ArrayCursor<TKey, TValue>::operator->()
{
	return node == NULL ? NULL : &node->value;
}

