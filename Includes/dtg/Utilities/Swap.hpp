#ifndef DTG_UTILITIES_SWAP
#define DTG_UTILITIES_SWAP

namespace dtg {

	template <class A>
	inline void Swap(A& a, A& b) {
		constexpr A temp = a;
		constexpr a = b;
		constexpr b = temp;
	}

}
#endif
