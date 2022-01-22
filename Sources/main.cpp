#include <iostream>
#include <iflex/Traits.hpp>
#include <string>
namespace my {
	enum Tokens {
		NA, //''
		INT, // 42
		LP, // (
		RP, // )
		LB, // {
		RB, // }
		EQ, // =
		END_EXPR, // ;
		RETURN, // return
		NAME,
	};
	typedef iflex::Traits<Tokens, NA> Traits;

	enum class ByteCode : uint8_t {
		CONSTANT_LITERAL,
		CONSTANT_INT4,
		END_COMPILER_INFO,
		BEGIN_FUNCTION,
		END_FUCTION,
		RETURN,
		ASSIGN,
	};

	class Tokenizer : public iflex::Tokenizer<Traits> {
		public:
		using Base = iflex::Tokenizer<Traits>;
		using CharType = typename Traits::CharType;
		using Tokenable = typename Traits::Tokenable;
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
		private:
		Tokenable m_Names[9] {
			{"int", INT},
			{"(", LP},
			{")", RP},
			{"{", LB},
			{"}", RB},
			{"=", EQ},
			{";", END_EXPR},
			{"return", RETURN},
			{"[_A-Za-z][_A-Za-z0-9]*", NAME, true},
		};
	};
	class Parser : public iflex::Parser<Traits> {
		struct name_info {
			unsigned int stack_address;
			bool variable;
		};
		Parser() = default;
		Parser(const Parser&) = delete;
		Parser(Parser&&) = delete;
		public:
		static Parser& Get() {
			static Parser parser;
			return parser;
		}
		/*
		Parser():m_Info(1)
		,m_InfoIt(m_Info.begin())
		,m_Bytes(1)
		,m_BytesIt(){}
*//*
		void Parse() {
			const int create = 1,
			create_int = 2 | 1;
			name = 4,
			lp = 8,
			rp = 16;
			unsigned int scope = 0;
			int parenthesis = 0;
			int braces = 0;
			int flag = 0;
			unsigned int stack_counter = 0;
			std::vector<std::map<unsigned int, name_info>> names;
			names.resize(16);
			Tokens token;
			Tokens prev_token;
			if (!ResetActiveToken())
				return;
			do {
				switch (token = GetToken().GetType()) {
					case INT: //int
						if (flag & create)
							std::cerr <<
							"[ERROR]: expected variable name.\n";
						flag |= create;
					break;
					case LP: //(

					break;
					case RP: //)

					break;
					case LB: //{

					break;
					case RB: //}

					break;
					case EQ: //=

					break;
					case END_EXPR: //;

					break;
					case RETURN: //return

					break;
					case NAME:
					if (prev_token != INT || prev_token != EQ)
					break;
					default:
				}
				token = &GetToken();
			} while(Advance());
		}

		void Push(ByteCode token) {
			try {
				m_BytesIt->Push(token);
			} catch(std::runtime_error) {
				m_Bytes.emplace_back();
				++m_BytesIt;
				m_BytesIt->Push(token);
			}
		}
		void Push(int i) {
			Push(ByteCode::CONSTANT_INT);
			try {
				m_BytesIt->Push(i);
			} catch(std::runtime_error) {
				m_Bytes.emplace_back();
				++m_BytesIt;
				m_BytesIt->Push(i);
			}
		}
*/		
		private:
		//std::list<dtg::RawStack<ByteCode, 4096>> m_Info;
		//std::list<dtg::RawStack<ByteCode, 4096>>::iterator m_InfoIt;
		//std::list<dtg::RawStack<ByteCode, 4096>> m_Bytes;
		//std::list<dtg::RawStack<ByteCode, 4096>>::iterator m_BytesIt;
	};


	class Lexer : public iflex::Lexer<Traits> {
		using Base = iflex::Lexer<Traits>;
		Lexer(Parser& parser):Base(512, parser, my::Tokenizer::Get()){}
		public:
		static Lexer& Get() {
			static Lexer t(my::Parser::Get());
			return t;
		}
		void Lex() {
			using namespace iflex::is;
			using namespace iflex::logic;
			while (Lexing()) {
				Skip<Space>();
				Read< AlNu, Or, Value<'_'> >();
				if (TokenEmpty()) {
					ReadOne<Any>();
				}
				Flush();
			}
		}
		template<size_t size>
		void ReadString(const char (&str)[size]) {
			StringOpen(str, size);
		}
		void ReadFile(const char *str) {
			FileOpen(str);
		}
	};
}

int main (int argc, char *argv[]) {
	if (argc != 3) {
		std::cout << "Current compiler version only supports 2 arguments.\n";
		return 0;
	}
	my::Lexer& lexer = my::Lexer::Get();
	lexer.ReadFile(argv[1]);
	lexer.Lex();
	std::cout << "Test\n";
	for (const auto& e : my::Parser::Get())
		std::cout << e << '\n';
	std::cout << "\n\n";
	for (const auto& e : my::Parser::Get().GetNames()) {
		std::cout << e << '\n';
	}
	/*
	using namespace iflex;
	my::Lexer& lexer = my::Lexer::Get();
	lexer.ReadString("program _aZ0 _0 aZ0 0Z0 function");
	lexer.Lex();//    012345678901234567890123
	for (const auto& e : my::parser)
		std::cout << e;
	*/
	/*
	my::Tokenizer& tokenizer = my::Tokenizer::Get();
	Token<my::Traits> token1(tokenizer.Find(my::PROGRAM), my::PROGRAM, 1, 1);
	Token<my::Traits> token2 = tokenizer.Tokenize("_aZ0", 2, 37);
	Token<my::Traits> token3 = tokenizer.Tokenize("_0", 2, 37);
	Token<my::Traits> token4 = tokenizer.Tokenize("aZ0", 2, 37);
	Token<my::Traits> token5 = tokenizer.Tokenize("0Z0", 2, 37);
	std::cout << token1 << '\n' << token2 << '\n' 
	<< token3 << '\n' << token4 << '\n' << token5;
	*/
}
