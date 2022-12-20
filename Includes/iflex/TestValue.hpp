#ifndef IFLEX_TESTVALUE_HPP
#define IFLEX_TESTVALUE_HPP
#include "CrossPlatform.hpp"
#include <dtg/Pack.hpp>
namespace iflex {
	namespace is {
		typedef enum S{} S, Space, Whitespace, space, whitespace, ws, WS, spc, Spc, Sp, sp;
		typedef enum P{} P, Printable, Print, Prnt, p, printable, print, prnt;
		typedef enum C{} C, Control, Ctrl, Cntrl, c, control, crtl, cntrl;
		typedef enum U{} U, Upper, Up, Upr, Upp, Uppr, u, upper, up, upr, upp, uppr;
		typedef enum L{} L, Lower, Low, Lwr, Lowr, l, lower, low, lwr, lowr;
		typedef enum A{} A, Alpha, Alph, Al, a, alpha, alph, al;
		typedef enum D{} D, Digit, Dgt, d, digit, dgt;
		typedef enum Punct{} Punctuation, Punct, Pnct, Punc;
		typedef enum H{} H, Hex, Hexdigit, Hexdgt, Hdigit, Hdgt, Hx,
			Hxdigit, Hxdgt, x, Xdigit, Xdgt, Hexd, Hxd, Xd, XD,
			h, hex, hexdigit, hexdgt, hdigit, hdgt, hx,
			hxdigit, hxdgt, xdigit, xdgt, hexd, hxd, xd;
		typedef enum AN{}Anum, Alphanum, Alphnum, Alnum, Anu, Alphanu, Alphnu, Alnu,
			An, Alphan, Alphn, Aln, Anumeric, Alphanumeric, Alphnumeric,
			Alnumeric, Anmrc, Alphanmrc, Alphnmrc, Alnmrc,
			anum, alphanum, alphnum, alnum, anu, alphanu, alphnu, alnu,
			an, alphan, alphn, aln, anumeric, alphanumeric, alphnumeric,
			alnumeric, anmrc, alphanmrc, alphnmrc, alnmrc,
			ANum, AlphaNum, AlphNum, AlNum, ANu, AlphaNu, AlphNu, AlNu,
			AN, AlphaN, AlphN, AlN, ANumeric, AlphaNumeric, AlphNumeric,
			AlNumeric, ANmrc, AlphaNmrc, AlphNmrc, AlNmrc;
		typedef enum G{}G, Graph, Graphical, Grph, Grphcl, Grphc,
			g, graph, graphical, grph, grphcl, grphc;
		typedef enum B{}B, Blank, Blnk, b, blank, blnk;
		typedef enum All{}All, Any, all, any;
		typedef enum None{} None, none, No, no;
		template<auto val>
		struct Value {
			static constexpr const decltype(val) value = val;
		};
	};
	namespace logic {
		typedef enum AND{} AND, And, A;
		typedef enum OR{}  OR, Or, O;
		//enum{XOR, X};
		typedef enum NOT{} NOT, Not, N;
		typedef enum NAND{}NAND, Nand, NA, NOTAND, NotAnd, Notand, Na;
		typedef enum NOR{} NOR, NO, NOTOR, Nor, NOr, No, NotOr, Notor;
		typedef enum TRUE{} TRUE, True, T;
		typedef enum FALSE{} FALSE, False, F;
		typedef enum TO{} BETWEEN, Between, To;
		//enum{NXOR, NX};
	};

	template<bool>
	struct BoolTypeStruct;

	template<>
	struct BoolTypeStruct<true> {
		typedef logic::True type;
	};
	template<>
	struct BoolTypeStruct<false> {
		typedef logic::False type;
	};


	template<class...Expression>
	struct TestValueStruct;

	template<auto value>
	struct TestValueStruct<is::Value<value>> {
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c, const std::locale& locale) {
			(void)locale;
			return c == value;
		}
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c) {
			return c == value;
		}
	};

	template<auto value1, auto value2>
	struct TestValueStruct<is::Value<value1>, logic::To, is::Value<value2>> {
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c, const std::locale& locale) {
			(void)locale;
			return value1 <= c && c >= value2;
		}
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c) {
			return value1 <= c && c >= value2;
		}
	};

	template<>
	struct TestValueStruct<logic::True> {
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c, const std::locale& locale) {
			static_cast<void>(c);
			static_cast<void>(locale);
			return true;
		}

		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c) {
			static_cast<void>(c);
			return true;
		}
	};

	template<>
	struct TestValueStruct<logic::False> {
		template<class CompareType>
			static constexpr IFLEX_FORCE_INLINE
			bool TestFor(CompareType c, const std::locale& locale) {
				static_cast<void>(c);
				static_cast<void>(locale);
				return false;
			}

		template<class CompareType>
			static constexpr IFLEX_FORCE_INLINE
			bool TestFor(CompareType c) {
				static_cast<void>(c);
				return false;
			}
	};

	template<>
	struct TestValueStruct<is::Punct> {
		template<class CompareType>
			static constexpr IFLEX_FORCE_INLINE
			bool TestFor(CompareType c, const std::locale& locale) {
				return std::ispunct(c, locale);
			}
		template<class CompareType>
			static constexpr IFLEX_FORCE_INLINE
			bool TestFor(CompareType c) {
				return std::ispunct(c);
			}
	};

	template<>
	struct TestValueStruct<is::Any> {
		template<class CompareType>
			static constexpr IFLEX_FORCE_INLINE
			bool TestFor(CompareType c) {
				return c != 0;
			}
		template<class CompareType>
			static constexpr IFLEX_FORCE_INLINE
			bool TestFor(CompareType c, const std::locale& locale) {
				static_cast<void>(locale);
				return c != 0;
			}
	};
	
	template<>
	struct TestValueStruct<is::AlphaNumeric> {
		template<class CompareType>
			static constexpr IFLEX_FORCE_INLINE
			bool TestFor(CompareType c, const std::locale& locale) {
				return std::isalnum(c, locale);
			}
		template<class CompareType>
			static constexpr IFLEX_FORCE_INLINE
			bool TestFor(CompareType c) {
				return std::isalnum(c);
			}
	};

	template<>
	struct TestValueStruct<is::Digit> {
		template<class CompareType>
			static constexpr IFLEX_FORCE_INLINE
			bool TestFor(CompareType c, const std::locale& locale) {
				return std::isdigit(c, locale);
			}
		template<class CompareType>
			static constexpr IFLEX_FORCE_INLINE
			bool TestFor(CompareType c) {
				return std::isdigit(c);
			}
	};
	template<>
	struct TestValueStruct<is::Alpha> {
		template<class CompareType>
			static constexpr IFLEX_FORCE_INLINE
			bool TestFor(CompareType c, const std::locale& locale) {
				return std::isalpha(c, locale);
			}
		template<class CompareType>
			static constexpr IFLEX_FORCE_INLINE
			bool TestFor(CompareType c) {
				return std::isalpha(c);
			}
	};

	template<>
	struct TestValueStruct<is::Space> {
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c, const std::locale& locale) {
			return std::isspace(c, locale);
		}
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c) {
			return std::isspace(c);
		}
	};

	template<>
	struct TestValueStruct<is::None> {
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c, const std::locale& locale) {
			static_cast<void>(c);
			static_cast<void>(locale);
			return false;
		}
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c) {
			static_cast<void>(c);
			return false;
		}
	};


	template<class Check, class Check2, class... Rest>
	struct TestValueStruct<Check, logic::AND, Check2, Rest...> {
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c, const std::locale& locale) {
			return TestValueStruct< typename BoolTypeStruct
					< TestValueStruct<Check>::TestFor(c, locale)
					&& TestValueStruct<Check2>::TestFor(c, locale) >::type,
				Rest... >::TestFor(c, locale);
		}
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c) {
			return TestValueStruct< typename BoolTypeStruct
					< TestValueStruct<Check>::TestFor(c)
					&& TestValueStruct<Check2>::TestFor(c) >::type,
				Rest... >::TestFor(c);
		}
	};

	template<class Check, class Check2, class... Rest>
	struct TestValueStruct<Check, logic::NAND, Check2, Rest...> {
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c, const std::locale& locale) {
			return TestValueStruct< typename
					BoolTypeStruct< !(TestValueStruct<Check>::TestFor(c, locale)
					&& TestValueStruct<Check2>::TestFor(c, locale)) >::type,
				Rest... >::TestFor(c, locale);
		}
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c) {
			return TestValueStruct< typename
					BoolTypeStruct< !(TestValueStruct<Check>::TestFor(c)
					&& TestValueStruct<Check2>::TestFor(c)) >::type,
				Rest... >::TestFor(c);
		}
	};

	template<class Check, class... Rest>
	struct TestValueStruct<Check, logic::OR, Rest...> {
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c, const std::locale& locale) {
			return TestValueStruct<Check>::TestFor(c, locale) ||
			TestValueStruct<Rest...>::TestFor(c, locale);
		}
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c) {
			return TestValueStruct<Check>::TestFor(c) ||
			TestValueStruct<Rest...>::TestFor(c);
		}
	};

	template<class Check, class... Rest>
	struct TestValueStruct<Check, logic::NOR, Rest...> {
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c, const std::locale& locale) {
			return !(TestValueStruct<Check>::TestFor(c, locale) ||
			TestValueStruct<Rest...>::TestFor(c, locale));
		}
		template<class CompareType>
		static constexpr IFLEX_FORCE_INLINE
		bool TestFor(CompareType c) {
			return !(TestValueStruct<Check>::TestFor(c) ||
			TestValueStruct<Rest...>::TestFor(c));
		}
	};

	template <class...Ts>
	struct TestValueStruct<std::tuple<Ts...>> : TestValueStruct<Ts...> {};


	template<class CompareType, class...Expression>
	IFLEX_FORCE_INLINE
	bool TestValue(CompareType c) {
		return TestValueStruct<typename dtg::revert_pack<Expression...>::type>::TestFor(c);
	}
	template<class CompareType, class...Expression>
	IFLEX_FORCE_INLINE
	bool TestValue(CompareType c, const std::locale& locale) {
		return TestValueStruct<typename dtg::revert_pack<Expression...>::type>::TestFor(c, locale);
	}
}//iflex
#endif
