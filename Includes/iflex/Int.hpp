#pragma once
#ifndef IFLEX_INT_HPP
#define IFLEX_INT_HPP
#endif

#include <cstdint>

namespace iflex {
	template<int size>
	struct IntType;
	
	template<>
	struct IntType<1> {
		typedef int8_t type;
	};
	template<>
	struct IntType<2> {
		typedef int16_t type;
	};
	template<>
	struct IntType<4> {
		typedef int32_t type;
	};
	template<>
	struct IntType<8> {
		typedef int64_t type;
	};
}
