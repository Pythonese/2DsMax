#pragma once
template <class T>
class List
{
	struct Node
	{
		Node* next = nullptr;
		T data = T();
		~Node()
		{
			delete next;
			next = nullptr;
		}
	};
	Node* head = nullptr;
public:
	static class Iterator
	{
		Node* node;
	public:
		Iterator(Node* node)
		{
			this->node = node;
		}
		Iterator& operator++()
		{
			node = node->next;
			return *this;
		}
		Iterator operator++(int i)
		{
			Iterator res = *this;
			node = node->next;
			return res;
		}
		bool operator!=(Iterator iter)
		{
			return node != iter.node;
		}
		T& operator*()
		{
			return node->data;
		}
	};
	T& getFirst()
	{
		return head->data;
	}
	virtual void push(const T& t)
	{
		if (head == nullptr)
		{
			head = new Node;
			head->data = t;
		}
		else
		{
			Node* newHead = new Node;
			newHead->next = head;
			newHead->data = t;
			head = newHead;
		}
	}
	bool remove(const T& t)
	{
		if (head == nullptr)
			return false;
		Node* iter = head;
		if (head->data == t)
		{
			head = head->next;
			delete iter;
			return true;
		}
		while (iter->next->data != t)
		{
			iter = iter->next;
			if (iter->next == nullptr)
				return false;
		}
		Node* deletedNode = iter->next;
		iter->next = iter->next->next;
		deletedNode->next = nullptr;
		delete deletedNode;
		return true;
	}
	friend bool moveItem(List<T>& a, List<T>& b, const T& item)
	{
		a.push(item);
		return b.remove(item);
	}
	bool contains(const T& t)
	{
		Node* iter = head;
		while (iter != nullptr)
		{
			if (iter->data == t)
				return true;
			iter = iter->next;
		}
		return false;
	}
	void clear()
	{
		delete head;
		head = nullptr;
	}
	bool isEmpty()
	{
		return head == nullptr;
	}
	size_t getSize()
	{
		Node* iter = head;
		size_t res = 0;
		while (iter != nullptr)
		{
			iter = iter->next;
			res++;
		}
		return res;
	}
	List<T>::Iterator begin()
	{
		return head;
	}
	List<T>::Iterator end()
	{
		return nullptr;
	}
};