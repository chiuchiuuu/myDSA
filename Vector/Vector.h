#pragma once

#include <exception>
typedef unsigned int Rank;
#define DEFAULT_CAPACITY 10

template<typename T>
class Vector
{
protected:
	Rank _size;
	int _capacity;
	T *_elem;
	void copyFrom(const T *A, Rank lo, Rank hi);
	// Dynamic memory allocation
	void expand();
	void shrink();

public:
	// construction
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0)
	{
		_elem = new T[_capacity = c];
		for (_size = 0; _size < s; _elem[_size++] = v);
	}
	Vector(const T *A, Rank n) { copyFrom(A, 0, n); }
	Vector(const T *A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }
	Vector(const Vector<T> &v) { copyFrom(v._elem, 0, v._size); }
	Vector(const Vector<T> &v, Rank lo, Rank hi) { copyFrom(v._elem, lo, hi); }
	
	// deconstruction
	~Vector() { delete[] _elem; }

	// conmon API
	Rank size() const { return _size; }
	bool empty() const { return !_size; }
	int disorder() const;
	T& operator[](Rank r) const;
	Vector<T>& operator=(const Vector<T> &V);
	T remove(Rank r);
	int remove(Rank lo, Rank hi);
	Rank insert(Rank r, const T &e);
	Rank insert(const T &e) { return insert(_size, e); }	// insert as last

	// API for unsorted vector
	Rank find(const T &e) const { return find(e, 0, _size); }
	Rank find(const T &e, Rank lo, Rank hi) const;
	void sort(Rank lo, Rank hi);
	void sort() { sort(0, _size); }
	int deduplicate();

	// API for sorted vector
	Rank search(const T &e) const { return 0 >= _size ? -1 : search(e, 0, _size); }
	Rank search(const T &e, Rank lo, Rank hi) const;
	void unsort(Rank lo, Rank hi);
	void unsort() { unsort(0, _size); }
	int uniquify();

	// traverse
	void traverse(void(*visit)(T &));
	template<typename VST>
	void traverse(VST& visit);

};

template<typename T>
void Vector<T>::copyFrom(const T *A, Rank lo, Rank hi)
{
	_elem = new T[_capacity = 2 * (hi - lo)];
	_size = 0;
	while (lo < hi)
		_elem[_size++] = A[lo++];
}

template<typename T>
int Vector<T>::disorder() const
{
	int count = 0;
	for (int i = 0; i < _size - 1; i++)
		if (_elem[i] > _elem[i + 1])
			count++;
	return count;
}

template<typename T>
T& Vector<T>::operator[](Rank r) const
{
	if (size() < r)
		throw out_of_range("invalid Vector<T> subscript");
	return _elem[r];
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T> &V)
{
	if (_elem)
		delete[] _elem;
	copyFrom(V._elem, 0, V.size());
	return *this;
}

template<typename T>
T Vector::remove(Rank r)
{
	T tmp = _elem[r];
	remove(r, r + 1);
	return tmp;
}

template<typename T>
int Vector<T>::remove(Rank lo, Rank hi)
{
	if (lo == hi) return 0;
	while (hi <_size)
		_elem[lo++] = _elem[hi++];
	_size = lo;
	shrink();
	return hi - lo;
}

template<typename T>
void Vector<T>::expand()
{
	if (_size < _capacity) return;
	if (_capacity < DEFAULT_CAPACITY)
		_capacity = DEFAULT_CAPACITY;
	T *oldElem = _elem;
	_elem = new T[_capacity <<= 1];
	// can't use memcpy(_elem, oldElem, _size*sizeof(T))(shallow copy)
	for (int i = 0; i < _size; i++)
		_elem[i] = oleElem[i]; 
	delete[] oldElem;
}

template<typename T>
void Vector<T>::shrink()
{
	if (_capacity < (DEFAULT_CAPACITY << 1)) return; // no less than default capacity
	if ((_size << 2) > _capacity) return; // threshold 25%
	T *oldElem = _elem;
	_elem = new T[_capacity >>= 1];
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i];
	delete[] oldElem;
}

template<typename T>
Rank Vector<T>::insert(Rank r, const T &e)
{
	expand();
	for (int i = _size; i > r; i--)
		_elem[i] = _elem[i - 1];
	_elem[r] = e;
	_size++;
	return r;
}

template<typename T>
Rank Vector<T>::find(const T &e, Rank lo, Rank hi) const
{
	if (lo >= hi)
		throw invalid_argument("lo should be less than hi");
	if (hi > _size || lo > _size)
		throw out_of_range("rank out of range");

	while ((lo < hi--) && e != _elem[hi]));
	return hi;
}

template<typename T>
void Vector<T>::traverse(void(*visit)(T &))
{
	for (int i = 0; i < _size; i++)
		visit(_elem[i]);
}

template<typename T, typename VST>
void Vector<T>::traverse(VST& visit)
{
	for (int i = 0; i < _size; i++)
		visit(_elem[i]);
}