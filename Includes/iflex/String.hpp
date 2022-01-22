#pragma once
#ifndef IFLEX_TOKEN_STRING_HPP
#define IFLEX_TOKEN_STRING_HPP
#include <cstring>
namespace iflex {

	template <class T>
	class GetStringCompare;
	
	template<>
	struct GetStringCompare<char>{
		static constexpr auto get = strcmp;
	};

	template<>
	struct GetStringCompare<wchar_t>{
		static constexpr auto get = wcscmp;
	};

	template <class T>
	class GetEmptyString;
	
	template<>
	struct GetEmptyString<char>{
		static constexpr const char* get = "";
	};

	template<>
	struct GetEmptyString<wchar_t>{
		static constexpr const wchar_t * get = L"";
	};

	template<class T>
	struct GetLineFeed;

	template<>
	struct GetLineFeed<char>{
		static constexpr auto get = '\n';
	};
	template<>
	struct GetLineFeed<wchar_t>{
		static constexpr auto get = L'\n';
	};

}
#endif
