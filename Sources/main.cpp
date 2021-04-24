#include <iostream>
#include <iflex/Token.hpp>

namespace my {
	enum Tokens {
		NA,
		PROGRAM,
		NAME,
	};

	typedef iflex::Traits<Tokens, NA> Traits;
	class Tokenizer : iflex::Tokenizer<Traits> {
		public:

		using Base = iflex::Tokenizer<Traits>;
		using Base::Tokenize;
		private:

		Tokenizer(){
			Initialize(m_Names, sizeof(m_Names) / sizeof(Tokenable));
		}
		Tokenizer(const Tokenizer&) = delete;
		Tokenizer(Tokenizer&&) = delete;
		public:
		static Tokenizer& Get() {
			static Tokenizer t;
			return t;
		}

		static auto Find(const char* name) {
			return Get().Base::Find(name);
		}

		static auto Find(Tokens token) {
			return Get().Base::Find(token);
		}

		private:
		Tokenable m_Names[3] {
			{"", NA},
			{"program", PROGRAM},
			{"_[a-z]*[A-Z]*[0-9]*", NAME, true},
		};


	};
	/*
	class TokenList {
		public:
		using Tokenable = iflex::Tokenable<Tokens,NA>;
		private:
		Tokenable m_Tokenables[1] {
			{"", NA}
		};
		public:
		typedef iflex::Traits<Tokenable, sizeof(m_Tokenables) / sizeof(Tokenable)> Traits;
		using Tokenizer = Traits::Tokenizer;

		private:
		static TokenList& GetSelf() {
			static TokenList t;
			return t;
		}
		public:
		static Tokenable* Get() {
			return GetSelf().m_Tokenables;
		}

		private:
	};
	*/
//	typedef TokenList::Traits Traits;

}




int main () {
	using namespace iflex;
	iflex::Token<my::Traits> token(my::Tokenizer::Find(my::PROGRAM), my::PROGRAM, 1, 1);
	Token<my::Traits> token2 = my::Tokenizer::Get().Tokenize("_aZ0", 2, 37);
	Token<my::Traits> token3 = my::Tokenizer::Get().Tokenize("_0", 2, 37);
	Token<my::Traits> token4 = my::Tokenizer::Get().Tokenize("aZ0", 2, 37);
	std::cout << token << '\n' << token2 << '\n' 
	<< token3 << '\n' << token4;;
}
