/*
#pragma once
#ifndef IFLEX_PARSEABLE_HPP
# define IFLEX_PARSEABLE_HPP
#endif

namespace iflex {
	namespace detail {
		enum ParseableMessage {
			INVALID,
			VALID,
			FINISHED
		};
		template<class Enum, unsigned int i, Enum token, Enum... tokens>
		struct parseable_pattern : parseable_pattern<Enum, i - 1, tokens...> {
			template<>
			token get<i>() {
				return token;
			}
		};
		template<class Enum, unsigned int i>
		struct parseable_pattern<Enum, i, Enum token> {
			template<unsigned int i>
			token get();
			template<>
			token get<i>() {
				return token;
			}
		}
	}
	template<unsigned int i, unsigned int is...>
	class ParamsPattern {
		Ret
	};

	template<class Enum, Enum... tokens>
	class ParseablePattern : public detail::parseable_pattern<Enum, 0, tokens...>{

	};

	template<class Enum, class ParamsPattern, class FunctionType, class Pattern>
	class Parseable {
		public:
		template<unsigned int i>
		detail::ParseableMessage Test(Enum token) {
			if (Pattern::get<i>() != token)
				return detail::ParseableMessage::INVALID;
			if (i == Pattern::size)
				return detail::ParseableMessage::FINISHED;
			return detail::ParseableMessage::VALID;
		}
	};
}
*/
