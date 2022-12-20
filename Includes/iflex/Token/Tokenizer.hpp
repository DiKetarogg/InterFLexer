#pragma once
#ifndef IFLEX_TOKEN_TOKENIZER_HPP
#define IFLEX_TOKEN_TOKENIZER_HPP

#include <regex>
#include <dtg/Utilities.hpp>
#include <unordered_map>
#include <vector>
#include "../String.hpp"
namespace iflex {
	/*
	template<class T>
	struct tokenable_hash_name {
		inline auto operator()(const T& key) const {
			return m_Hash(key.GetName());
		}

		std::hash<const typename T::CharType*> m_Hash;
	};
	*/
	template <class Traits>
	class Tokenizer {
		public:
		//using TokenablesArray = std::array<typename Traits::Tokenable, Traits::tokenablesCount>;
		//using RegularsArray = std::array<typename Traits::TokenableRegular, Traits::regularsCount>;

		public:
		using Token = typename Traits::Token;
		using CharType = typename Traits::CharType;
		using TokenEnum = typename Traits::TokenEnum;
		using Tokenable = typename Traits::Tokenable;
		using TokenableRegular = typename Traits::TokenableRegular;
		using TokenGroupMap = typename Traits::TokenGroupMap;

		protected:
		using TokenablesMap = std::unordered_map<dtg::SimpleString, TokenEnum>;
		using RegularsArray = std::vector<TokenableRegular>;
		public:
		Tokenizer(std::initializer_list<Tokenable> tokenables
			, std::initializer_list<TokenableRegular> regs
			, TokenGroupMap&& map)
		: m_Tokenables()
		, m_Regulars(regs)
		, m_TokenGroupMap(std::forward<TokenGroupMap>(map)) {
			for (const auto& e : tokenables) {
				m_Tokenables.emplace(e.GetName(), e.GetToken());
			}
		}
		Tokenizer(const Tokenizer&) = delete;
		Tokenizer(Tokenizer&&) = delete;

		private:
		TokenEnum FindRegular(const CharType* name) const {
			for (const auto& e : m_Regulars) {
				if (std::regex_match(name, e))
					return e.GetToken();
			}
			return Traits::NA;
		}

		private:
		static int cmpTokenableStr(const CharType* name, const Tokenable* tokable) {
			return GetStringCompare<CharType>::get(name, tokable->GetName());
		}
		static int cmpTokenableEnum(TokenEnum searched, const Tokenable* tokable) {
			return ((*tokable) < searched) - (tokable > searched);
		}
		public:

		TokenEnum Find(const CharType* name, size_t size) const {
//			if (!m_WordsSize)
//				return FindRegular(name);
			auto thief = dtg::BasicSimpleString<CharType>::
				StealingConstructor(const_cast<CharType*>(name), size);
			auto it = m_Tokenables.find(thief);
			thief.Lose();
			if (it == m_Tokenables.end())
				return FindRegular(name);
/*
			const Tokenable* toka = dtg::binarySearch(m_Tokenables.begin()
				, m_Tokenables.size()
				, name);
//				, cmpTokenableStr);
*/
			return it->second;
		}
		inline Token Tokenize(const CharType* name, size_t line, size_t character) const {
			return Token(name, Find(name, dtg::countElems(name)), line, character);
		}

		inline Token Tokenize(const CharType* name, size_t length, size_t line, size_t character) const {
			return Token(name, Find(name, length), line, character);
		}

		inline Token operator()(const CharType* name, size_t line, size_t character) const {
			return Token(name, Find(name, dtg::countElems(name)), line, character);
		}

		inline Token operator()(const CharType* name, size_t length, size_t line, size_t character) const {
			return Token(name, Find(name, length), line, character);
		}
		inline const TokenGroupMap& GetTokenGroupMap() const {
			return m_TokenGroupMap;
		}
		private:
		TokenablesMap m_Tokenables;
		RegularsArray m_Regulars;
		TokenGroupMap m_TokenGroupMap;
		size_t        m_WordsSize;
	};
}

//		TokenablesList		m_Tokenables;
//		TokenablesMap		m_MapTokenable;
/*
		const CharType* Find(TokenEnum token) const {
			if (!m_WordsSize)
				return GetEmptyString<CharType>::get;
			const Tokenable* toka = dtg::binarySearch(m_MapTokenable
				, m_WordsSize
				, token
				, cmpTokenableEnum
				);

			size_t l = 0;
			size_t m = (m_WordsSize - 1) / 2;
			size_t r = m_WordsSize;
			for (;;) {
				if (static_cast<size_t>(token) >
					static_cast<size_t>(m_MapTokenable[m].GetToken())) {
					l = m;
					m += (r - l + 1) / 2;
					if (l == m)
						break;
				}
				else if(static_cast<size_t>(token)
					< static_cast<size_t>(m_MapTokenable[m].GetToken())) {
					r = m;
					m -= (r - l + 1) / 2;
					if (m == r)
						break;
				}
				else
					return m_MapTokenable[m].GetName();
			}

			if (!toka)
				return GetEmptyString<CharType>::get;
			return toka->GetName();
		}
*/
#endif
