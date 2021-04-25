#pragma once
#ifndef IFLEX_TOKEN_TOKENABLE_HPP
#define IFLEX_TOKEN_TOKENABLE_HPP

#include <regex>

#include "String.hpp"
#include <dtg/Utilities.hpp>
namespace iflex {
	template<class Traits>
	class Tokenable {
	public:
	using CharType	= typename Traits::CharType;
	using Enum	= typename Traits::TokenEnum;

		Tokenable()
		:m_Name(const_cast<CharType*>
		(GetEmptyString<CharType>::get)), m_Enum(Traits::NA){};

		Tokenable(const CharType* name, Enum token, bool regular = false)
			:m_Name(
				regular ?
					reinterpret_cast<CharType*>(new std::basic_regex<CharType>(name))
				:
					const_cast<CharType*>(name)),
			m_Enum(token),
			m_IsRegular(regular){}

		Tokenable(const Tokenable& other)
			:m_Name(
			other.m_IsRegular ?
					reinterpret_cast<CharType*>
					(new std::basic_regex<CharType>(*other.m_Regular))
				:
					other.m_Name
			),
			m_Enum(other.m_Enum), m_IsRegular(other.m_IsRegular){}

		void Swap(Tokenable& other) {
			dtg::Swap(m_Name, other.m_Name);
			dtg::Swap(m_Enum, other.m_Enum);
			dtg::Swap(m_IsRegular, other.m_IsRegular);
		}

		const CharType* GetName() const {
			if (m_IsRegular)
				return 0;
			return m_Name;
		}

		const std::basic_regex<CharType>* GetRegular() const {
			if (m_IsRegular)
				return m_Regular;
			return 0;
		}

		bool Regular() const {
			return m_IsRegular;
		}

		Enum GetToken() const {
			return m_Enum;
		}

		Tokenable& operator=(const Tokenable& other) {
			if (m_IsRegular)
				delete m_Regular;

			if (other.m_IsRegular)
				m_Regular = new std::basic_regex<CharType>(*other.m_Regular);
			else
				m_Name = other.m_Name;

			m_IsRegular = other.m_IsRegular;
			m_Enum = other.m_Enum;
			return *this;
		}

		Tokenable& operator=(Tokenable&& other) {
			m_Name = other.m_Name;
			m_IsRegular = other.m_IsRegular;
			m_Enum = other.m_Enum;
			other.m_Name = 0;
			return *this;
		}

		~Tokenable() {
			if (m_IsRegular)
				delete m_Regular;
		}
	private:

		union {
		CharType*			m_Name;
		std::basic_regex<CharType>* 	m_Regular;
		};
		Enum				m_Enum;
		bool				m_IsRegular;
	}; //class Tokenable
} // namespace iflex
#endif
