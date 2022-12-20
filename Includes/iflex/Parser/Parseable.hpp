#pragma once
#ifndef IFLEX_PARSER_PARSEABLE_HPP
# define IFLEX_PARSER_PARSEABLE_HPP
#endif
#include <vector>
#include <set>
namespace iflex {
	template<class Traits>
	class ParseableContext {
		public:
		using TokenEnum = typename Traits::TokenEnum;
		using Token = typename Traits::Token;
		using iterator = typename Traits::TokenList::iterator;
		using Iterator = typename Traits::TokenList::iterator;
		using const_iterator = typename Traits::TokenList::const_iterator;
		using ConstIterator = typename Traits::TokenList::const_iterator;

		public:
		ParseableContext(Iterator tokens, Iterator end)
		:m_Begin(tokens), m_End(end) {}

//		const Token& operator[](size_t i) const {
//			return m_TokensStart[i];
//		}

		Iterator begin() {
			return m_Begin;
		}

		ConstIterator begin() const {
			return m_Begin;
		}

		Iterator end() {
			return m_End;
		}

		ConstIterator end() const {
			return m_End;
		}

		Token& operator [](std::size_t n) {
			return *(m_Begin + n);
		}
		const Token& operator [](std::size_t n) const {
			return *(m_Begin + n);
		}
		private:
		Iterator m_Begin;
		Iterator m_End;
	};

	template<class Traits, class Visitor>
	class Parseable {
		public:
		using Token = typename Traits::Token;
		using TokenEnum = typename Traits::TokenEnum;
		using GroupEnum = typename Traits::GroupEnum;
		using TokenGroup = typename Traits::TokenGroup;
		using CommandEnum = typename Traits::CommandEnum;
		using TokenGroupMap = typename Traits::TokenGroupMap;
		using ParseableContext = typename Traits::ParseableContext;

		using ContextIterator = typename ParseableContext::iterator;

		public:

		void SetTokenableGroupMap(const TokenGroupMap* map) noexcept {
			m_TokenGroupMap = map;
		}

		const TokenGroupMap* GetTokenableGroupMap() const noexcept {
			return m_TokenGroupMap;
		}

		TokenGroupMap* GetTokenableGroupMap() noexcept {
			return m_TokenGroupMap;
		}

		constexpr Parseable(std::initializer_list<GroupEnum> list
		,void (Visitor::*hook)(ParseableContext))
		: m_Groups(list)
		, m_Hook(hook) {}

		constexpr Parseable(const Parseable& other)
		: m_Groups(other.m_Groups)
		, m_TokenGroupMap(other.m_TokenGroupMap)
		, m_Hook(other.m_Hook) {}

		constexpr Parseable(Parseable&& other)
		: m_Groups(std::forward<std::vector<GroupEnum>>(other.m_Groups))
		, m_TokenGroupMap(other.m_TokenGroupMap)
		, m_Hook(other.m_Hook) {}

		Parseable& operator=(const Parseable& other) {
			m_Groups = other.m_Groups;
			m_TokenGroupMap = other.m_TokenGroupMap;
			m_Hook = other.m_Hook;
			return *this;
		}

		Parseable& operator=(Parseable&& other) {
			m_Groups = std::forward<std::vector<GroupEnum>>(other.m_Groups);
			m_TokenGroupMap = other.m_TokenGroupMap;
			m_Hook = other.m_Hook;
			return *this;
		}

		constexpr inline TokenEnum GetGroups() const noexcept {
			return m_Groups;
		}

		constexpr inline void SetGroupsMap(const TokenGroupMap* map) {
			m_TokenGroupMap = map;
		}
/*
		constexpr inline ParseEnum GetGroups() {
			return m_Result;
		}
*/
		ContextIterator Match(ContextIterator begin, ContextIterator end) const {
			ContextIterator it = begin;
			for (const auto& e : m_Groups) {
				if (it == end || !m_TokenGroupMap->TokenInGroup(it->GetType(), e))
					return begin;
				++it;
			}
			return it;
		}

		int Match(const Token* begin, int size) const {
			int match_size = 0;
			if (static_cast<int>(m_Groups.size()) < size)
				return 0;
			for (const auto& e : m_Groups) {
				if (!m_TokenGroupMap->TokenInGroup(begin->GetType(), e))
					return 0;
				++match_size;
				++begin;
			}
			return match_size;
		}
		int Match(const TokenEnum* begin, int size) const {
			int match_size = 0;
			if (static_cast<int>(m_Groups.size()) < size)
				return 0;
			for (const auto& e : m_Groups) {
				if (!m_TokenGroupMap->TokenInGroup(*begin, e))
					return 0;
				++match_size;
				++begin;
			}
			return match_size;
		}

		inline int Match(const TokenEnum* begin, const TokenEnum* end) const {
			return Match(begin, (static_cast<size_t>(end)
			- static_cast<size_t>(begin) ) / sizeof(void *));
		}

		inline void Execute(Visitor& visitor, ContextIterator begin, ContextIterator end) const {
			(visitor.*m_Hook)({begin, end});
		}

		int Compare(const Parseable& other) const {
			int ifeq;
			int size;
			if (m_Groups.size() == other.m_Groups.size()) {
				ifeq = 0;
				size = m_Groups.size();
			}
			else if (m_Groups.size() < other.m_Groups.size()) {
				ifeq = 1;
				size = m_Groups.size();
			}
			else {
				ifeq = -1;
				size = m_Groups.size();
			}
			for (int i = 0; i != size; ++i) {
				const auto& e = m_Groups[i];
				const auto& o = other.m_Groups[i];
				if (e < o)
					return -1;
				if (e > o)
					return 1;
			}
			return ifeq;
		}

		int Compare(GroupEnum token) const {
			const auto& e = m_Groups.front();
			return (e > token) - (e < token);
		}

		DTG_SPACESHIP_OPERATOR(const Parseable& other, Compare(other), 0);

		DTG_SPACESHIP_OPERATOR(GroupEnum token, Compare(token), 0);

		friend bool operator >= (GroupEnum token, const Parseable& other) {
			return other < token;
		}

		friend bool operator > (GroupEnum token, const Parseable& other) {
			return other < token;
		}

		friend bool operator == (GroupEnum token, const Parseable& other) {
			return other == token;
		}
		friend bool operator <= (GroupEnum token, const Parseable& other) {
			return other >= token;
		}

		private:
		std::vector<GroupEnum> m_Groups;
		const TokenGroupMap* m_TokenGroupMap;
		void (Visitor::* m_Hook)(ParseableContext);
	};// class Parseable
	template<class Traits, class Visitor>
	class ParseableList
	: public std::vector<typename Traits::template Parseable<Visitor>> {
		public:
		using Base = std::vector<typename Traits::template Parseable<Visitor>>;
		using TokenEnum = typename Traits::TokenEnum;
		using Parseable = typename Traits::template Parseable<Visitor>;
		using TokenGroupMap = typename Traits::TokenGroupMap;
		public:
		ParseableList(const TokenGroupMap &map, std::initializer_list<Parseable> list)
		: Base(dtg::quickSort(Base(list)))
		, m_TokenGroupMap(map) {
			for (auto& e : *this) {
				e.SetGroupsMap(&m_TokenGroupMap);
			}
		}

		std::set<const Parseable*> FindParseables(TokenEnum token) {
			std::set<const Parseable*> set;
			const Parseable* last = &Base::back() + 1;
			auto it = m_TokenGroupMap.find(token);
			if (it == m_TokenGroupMap.end())
				return set;
			for (const auto& e : it->second) {
				const Parseable* it = dtg::binarySearchFirst(&Base::front(), Base::size(), e);
				if (!it)
					continue;
				auto hint = set.insert(it).first;
				do {
					hint = set.insert(hint, it);
					++it;
				}
				while (it && it != last && *it == e);
			}
			return set;
		}

		private:
		const TokenGroupMap& m_TokenGroupMap;
	};// class ParseableList
}// namespace iflex
