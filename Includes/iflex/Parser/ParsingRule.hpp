#pragma once
#ifndef IFLEX_PARSER_PARSINGRULE_HPP
#define IFLEX_PARSER_PARSINGRULE_HPP
#endif

#include <unordered_set>
/*
namespace iflex {

	template <class Traits>
	class Recipe {
		public:
		using typename Traits::TokenGroup;
		using typename Traits::TokenEnum;
		public:
		template<class... Args>
		Recipe(Args... args): m_Groups(std::forward<Args>(args)...) {}
		bool Match(const TokenEnum* begin, const TokenEnum* end) {
			for (const auto& e : m_Groups) {
				if (e.Find(begin)) {
					return 1;
				}
			}
		}
		private:
		std::initializer_list<TokenGroup> m_Groups;
	}; // class ParsingRule
}// namespace iflex
*/
