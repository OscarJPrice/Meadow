#ifndef _MEADOW_DEBUG_MYVARIANT_HPP
#define _MEADOW_DEBUG_MYVARIANT_HPP

#include <stdint>
#include <typeinfo>

class Any {

	void* data;
	std::type_info& type_id;

public:

	Any() = default;

	template <typename T>
	set(T* value) : data((void*)value) {
		type_id = typeid(T);
	}

	template <typename T>
	change(T value) {
		if (typeid(T) == type_id) {
			*(T*)data = value;
		}
		else {
			throw std::invalid_argument("Type mismatch");
		}
	}

};



#endif