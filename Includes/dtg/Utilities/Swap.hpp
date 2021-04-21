#ifndef DTG_UTILITIES_SWAP
#define DTG_UTILITIES_SWAP

namespace dtg {

	template <class A>
	inline void Swap(A& a, A& b) {
		A temp = a;
		a = b;
		b = temp;
	}

}
#endif
