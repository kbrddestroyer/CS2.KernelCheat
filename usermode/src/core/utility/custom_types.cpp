#include "custom_types.h"

template <typename T>
Exception<T>::Exception()
{
	this->val = (T) 0;
}

template <typename T>
Exception<T>::Exception(T val)
{
	this->val = val;
}

template <typename T>
T Exception<T>::err() { return val; }

template <typename T>
Exception<T>::operator T() const
{
	return this->val;
}