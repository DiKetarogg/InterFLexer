#pragma once
#ifndef IFLEX_TOKEN_HPP
#define IFLEX_TOKEN_HPP

#include <dtg/Utilities.hpp>

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

		Tokenable() = delete;

		Tokenable(const char* name, Type type, bool regular = false):m_Name(name),
			m_Type(type), m_Regular(regular){}

		Tokenable(const Tokenable&) = delete;

		Tokenable(Tokenable&&) = delete;

		void Swap(Tokenable& other) {
			dtg::Swap(m_Name, other.m_Name);
			dtg::Swap(m_Type, other.m_Type);
			dtg::Swap(m_Regular, other.m_Regular);
		}

		bool Regular() {
			return m_Regular;
		}
	private:
		char*	m_Name;
		Type	m_Type;
		bool	m_Regular;
	};

	template <class Traits>
	class Tokenizer {
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

			}
		}

		public:

		private:
		static Tokenable* m_Tokenables;
		static Tokenable* m_MapTokenable[Traits::size];
	};



}

#endif



















