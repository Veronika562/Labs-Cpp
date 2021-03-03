#pragma once
template <typename T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T>
{
private:
	T* ptr;

public:
	Iterator() : ptr(nullptr) {}
	Iterator(T* p) : ptr(p) {}
	Iterator(const Iterator<T>& other) : ptr(other.ptr) {}

	Iterator<T>& operator=(const Iterator<T>& other) {
		ptr = other.ptr;
		return *this;
	}

	Iterator<T>& operator++() {
		ptr++;
		return *this;
	}
	Iterator<T>& operator--() {
		ptr--;		
		return *this;
	}
	Iterator<T> operator++(int) {
		Iterator<T> temp(*this);
		ptr++;
		return temp;
	}
	Iterator<T> operator--(int) {
		Iterator<T> temp(*this);
		ptr--;
		return temp;
	}

	T operator*() const { return *ptr; }
	T& operator*() { return *ptr; }
	T* operator->() { return ptr; }

	bool operator==(const Iterator<T>& it) const {
		return ptr == it.ptr;
	}
	bool operator!=(const Iterator<T>& it) const {
		return ptr != it.ptr;
	}

	Iterator<T> operator+(int n) {
		return ptr + n;
	}
	Iterator<T> operator-(int n) {
		return ptr - n;
	}

	bool operator>(Iterator<T> right) {
		return ptr > right.ptr;
	}
	bool operator>=(Iterator<T> right) {
		return ptr >= right.ptr;
	}
	bool operator<(Iterator<T> right) {
		return ptr < right.ptr;
	}
	bool operator<=(Iterator<T> right) {
		return ptr <= right.ptr;
	}

	Iterator<T> operator+=(int n) {
		ptr += n;
		return *this;
	}
	Iterator<T> operator-=(int n) {
		ptr -= n;
		return *this;
	}
};
