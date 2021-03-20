#pragma once

template <typename T> class Iterator;
template <typename T> class List;

template <typename T>
struct node {
	T data;
	struct node* next;
	struct node* prev;
};

template <typename T>
class List
{
private:
	node<T>* head;
	node<T>* tail;
	size_t length;
	void delete_all() {
		while (length != 0) {
			node<T>* elem = head;
			head = head->next;
			delete elem;
			length--;
		}
	}

public:
	List() {
		head = nullptr;
		tail = nullptr;
		length = 0;
	}

	List(List<T>& other) {
		node<T>* temp = other.head;
		while (temp != nullptr) {
			push_back(temp->data);
			temp = temp->next;
		}
		length = other.length;
	}

	~List() {
		delete_all();
	}

	List(const std::initializer_list<T>& list) {
		for (auto& element : list) {
			push_back(element);
		}
	}

	List<T>& operator=(List<T>& other) {
		if (length == other.length) {
			node<T> elem_this, elem_other;
			while (elem_other.next != nullptr) {
				elem_this.data = elem_other.data;
				elem_this = elem_this.next;
				elem_other = elem_other.next;
			}
		}
		else {
			delete_all();
			head = nullptr;
			tail = nullptr;
			length = 0;
			node<T>* temp = other.head;
			while (temp != nullptr) {
				push_back(*temp.data);
				temp = temp->next;
			}
		}
		return *this;
	}

	void push_back(T value) {
		node<T>* elem = new node<T>;
		(*elem).next = nullptr;
		(*elem).prev = tail;
		(*elem).data = value;
		if (tail != nullptr) {
			(*tail).next = elem;
		}
		if (length == 0) {
			head = tail = elem;
		} 
		else {
			tail = elem;
		}
		length++;
	}

	void push_front(T value) {
		node<T>* elem = new node<T>;
		elem->next = head;
		elem->prev = nullptr;
		elem->data = value;
		if (head != nullptr) {
			head->prev = elem;
		}
		if (length == 0) {
			head = tail = elem;
		}
		else {
			head = elem;
		}
		length++;
	}

	void pop_back() {
		node<T>* temp = tail;
		tail = tail->prev;
		delete temp;
		tail->next = nullptr;
		length--;
	}

	void pop_front() {
		node<T>* temp = head;
		head = head->next;
		delete temp;
		head->prev = nullptr;
		length--;
	}

	Iterator<T> begin() {
		node<T>* p = head;
		Iterator<T> it(p);
		return it;
	}
	Iterator<T> end() {
		node<T>* p = tail;
		Iterator<T> it(p);
		return it;
	}
};

template <typename T>
class Iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
private:
	node<T>* ptr;

public:
	Iterator() { ptr = nullptr; }
	Iterator(node<T>* p) { ptr = p; }
	Iterator(const Iterator<T>& other) { ptr = other.ptr; }
	Iterator<T>& operator++() {
		ptr = ptr->next;
		return *this;
	}
	Iterator<T>& operator--() {
		ptr = ptr->prev;
		return *this;
	}
	Iterator<T> operator++(int) {
		Iterator<T> temp(*this);
		ptr = (*ptr).next;
		return temp;
	}
	Iterator<T> operator--(int) {
		Iterator<T> temp(*this);
		ptr = *ptr.prev;
		return temp;
	}

	bool operator==(const Iterator& other) const { return ptr == other.ptr; }
	bool operator!=(const Iterator& other) const { return ptr != other.ptr; }

	T operator*() const { return *ptr.data; }
	T& operator*() { return (*ptr).data; }
	Iterator<T>* operator->() { return ptr; }
};
