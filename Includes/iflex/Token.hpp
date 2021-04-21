#pragma once
#ifndef IFLEX_TOKEN_HPP
#define IFLEX_TOKEN_HPP

#include <dtg/Utilities.hpp>
#include <regex>

namespace iflex {
	template<class TokenTypes, size_t TokenizerSize, TokenTypes NOTAPPLICALE>
	struct Traits {
		using token_types = TokenTypes;

		static const size_t	size	= TokenizerSize;
		static const TokenTypes NA	= NOTAPPLICALE;
	};

	template<class Traits>
	class Token {
	public:
		using Type = Traits::TokenTypes;
		Token():m_Name(0),m_Type(Traits::NA){}
		
		Token(const char* name, size_t ch, size_t line):m_Name(name),
			m_Char(ch),m_Line(line),m_Type(type){}


		void SetName(const char* name) {
			m_Name = name;
		}

		void SetLine(size_t line) {
			m_Line = line;
		}

		void SetChar(size_t ch) {
			m_Char = ch;
		}

		void SetType(Type type) {
			m_Type = type;
		}

		Set(const char* name, size_t line, size_t ch, Type type) {
		m_Name = const_cast<char*>(name);
		m_Line = line;
		m_Char = ch;
		m_Type = type;
		}
		
	private:
		char*	m_Name;
		size_t	m_Line;
		size_t	m_Char;
		Type	m_Type;
	};

	template<class Traits>
	class Tokenable {
	public:
		using Type = Traits::TokenTypes;

		Tokenable():m_Name(0), m_Type(Traits::NA){};

		Tokenable(const char* name, Type type, bool regular = false):m_Name(name),
			m_Type(type), m_Regular(regular){}

		Tokenable(const Tokenable&):m_Name(other.m_Name),
			m_Type(other.m_Type), m_Regular(other.m_Regular){}

		void Swap(Tokenable& other) {
			dtg::Swap(m_Name, other.m_Name);
			dtg::Swap(m_Type, other.m_Type);
			dtg::Swap(m_Regular, other.m_Regular);
		}

		const char* GetName() const {
			return m_Name;
		}

		Type GetType() const {
			return m_Type;
		}

		bool Regular() const {
			return m_Regular;
		}
	private:
		char*	m_Name;
		Type	m_Type;
		bool	m_Regular;
	};

	template <class Traits>
	class Tokenizer {
		public:
		using Type = Traits::TokenTypes;
		protected:
		Tokenizer(Tokenable* Tokenables):m_Tokenables(Tokenables){Sort();}

		Tokenizer() = delete;
		Tokenizer(const Tokenizer&) = delete;
		Tokenizer(Tokenizer&&) = delete;


		size_t SortOutRegular() {
			Tokenable temp
			size_t j = Traits::size - 1;
			for (size_t i = 0; i != j; ++i) {
				if(m_Tokenables[i].Regular())
				for (;i != j;--j) {
					if(!m_Tokenables[j].Regular()) {
						m_Tokenables[i].Swap(m_Tokenables[j]);
						--j;
						break;
					}
				}
			}
			return j;
		}

		void Sort() {
			size_t j = SortOutRegular();
			if (!m_Tokenables[j].Regular()) {// Check if all Tokenables are regulars
				dtg::MergeSort(m_Tokenables, j,
					[](const Tokenable& first, const Tokenable& second)->bool {
						return strcmp(first.GetName(), second.GetName());
				});
			}
		}

		Type FindRegular(const char* name) {
			for (size_t i = m_ReservedSize + 1; i != Traits::size; ++i) {
				if (std::regex_match(name, m_Tokenables[i].GetName())
					return m_Tokenables[i].GetType());
			}
			return Traits::NA;
		}

		public:

		static Type Find(const char* name) {
			size_t l = 0;
			size_t m = m_ReservedSize / 2;
			size_t r = m_ReservedSize - 1;
			int temp;
			for (;;) {
				if (temp = strcmp(name,m_Tokenables[m]) > 0) {
					l = m;
					m += (r - l) / 2;
					if (l == m)
						break;
				}
				else if(temp < 0) {
					r = m;
					m -= (r - l) / 2;
					if (m == r)
						break;
				}
				else
					return m_Tokenables[m].GetType();
			}
			return FindRegular(name);
		}

		private:
		static Tokenable*	m_Tokenables;
		static Tokenable*	m_MapTokenable[Traits::size];
		static size_t		m_ReservedSize;
	};



}

#endif



















