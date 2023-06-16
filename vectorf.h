#pragma once
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <functional>
#include <string>
#include <vector>

namespace std
{
	template <typename T>
	class vectorf : public vector<T>
	{
	public:
		vectorf();
		vectorf(initializer_list<T>);
		bool tryGetOne(function<bool(const T&, T&)>, T&, size_t&);
		vector<reference_wrapper<T>> getSelection(function<bool(const T&, T&)>, T&);
	};
	
	template<typename T>
	inline vectorf<T>::vectorf() {}

	template<typename T>
	vectorf<T>::vectorf(initializer_list<T> init) : vector<T>(init) {}

	template<typename T>
	bool vectorf<T>::tryGetOne(function<bool(const T&, T&)> selector, T& comparer, size_t& index)
	{
		bool result = false;
		for (size_t i = 0; i < this->size(); i++)
		{
			if (selector((*this)[i], comparer))
			{
				comparer = ((*this)[i]);
				index = i;
				result = true;
				break;
			}
		}
		return result;
	}
	
	template<typename T>
	vector<reference_wrapper<T>> vectorf<T>::getSelection(function<bool(const T&, T&)> selector, T& comparer)
	{
		vector<reference_wrapper<T>> v;
		for (auto& data : *this) { if (selector(data, comparer)) v.push_back(ref(data)); }
		return v;
	}
}