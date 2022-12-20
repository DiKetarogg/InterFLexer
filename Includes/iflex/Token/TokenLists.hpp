#pragma once
#ifndef IFLEX_TOKENLISTS_HPP
#define IFLEX_TOKENLISTS_HPP
#include <deque>
#include <set>
#include <dtg/Utilities.hpp>

namespace iflex {
	template<class Traits>
	class TokenList : public std::deque<typename Traits::Token> {
		public:
		using Token		= typename Traits::Token;
		using Enum		= typename Traits::TokenEnum;
		using CharType		= typename Traits::CharType;
		using Base		= std::deque<Token>;

		TokenList() = default;

		//template<class... Args>
		//TokenList(Args... args):Base(std::forward<Args>(args)...){}

		//template<class... Args>
		//TokenList(const char* name, Args... args):Base(args...), m_Name(name){}

/*
		const char* GetName() const noexcept{
			return m_Name;
		}
		void SetName(const char* name) {
			m_Name = name;
		}
*/
//		DTG_SPACESHIP_OPERATOR(const TokenList& other, std::strcmp(m_Name, other.m_Name), 0);
//		DTG_SPACESHIP_OPERATOR(const char* other, std::strcmp(m_Name, other), 0);

		private:
		//const char* m_Name;
	};

	template<class Traits>
	class TokenLists : protected std::unordered_map<dtg::SimpleString, TokenList<Traits>> {
		public:
		using TokenList = typename Traits::TokenList;
		using Base = std::unordered_map<dtg::SimpleString, TokenList>;
		using Token = typename Traits::Token;
		using TokenEnum = typename Traits::TokenEnum;
		using GroupEnum = typename Traits::GroupEnum;
		public:
		using Base::begin;
		using Base::end;
		using Base::cbegin;
		using Base::cend;
		using typename Base::iterator;
		using typename Base::const_iterator;
		//1 if file existed
		//0 if file created
		inline bool SetActive(const char* name, size_t length) {
			auto thief = dtg::SimpleString::StealingConstructor(const_cast<char *>(name), length);
			auto it = Base::find(thief);
			if (it != Base::end()) {
				m_Active = it;
				return 1;
			}
			m_Active = Base::emplace(thief, TokenList{}).first;
			thief.Lose();
			return 0;
		}
		
		inline void SetActive(iterator it) {
			m_Active = it;
		}

		inline iterator GetActive() {
			return m_Active;
		}
		
		inline const_iterator GetActive() const noexcept {
			return m_Active;
		}

		inline void Push(Token token) {
			m_Active->second.push_back(token);
		}
		private:
		iterator m_Active;
	};
}

#endif
