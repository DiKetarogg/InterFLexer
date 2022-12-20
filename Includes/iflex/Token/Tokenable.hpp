#pragma once
#ifndef IFLEX_TOKEN_TOKENABLE_HPP
#define IFLEX_TOKEN_TOKENABLE_HPP

#include <regex>

#include "../String.hpp"
#include <dtg/Utilities.hpp>
namespace iflex {
	template<class Traits>
	class TokenableRegular : public std::basic_regex<typename Traits::CharType> {
	public:
		using CharType	= typename Traits::CharType;
		using Enum	= typename Traits::TokenEnum;
		using Base	= std::basic_regex<typename Traits::CharType>;

//		constexpr TokenableRegular():m_Enum(Traits::NA){};

		constexpr TokenableRegular(const CharType* expression, Enum token)
			: Base(expression)
			, m_Enum(token) {}

		constexpr TokenableRegular(const TokenableRegular& other) = default;
		constexpr TokenableRegular(TokenableRegular&& other) = default;
			//:Base(other.m_Regular)
			//,m_Enum(other.m_Enum) {}

		constexpr void Swap(TokenableRegular& other) {
			dtg::Swap(static_cast<Base&>(*this), static_cast<Base&>(other));
			dtg::Swap(m_Enum, other.m_Enum);
		}

		Enum GetToken() const noexcept {
			return m_Enum;
		}

		constexpr TokenableRegular& operator = (const TokenableRegular& other) {
			Base::operator=(other);
			m_Enum = other.m_Enum;
			return *this;
		}

		constexpr TokenableRegular& operator = (TokenableRegular&& other) {
			Base::operator=(std::forward<std::basic_regex<CharType>>(other));
			m_Enum = other.m_Enum;
			return *this;
		}

	private:
		Enum m_Enum;
	}; //class TokenableRegular

	template<class Traits>
	class Tokenable {
	public:
	using CharType	= typename Traits::CharType;
	using Enum	= typename Traits::TokenEnum;

		constexpr Tokenable()
		:m_Name(GetEmptyString<CharType>::get), m_Enum(Traits::NA) {};

		constexpr Tokenable(const CharType* name, Enum token)
			:m_Name(name)
			,m_Enum(token) {}

		constexpr Tokenable(const Tokenable& other) = default;
//			:m_Name(other.m_Name),
//			m_Enum(other.m_Enum) {}

		void Swap(Tokenable& other) {
			dtg::Swap(m_Name, other.m_Name);
			dtg::Swap(m_Enum, other.m_Enum);
		}

		const CharType* GetName() const noexcept {
			return m_Name;
		}

		Enum GetToken() const noexcept {
			return m_Enum;
		}

		constexpr Tokenable& operator = (const Tokenable& other) {
			m_Name = other.m_Name;
			m_Enum = other.m_Enum;
			return *this;
		}
/*
		Tokenable& operator = (Tokenable&& other) {
			m_Name = other.m_Name;
			m_Enum = other.m_Enum;
			other.m_Name = 0;
			return *this;
		}
*/
		constexpr Tokenable& operator = (const char *name) {
			m_Name = name;
			return *this;
		}

		constexpr Tokenable& operator = (Enum en) {
			m_Enum = en;;
			return *this;
		}
		constexpr bool operator < (const Tokenable& other) const noexcept {
			return GetStringCompare<CharType>::get(m_Name, other.m_Name) < 0;
		}

		constexpr bool operator <= (const Tokenable& other) const noexcept {
			return GetStringCompare<CharType>::get(m_Name, other.m_Name) <= 0;
		}

		constexpr bool operator == (const Tokenable& other) const noexcept {
			return GetStringCompare<CharType>::get(m_Name, other.m_Name) == 0;
		}

		constexpr bool operator != (const Tokenable& other) const noexcept {
			return GetStringCompare<CharType>::get(m_Name, other.m_Name) != 0;
		}

		constexpr bool operator > (const Tokenable& other) const noexcept {
			return GetStringCompare<CharType>::get(m_Name, other.m_Name) > 0;
		}

		constexpr bool operator >= (const Tokenable& other) const noexcept {
			return GetStringCompare<CharType>::get(m_Name, other.m_Name) >= 0;
		}

		explicit operator Enum() const noexcept {
			return m_Enum;
		}

	private:
		const CharType* m_Name;
		Enum m_Enum;
	}; //class Tokenable
	/*
	template<class Traits>
	class TokenablesList {
		public:
		using TokenablesArray = std::array<typename Traits::Tokenable, Traits::tokenablesCount>;
		using RegularsArray = std::array<typename Traits::TokenableRegular, Traits::regularCount>;
		public:
		template<class Range>
		constexpr TokenablesList(Range&& range):Base(dtg::quickSort(std::forward<Range>(range))) {}

		private:
		TokenablesArray m_Tokenables;
		RegularsArray m_Regulars;
	};
*/
/*
	template<class Traits>
	class TokenablesMap : public std::array<typename Traits::Tokenable, Traits::tokenablesCount> {
		public:
		using Base = std::array<typename Traits::Tokenable, Traits::tokenablesCount>;
		public:
		template<class Range>
		constexpr TokenablesMap(Range&& range):Base(dtg::quickSort(std::forward<Range>(range))) {}
	};
*/
} // namespace iflex
#endif
