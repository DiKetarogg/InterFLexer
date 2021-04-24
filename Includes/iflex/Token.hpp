#pragma once
#ifndef IFLEX_TOKEN_HPP
#define IFLEX_TOKEN_HPP

#include <dtg/Utilities.hpp>
#include <regex>

namespace iflex {
	template<class Type, Type NA>
	class Tokenable;
	template<class Traits>
	class Tokenizer;
	template<class TokenTypee, /*size_t TokenizerSize,*/ TokenTypee NOTAPPLICABLE>
	struct Traits {
		private:
		
		Traits() = delete;
		Traits(const Traits&) = delete;
		Traits(Traits&&) = delete;
		
		friend class Tokenizer<Traits>;
		public:
		using TokenType		= TokenTypee;
		typedef iflex::Tokenizer<Traits> Tokenizer;
		static const TokenType NA = NOTAPPLICABLE;
		using Tokenable = iflex::Tokenable<TokenType, NA>;

		//private:
		//static TokenableType* names = Names;
	};

	template<class Traits>
	class Token {
	public:
		using Type = typename Traits::TokenType;
		Token():m_Name(0),m_Type(Traits::NA){}
		
		Token(const char* name, Type type, size_t line, size_t character)
			:m_Name(const_cast<char*>(name)),
			m_Line(line),m_Char(character),m_Type(type){}

		Token(const char* name, Type type, const std::pair<size_t, size_t>& location)
			:m_Name(const_cast<char*>(name)),
			m_Line(location.first),m_Char(location.second),m_Type(type){}

		void SetName(const char* name) {
			m_Name = name;
		}

		void SetLine(size_t line) {
			m_Line = line;
		}

		void SetChar(size_t ch) {
			m_Char = ch;
		}

		void SetType(Type type) {
			m_Type = type;
		}

		void Set(const char* name, size_t line, size_t ch, Type type) {
		m_Name = const_cast<char*>(name);
		m_Line = line;
		m_Char = ch;
		m_Type = type;
		}
	
		const char* GetName() const {
			return m_Name;
		}

		size_t GetLine() const {
			return m_Line;
		}

		size_t GetChar() const {
			return m_Char;
		}

		Type GetType() const {
			return m_Type;
		}	
	private:
		char*	m_Name;
		size_t	m_Line;
		size_t	m_Char;
		Type	m_Type;
	};

	template<class Type, Type NA>
	class Tokenable {
	public:
		enum {
			WORD,
			REGULAR,
			SPECIAL,
			COMPOUND
		};
		Tokenable():m_Name(const_cast<char*>("")), m_Token(NA){};

		Tokenable(const char* name, Type token, bool regular = false)
			:m_Name(
				regular ?
					reinterpret_cast<char*>(new std::regex(name))
				:
					const_cast<char*>(name)),
			m_Token(token),
			m_Type(regular ? REGULAR : WORD){}

		Tokenable(const Tokenable& other)
			:m_Name(
			other.m_Type == REGULAR ?
					reinterpret_cast<char*>
					(new std::regex(*other.m_Regular))
				:
					other.m_Name
			),
			m_Token(other.m_Token), m_Type(other.m_Type){}

		void Swap(Tokenable& other) {
			dtg::Swap(m_Name, other.m_Name);
			dtg::Swap(m_Token, other.m_Token);
			dtg::Swap(m_Type, other.m_Type);
		}

		const char* GetName() const {
			if (m_Type == REGULAR)
				return 0;
			return m_Name;
		}

		const std::regex* GetRegular() const {
			if (m_Type != REGULAR)
				return 0;
			return m_Regular;
		}

		bool Regular() const {
			return m_Type == REGULAR;
		}

		decltype(WORD) GetType() const {
			return m_Type;
		}

		Type GetToken() const {
			return m_Token;
		}

		Tokenable& operator=(const Tokenable& other) {
			if (m_Type == REGULAR)
				delete m_Regular;

			if (other.m_Type == REGULAR)
				m_Regular = new std::regex(*other.m_Regular);
			else
				m_Name = other.m_Name;

			m_Type = other.m_Type;
			m_Token = other.m_Token;
			return *this;
		}

		Tokenable& operator=(Tokenable&& other) {
			m_Name = other.m_Name;
			m_Type = other.m_Type;
			m_Token = other.m_Token;
			other.m_Name = 0;
			return *this;
		}

		~Tokenable() {
			if (m_Type == REGULAR)
				delete m_Regular;
		}
	private:

		union {
		char*		m_Name;
		std::regex* 	m_Regular;
		};
		Type		m_Token;
		decltype(WORD)	m_Type;
	};

	template <class Traits>
	class Tokenizer {
		public:
		using Type = typename Traits::TokenType;
		using Tokenable = typename Traits::Tokenable;
		using Token = typename iflex::Token<Traits>;
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
			Tokenable temp;
			size_t& j = m_LastWord;
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
			
			//m_LastWord = j;
		}

		void Sort() {
			SortOutRegular();
			if (!m_Tokenables[m_LastWord].Regular()) {// Check if there are non-regular tokens
				dtg::MergeSort<Tokenable>(m_Tokenables, m_LastWord + 1,
					[](const Tokenable& first, const Tokenable& second)->bool {
						return strcmp(first.GetName(), second.GetName()) < 0;
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

		Type FindRegular(const char* name) const {
			for (size_t i = m_LastWord + 1; i != m_Size; ++i) {
				if (std::regex_match(name, *m_Tokenables[i].GetRegular()))
					return m_Tokenables[i].GetToken();
			}
			return Traits::NA;
		}

		public:

		Type Find(const char* name) const {
			if (!m_LastWord)
				return FindRegular(name);
			size_t l = 0;
			size_t m = m_LastWord / 2;
			size_t r = m_LastWord;
			int temp;
			for (;;) {
				if ((temp = strcmp(name,m_Tokenables[m].GetName())) > 0) {
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

		const char* Find(Type token) const {
			size_t l = 0;
			size_t m = m_LastWord / 2;
			size_t r = m_LastWord;
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
			//static_assert(true, "iflex::Tokenizer::Find() Token Type not found.");
			return "";
		}

		Token Tokenize(const char* name, size_t character, size_t line) {
			return Token(name, Find(name), character, line);
		}

		~Tokenizer() {
			delete m_MapTokenable;
		}
		private:
		Tokenable*		m_Tokenables;
		Tokenable**		m_MapTokenable;
		size_t			m_LastWord;
		size_t			m_Size;
	};
#if defined(_GLIBCXX_OSTREAM) or defined(_OSTREAM_)

template<class Traits>
std::ostream& operator <<(std::ostream& stream, const Token<Traits>& other) {
	stream << '\'' << other.GetName() << '\'' << "{Line:" <<
	other.GetLine() << ", Char:" << other.GetChar() << ", ID: " << other.GetType() << '}';
	return stream;
}
#endif

}

#endif



















