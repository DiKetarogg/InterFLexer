#pragma once
#ifndef IFLEX_TOKEN_TOKEN_HPP
#define IFLEX_TOKEN_TOKEN_HPP

#include <dtg/Utilities.hpp>
#include <regex>
#include "../String.hpp"
namespace iflex {

	template<class Traits>
	class Token {
	public:
		using Enum = typename Traits::TokenEnum;
		using CharType = typename Traits::CharType;

		Token():m_Name(GetEmptyString<CharType>::get),m_Enum(Traits::NA){}
		
		Token(const CharType* name, Enum type, size_t line, size_t character)
			:m_Name(const_cast<CharType*>(name)),
			m_Line(line),m_Char(character),m_Enum(type){}

		Token(const CharType* name, Enum type, const std::pair<size_t, size_t>& location)
			:m_Name(const_cast<CharType*>(name)),
			m_Line(location.first),m_Char(location.second),m_Enum(type){}

		void SetName(const CharType* name) {
			m_Name = name;
		}

		void SetLine(size_t line) {
			m_Line = line;
		}

		void SetChar(size_t ch) {
			m_Char = ch;
		}

		void SetType(Enum type) {
			m_Enum = type;
		}

		void Set(const CharType* name, size_t line, size_t ch, Enum type) {
			m_Name = const_cast<CharType*>(name);
			m_Line = line;
			m_Char = ch;
			m_Enum = type;
		}
	
		const CharType* GetName() const {
			return m_Name;
		}

		size_t GetLine() const {
			return m_Line;
		}

		size_t GetChar() const {
			return m_Char;
		}

		Enum GetType() const {
			return m_Enum;
		}	
	private:
		CharType*	m_Name;
		size_t		m_Line;
		size_t		m_Char;
		Enum		m_Enum;
	};

#if defined(_GLIBCXX_OSTREAM) or defined(_OSTREAM_)

template<class Traits>
std::ostream& operator <<(std::ostream& stream, const Token<Traits>& other) {
	stream << '\'' << other.GetName() << '\'' << "{Line:" <<
	other.GetLine() << ", Char:" << other.GetChar() << ", ID: " << other.GetType() << '}';
	return stream;
}

template<class Traits>
std::wostream& operator <<(std::wostream& stream, const Token<Traits>& other) {
	stream << L'\'' << other.GetName() << L'\'' << L"{Line:" <<
	other.GetLine() << L", Char:" << other.GetChar() << L", ID: " << other.GetType() << L'}';
	return stream;
}
#endif

}

#endif



















