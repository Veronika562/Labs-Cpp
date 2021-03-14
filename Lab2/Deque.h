#pragma once
#include <initializer_list>
#include <cmath>

template <typename T> class Deque;
template <typename T> class Iterator;

template <typename T> class Deque
{
	friend class Iterator<T>;

private:
	T** units;
	size_t deque_size = 0;
	static const size_t unit_size = 60;
	size_t capacity = 0;
	T* begin_ptr, * end_ptr;
	size_t begin_unit = 0, end_unit = 0;

public:
	Deque();
	Deque(Deque<T>& other);
	Deque(const std::initializer_list<T>& list);
	Deque<T>& operator=(Deque<T>& other);
	~Deque();
	T& operator[](int idx);
	void push_back(T value);
	void push_front(T value);
	void pop_back();
	void pop_front();
	void clear();
	int size();
	bool empty();
	void resize(int n);
	Iterator<T> begin();
	Iterator<T> end();
};


template <typename T> class Iterator
{
	friend Iterator<T> Deque<T>::begin();
	friend Iterator<T> Deque<T>::end();
private:
	T* unit;
	T* element_ptr;
	Deque<T>* deque_ptr = nullptr;

public:
	Iterator();
	Iterator(const Iterator<T>& other);
	Iterator<T>& operator=(const Iterator<T>& other);
	Iterator<T>& operator++();
	Iterator<T>& operator--();
	Iterator<T> operator++(int);
	Iterator<T> operator--(int);
	T operator*() const;
	T& operator*();
	bool operator==(const Iterator<T>& other) const;
	bool operator!=(const Iterator<T>& other) const;
	Iterator<T> operator+(int n);
	Iterator<T> operator-(int n);
	Iterator<T> operator+=(int n);
	Iterator<T> operator-=(int n);
};


template <typename T>
Deque<T>::Deque() {
	units = new T * [2];
	units[0] = new T[unit_size];
	units[1] = new T[unit_size];
	capacity = unit_size * 2;
	begin_ptr = end_ptr = units[0];
}

template <typename T>
Deque<T>::Deque(Deque<T>& other) {
	capacity = other.capacity;
	units = new T * [capacity / unit_size];
	for (size_t i = 0; i < capacity / unit_size; i++) {
		units[i] = new T[unit_size];
	}
	begin_ptr = end_ptr = units[0];
	for (int i = 0; i < other.size(); i++) {
		push_back(other[i]);
	}
}

template <typename T>
Deque<T>::Deque(const std::initializer_list<T>& list) {
	capacity = static_cast<int>(ceil((float)list.size() / unit_size)) * unit_size;
	units = new T * [capacity / unit_size];
	for (size_t i = 0; i < capacity / unit_size; i++) {
		units[i] = new T[unit_size];
	}
	begin_ptr = end_ptr = units[0];
	for (auto& element : list) {
		push_back(element);
	}
}

template <typename T>
Deque<T>& Deque<T>::operator=(Deque<T>& other) {
	if (units != other.units) {
		for (size_t i = 0; i < this->capacity / unit_size; i++) {
			delete[] units[i];
		}
		delete[] units;
		deque_size = 0;
		capacity = other.capacity;
		units = new T * [capacity / unit_size];
		for (size_t i = 0; i < capacity / unit_size; i++) {
			units[i] = new T[unit_size];
		}
		begin_ptr = end_ptr = units[0];
		begin_unit = end_unit = 0;
		for (int i = 0; i < other.size(); i++) {
			push_back(other[i]);
		}
	}
	return *this;
}

template <typename T>
Deque<T>::~Deque() {
	for (int i = 0; i < static_cast<int>(capacity / unit_size); i++) {
		delete[] units[i];
	}
	delete[] units;
}

template <typename T>
T& Deque<T>::operator[](int idx) {
	idx += begin_ptr - units[begin_unit];
	return units[(begin_unit + idx / unit_size) % (capacity / unit_size)][idx % unit_size];
}

template <typename T>
void Deque<T>::push_back(T value) {
	if (end_ptr == units[end_unit] + unit_size - 1) {
		if (end_unit == begin_unit - 1 || end_unit == capacity / unit_size - 1) {
			size_t temp_end = deque_size;
			size_t temp_end_unit = (capacity / unit_size - begin_unit + end_unit) % (capacity / unit_size);
			resize(capacity * 2 - 1);
			deque_size = temp_end;
			end_unit = temp_end_unit;
			end_ptr = &(this->operator[](deque_size));
		}
		*end_ptr = value;
		end_ptr = units[++end_unit];
	}
	else {
		*end_ptr = value;
		end_ptr++;
	}
	deque_size++;
}

template <typename T>
void Deque<T>::push_front(T value) {
	if (deque_size == 0) {
		end_ptr++;
	}
	if (begin_ptr == units[begin_unit]) {
		if (end_unit == (begin_unit - 1 + capacity / unit_size) % (capacity / unit_size)) {
			size_t temp = deque_size;
			size_t temp_end_unit = (capacity / unit_size - begin_unit + end_unit) % (capacity / unit_size);
			resize(capacity * 2 - 1);
			deque_size = temp;
			end_unit = temp_end_unit;
			end_ptr = &(this->operator[](deque_size));
			begin_unit = capacity / unit_size - 1;
			begin_ptr = units[begin_unit] + unit_size - 1;
			*begin_ptr = value;
		}
		else {
			if (begin_unit == 0) {
				begin_unit = capacity / unit_size - 1;
				begin_ptr = units[begin_unit] + unit_size - 1;
				*begin_ptr = value;
			}
			else {
				begin_unit--;
				begin_ptr = units[begin_unit] + unit_size - 1;
				*begin_ptr = value;
			}
		}
	}
	else {
		begin_ptr--;
		*begin_ptr = value;
	}
	deque_size++;
}

template <typename T>
void Deque<T>::pop_back() {
	if (end_ptr == units[end_unit]) {
		end_ptr = units[--end_unit] + unit_size - 1;
	}
	else {
		end_ptr--;
	}
	deque_size--;
}

template <typename T>
void Deque<T>::pop_front() {
	if (begin_ptr == units[begin_unit] + unit_size - 1) {
		if (begin_unit == capacity / unit_size - 1) {
			begin_ptr = units[0];
			begin_unit = 0;
		}
		else {
			begin_ptr = units[++begin_unit];
		}
	}
	else {
		begin_ptr++;
	}
	deque_size--;
}

template <typename T>
void Deque<T>::clear() {
	resize(0);
}

template <typename T>
int Deque<T>::size() { return deque_size; }

template <typename T>
bool Deque<T>::empty() { return deque_size == 0; }

template <typename T>
void Deque<T>::resize(int n) {
	n++;
	size_t begin_index = begin_ptr - units[begin_unit];
	size_t new_num_of_units = static_cast<size_t>(ceil(static_cast<float>(n + begin_index) / unit_size));
	T** temp = new T * [new_num_of_units];
	size_t j = 0, i = begin_unit;
	if (static_cast<size_t>(n) > capacity - begin_index) {
		for (; j < capacity / unit_size; i++, j++) {
			temp[j] = *(units + (i % (capacity / unit_size)));
		}
		for (; j < new_num_of_units; j++) {
			temp[j] = new T[unit_size];
		}
	}
	else if (static_cast<size_t>(n) < capacity - begin_index - 1) {
		for (; j < new_num_of_units; i++, j++) {
			temp[j] = units[i % (capacity / unit_size)];
		}
		for (; j < capacity / unit_size; i++, j++) {
			delete[] units[i % (capacity / unit_size)];
		}
	}
	delete[] units;
	units = temp;
	begin_unit = 0;
	capacity = new_num_of_units * unit_size;
	deque_size = n - 1;
	end_ptr = &this->operator[](deque_size);
	end_unit = new_num_of_units - 1;
}

template <typename T>
Iterator<T> Deque<T>::begin() {
	Iterator<T> it;
	it.deque_ptr = this;
	it.unit = units[begin_unit];
	it.element_ptr = begin_ptr;
	return it;
}

template <typename T>
Iterator<T> Deque<T>::end() {
	Iterator<T> it;
	it.deque_ptr = this;
	it.unit = units[end_unit];
	it.element_ptr = end_ptr;
	return it;
}

template <typename T>
Iterator<T>::Iterator() {
	deque_ptr = nullptr;
	unit = nullptr;
	element_ptr = 0;
}

template <typename T> Iterator<T>::Iterator(const Iterator<T>& other) {
	deque_ptr = other.deque_ptr;
	unit = other.unit;
	element_ptr = other.element_ptr;
}

template <typename T>
Iterator<T>& Iterator<T>::operator=(const Iterator<T>& other) {
	deque_ptr = other.deque_ptr;
	unit = other.unit;
	element_ptr = other.element_ptr;
	return *this;
}

template <typename T>
Iterator<T>& Iterator<T>::operator++() {
	if (element_ptr - unit == deque_ptr->unit_size - 1) {
		for (int i = 0; i < deque_ptr->capacity / deque_ptr->unit_size; i++) {
			if (unit == deque_ptr->units[i]) {
				unit = deque_ptr->units[(i + 1) % (deque_ptr->capacity / deque_ptr->unit_size)];
				element_ptr = deque_ptr->units[(i + 1) % (deque_ptr->capacity / deque_ptr->unit_size)];
				break;
			}
		}
	}
	else {
		element_ptr++;
	}
	return *this;
}

template <typename T>
Iterator<T>& Iterator<T>::operator--() {
	if (element_ptr - unit == 0) {
		for (int i = 0; i < deque_ptr->capacity / deque_ptr->unit_size; i++) {
			if (unit == deque_ptr->units[i]) {
				unit = deque_ptr->units[(i - 1 + deque_ptr->capacity / deque_ptr->unit_size)
					% (deque_ptr->capacity / deque_ptr->unit_size)];
				element_ptr = deque_ptr->units[(i - 1 + deque_ptr->capacity / deque_ptr->unit_size) 
					% (deque_ptr->capacity / deque_ptr->unit_size)] + deque_ptr->unit_size - 1;
				break;
			}
		}
	}
	else {
		element_ptr--;
	}
	return *this;
}

template <typename T>
Iterator<T> Iterator<T>::operator++(int) {
	Iterator<T> temp(*this);
	if (element_ptr - unit == deque_ptr->unit_size - 1) {
		for (int i = 0; i < deque_ptr->capacity / deque_ptr->unit_size; i++) {
			if (unit == deque_ptr->units[i]) {
				unit = deque_ptr->units[(i + 1) % (deque_ptr->capacity / deque_ptr->unit_size)];
				element_ptr = deque_ptr->units[(i + 1) % (deque_ptr->capacity / deque_ptr->unit_size)];
				break;
			}
		}
	}
	else {
		element_ptr++;
	}
	return temp;
}

template <typename T>
Iterator<T> Iterator<T>::operator--(int) {
	Iterator<T> temp(*this);
	if (element_ptr - unit == 0) {
		for (int i = 0; i < deque_ptr->capacity / deque_ptr->unit_size; i++) {
			if (unit == deque_ptr->units[i]) {
				unit = deque_ptr->units[(i - 1 + deque_ptr->capacity / deque_ptr->unit_size) 
					% (deque_ptr->capacity / deque_ptr->unit_size)];
				element_ptr = deque_ptr->units[(i - 1 + deque_ptr->capacity / deque_ptr->unit_size)
					% (deque_ptr->capacity / deque_ptr->unit_size)] + deque_ptr->unit_size - 1;
				break;
			}
		}
	}
	else {
		element_ptr--;
	}
	return temp;
}

template <typename T>
T Iterator<T>::operator*() const {
	return *element_ptr;
}

template <typename T>
T& Iterator<T>::operator*() {
	return *element_ptr;
}

template <typename T>
bool Iterator<T>::operator==(const Iterator<T>& other) const {
	return element_ptr == other.element_ptr;
}

template <typename T>
bool Iterator<T>::operator!=(const Iterator<T>& other) const {
	return element_ptr != other.element_ptr;
}

template <typename T>
Iterator<T> Iterator<T>::operator+(int n) {
	Iterator temp;
	temp.deque_ptr = deque_ptr;
	int current_index = element_ptr - unit;
	if (n < (int)deque_ptr->unit_size - current_index) {
		temp.element_ptr = &unit[current_index + n];
		temp.unit = unit;
	}
	else {
		for (size_t i = 0; i < deque_ptr->capacity / deque_ptr->unit_size; i++) {
			if (unit == deque_ptr->units[i]) {
				n -= (deque_ptr->unit_size - current_index);
				size_t full_units = n / deque_ptr->unit_size;
				temp.unit = deque_ptr->units[(i + full_units + 1) % (deque_ptr->capacity / deque_ptr->unit_size)];
				n -= full_units * deque_ptr->unit_size;
				temp.element_ptr = &(temp.unit[n]);
				break;
			}
		}
	}
	return temp;
}

template <typename T>
Iterator<T> Iterator<T>::operator-(int n) {
	Iterator temp;
	temp.deque_ptr = deque_ptr;
	int current_index = element_ptr - unit;
	if (n <= current_index) {
		temp.element_ptr = &unit[current_index - n];
		temp.unit = unit;
	}
	else {
		for (size_t i = 0; i < deque_ptr->capacity / deque_ptr->unit_size; i++) {
			if (unit == deque_ptr->units[i]) {
				n -= current_index;
				size_t full_units = n / deque_ptr->unit_size;
				temp.unit = deque_ptr->units[(i - full_units - 1 + deque_ptr->capacity / deque_ptr->unit_size) 
					% (deque_ptr->capacity / deque_ptr->unit_size)];
				n -= full_units * deque_ptr->unit_size;
				temp.element_ptr = &(temp.unit[deque_ptr->unit_size - n - 1]);
				break;
			}
		}
	}
	return temp;
}

template <typename T>
Iterator<T> Iterator<T>::operator+=(int n) {
	int current_index = element_ptr - unit;
	if (n < deque_ptr->unit_size - current_index) {
		element_ptr = &unit[current_index + n];
	}
	else {
		for (int i = 0; i < deque_ptr->capacity / deque_ptr->unit_size; i++) {
			if (unit == deque_ptr->units[i]) {
				n -= (deque_ptr->unit_size - current_index);
				size_t full_units = n / deque_ptr->unit_size;
				unit = deque_ptr->units[(i + full_units + 1) % (deque_ptr->capacity / deque_ptr->unit_size)];
				n -= full_units * deque_ptr->unit_size;
				element_ptr = &unit[n];
				break;
			}
		}
	}
	return *this;
}

template <typename T>
Iterator<T> Iterator<T>::operator-=(int n) {
	int current_index = element_ptr - unit;
	if (n <= current_index) {
		element_ptr = &unit[current_index - n];
	}
	else {
		for (size_t i = 0; i < deque_ptr->capacity / deque_ptr->unit_size; i++) {
			if (unit == deque_ptr->units[i]) {
				n -= current_index;
				size_t full_units = n / deque_ptr->unit_size;
				unit = deque_ptr->units[(i - full_units - 1 + deque_ptr->capacity / deque_ptr->unit_size) 
					% (deque_ptr->capacity / deque_ptr->unit_size)];
				n -= full_units * deque_ptr->unit_size;
				element_ptr = &unit[deque_ptr->unit_size - n - 1];
				break;
			}
		}
	}
	return *this;
}
