#pragma once
#include <initializer_list>
#include "Iterator.h"
using namespace std;

template <typename T>
class Vector
{
private:
	T* arr = nullptr;
	size_t capacity = 0;
	size_t length = 0;
public:
	Vector() { 
		arr = new T[3]; 
		capacity = 2;
		length = 0;
	}

	Vector(int len) {
		arr = new T[len + 1];
		capacity = len;
		length = len;
	}

	Vector(Vector<T>& other) {
		capacity = other.capacity;
		length = other.length;
		arr = new T[capacity + 1];
		length = other.length;
		for (size_t i = 0; i < length; i++) {
			arr[i] = other.arr[i];
		}
	}

	Vector(const initializer_list<T>& list) {
		int list_size = list.size();
		resize(list_size);
		int count = 0;
		for (auto &element : list) {
			arr[count++] = element;
		}
	}

	~Vector() {
		delete[] arr;
	}

	Iterator<T> begin() {
		Iterator<T> temp(arr);
		return temp;
	}

	Iterator<T> end() {
		Iterator<T> temp(arr + length);
		return temp;
	}

	void resize(int n) {
		if (n < capacity) {
			length = n;
		}
		else {
			T* temp = new T[n + 1];
			for (int i = 0; i < length; i++) {
				temp[i] = arr[i];
			}
			for (int i = length; i < n; i++) {
				temp[i] = 0;
			}
			delete[] arr;
			arr = temp;
			capacity = n;
			length = n;
		}
	}

	int size() { return length; }
	
	bool empty() { return length == 0; }
	
	void push_back(T value) {
		if (length == capacity) {
			T* temp = new T[capacity * 2 + 1];
			for (size_t i = 0; i < length; i++) {
				temp[i] = arr[i];
			}
			delete[] arr;
			arr = temp;
			capacity *= 2;
		}
		arr[length++] = value;
	}

	void pop_back() { 
		if (length > 0) {
			length--;
		}
	}

	T& operator[](int idx) {
		return arr[idx];
	}

	Vector<T>& operator=(const Vector<T>& other) {
		if (arr != other.arr) {
			length = other.length;
			capacity = other.capacity;
			delete[] arr;
			arr = new T[capacity + 1];
			for (size_t i = 0; i < length; i++) {
				this->arr[i] = other.arr[i];
			}
		}
		return *this;
	}
};
