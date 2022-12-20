#pragma once
#ifndef IFLEX_TOKEN_TOKENGROUP_HPP
#define IFLEX_TOKEN_TOKENGROUP_HPP
#endif
#include <unordered_set>
#include "../Int.hpp"

namespace iflex {
	template<class T>
	struct int_hash {
		using integral_type = typename iflex::IntType<sizeof(T)>::type;

		int_hash() = default;

		inline auto operator()(const T& key) const {
			return m_Hash(static_cast<integral_type>(key));
		}
		
		std::hash<integral_type> m_Hash;
	};

	template <class Traits>
	class TokenGroup : public std::unordered_set<typename Traits::GroupEnum
			, int_hash<typename Traits::GroupEnum> > {
		public:
		using Base = std::unordered_set<typename Traits::GroupEnum>;
		using GroupEnum = typename Traits::GroupEnum;
		public:
		TokenGroup() = default;
		TokenGroup(std::initializer_list<GroupEnum> list)
		: Base(list) {}
		private:
	};//class TokenGroup
	template <class Traits>
	class TokenGroupMap : public std::unordered_map<typename Traits::TokenEnum, typename Traits::TokenGroup
			, int_hash<typename Traits::TokenEnum>> {
		public:
		using TokenEnum = typename Traits::TokenEnum;
		using GroupEnum = typename Traits::GroupEnum;
		using TokenGroup = typename Traits::TokenGroup;
		using CharType = typename Traits::CharType;
		using Base = std::unordered_map<TokenEnum, TokenGroup>;
		public:
		TokenGroupMap(std::initializer_list<
		std::pair<GroupEnum, std::initializer_list<TokenEnum> > > list) {
			for (const auto& e : list) {
				for (const auto& l : e.second) {
					(*this)[l].insert(e.first);
				}
			}
		}

		template<class... Args>
		TokenGroupMap(Args... args) {
			(Init(args), ...);
		}
		template<class... Args>
		TokenGroupMap(std::pair<GroupEnum, std::initializer_list<TokenEnum> > pair, Args... args) {
			Init(pair);
			(Init(args), ...);
		}
		bool TokenInGroup(TokenEnum token, GroupEnum group) const {
			auto token_it = this->find(token);
			if (token_it == this->end())
				return 0;
			auto group_it = token_it->second.find(group);
			return group_it != token_it->second.end();
		}
		private:
		inline void Init(std::pair<GroupEnum, std::initializer_list<TokenEnum> > pair) {
			for (const auto& e : pair.second)
				operator[](pair.first).insert(e);
		}
		inline void Init(std::pair<GroupEnum, TokenEnum> pair) {
			operator[](pair.first).insert(pair.second);
		}
	};// class TokenGroupMap
}// namespace iflex
