#pragma once
#ifndef IFLEX_TOKEN_TOKENIZER_HPP
#define IFLEX_TOKEN_TOKENIZER_HPP

#include <regex>
#include <dtg/Utilities.hpp>
#include "../String.hpp"
namespace iflex {
	template <class Traits>
	class Tokenizer {
		public:
		using Enum = typename Traits::TokenEnum;
		using Tokenable = typename Traits::Tokenable;
		using Token = typename Traits::Token;
		using CharType = typename Traits::CharType;
		protected:
		
		void Initialize(Tokenable* names, size_t size) {
			static bool called = false;
			if (!called) {
				m_Tokenables = names;
				m_Size = size;
				m_MapTokenable = new Tokenable*[size];
				called = true;
				Sort();
			}
		}

		Tokenizer() = default;

		//Tokenizer():m_Tokenables(Traits::Names){ Sort(); };
		Tokenizer(const Tokenizer&) = delete;
		Tokenizer(Tokenizer&&) = delete;

		private:

		void SortOutRegular() {
			size_t& j = m_WordsSize;
			j = m_Size - 1;
			while(j != 0 && m_Tokenables[j].Regular())
				--j;
			for (size_t i = 0; i != j; ++i) {
				if(m_Tokenables[i].Regular())
				for (;i != j;--j) {
					if(!m_Tokenables[j].Regular()) {
						m_Tokenables[i].Swap(m_Tokenables[j]);
						do {
						--j;
						} while(j != i && m_Tokenables[j].Regular());
						break;
					}
				}
			}
			++j;	
		}

		void Sort() {
			SortOutRegular();
			if (m_WordsSize != m_Size) {// Check if there are non-regular tokens
				dtg::MergeSort<Tokenable>(m_Tokenables, m_WordsSize,
					[](const Tokenable& first, const Tokenable& second)->bool {
						return iflex::GetStringCompare<CharType>::
						get(first.GetName(), second.GetName()) < 0;
				});
			}

			for (size_t i = 0; i != m_Size; ++i) {
				m_MapTokenable[i] = m_Tokenables + i;
			}

			dtg::MergeSort<Tokenable*>(m_MapTokenable, m_Size,
					[](Tokenable* const& first, Tokenable* const& second)->bool {
						return first->GetToken() < second->GetToken();
					}
				);
		}

		Enum FindRegular(const CharType* name) const {
			for (size_t i = m_WordsSize; i != m_Size; ++i) {
				if (std::regex_match(name, *m_Tokenables[i].GetRegular()))
					return m_Tokenables[i].GetToken();
			}
			return Traits::NA;
		}

		public:

		Enum Find(const CharType* name) const {
			if (!m_WordsSize)
				return FindRegular(name);
			size_t l = 0;
			size_t m = (m_WordsSize - 1) / 2;
			size_t r = m_WordsSize - 1;
			int temp;
			for (;;) {
				if ((temp = GetStringCompare<CharType>::
					get(name,m_Tokenables[m].GetName())) > 0) {
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
					return m_Tokenables[m].GetToken();
			}
			return FindRegular(name);
		}

		const CharType* Find(Enum token) const {
			if (!m_WordsSize)
				return GetEmptyString<CharType>::get;
			size_t l = 0;
			size_t m = (m_WordsSize - 1) / 2;
			size_t r = m_WordsSize - 1;
			for (;;) {
				if (static_cast<size_t>(token) >
					static_cast<size_t>(m_Tokenables[m].GetToken())) {
					l = m;
					m += (r - l + 1) / 2;
					if (l == m)
						break;
				}
				else if(static_cast<size_t>(token)
					< static_cast<size_t>(m_Tokenables[m].GetToken())) {
					r = m;
					m -= (r - l + 1) / 2;
					if (m == r)
						break;
				}
				else
					return m_Tokenables[m].GetName();
			}
			return GetEmptyString<CharType>::get;
		}

		inline Token Tokenize(const CharType* name, size_t line, size_t character) {
			return Token(name, Find(name), line, character);
		}

		inline Token operator()(const CharType* name, size_t line, size_t character) {
			return Token(name, Find(name), line, character);
		}

		~Tokenizer() {
			delete[] m_MapTokenable;
		}
		private:
		Tokenable*		m_Tokenables;
		Tokenable**		m_MapTokenable;
		size_t			m_WordsSize;
		size_t			m_Size;
	};
}
#endif
